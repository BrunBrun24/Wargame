#include "map.h"

#include <cstdlib>
#include <ctime>

Case::Case() : _terrains(TerrainsType::Plains) {}

Case::Case(TerrainsType type) : _terrains(type) {}

void Map::create_map() {
  std::srand(static_cast<unsigned int>(std::time(nullptr)));
  _cases.resize(SIZE_H, std::vector<Case>(SIZE_W));

  _generate_ocean();
  _generate_plains();
  _generate_snow();
  _generate_coasts();
  _generate_mountains();
}

void Map::_link_hex_neighbors(int row, int col) {
  std::vector<std::pair<int, int>> directions;

  if (row % 2 == 0) {
    directions = {
        {row - 1, col},      // Haut Gauche
        {row - 1, col + 1},  // Haut Droite
        {row, col - 1},      // Gauche
        {row, col + 1},      // Droite
        {row + 1, col},      // Bas Gauche
        {row + 1, col + 1}   // Bas Droite
    };
  } else {
    directions = {
        {row - 1, col - 1},  // Haut Gauche
        {row - 1, col},      // Haut Droite
        {row, col - 1},      // Gauche
        {row, col + 1},      // Droite
        {row + 1, col - 1},  // Bas Gauche
        {row + 1, col}       // Bas Droite
    };
  }

  for (auto& dir : directions) {
    int r = dir.first;
    int c = dir.second;

    if (r >= 0 && r < SIZE_H && c >= 0 && c < SIZE_W) {
      _cases[row][col].add_neighbor(&_cases[r][c]);
    }
  }
}

void Map::_generate_ocean() {
  for (int row = 0; row < SIZE_H; row++) {
    for (int col = 0; col < SIZE_W; col++) {
      _cases[row][col] = Case(TerrainsType::Ocean);
      _link_hex_neighbors(row, col);
    }
  }
}

void Map::_generate_plains() {
  // Placer des graines de terre (ex: 5 graines)
  int num_seeds = 10;
  std::vector<Case*> land_cases;

  for (int i = 0; i < num_seeds; ++i) {
    int r = std::rand() % SIZE_H;
    int c = std::rand() % SIZE_W;
    _cases[r][c].set_terrain_type(TerrainsType::Plains);
    land_cases.push_back(&_cases[r][c]);
  }

  // Propagation (on étend la terre à certains voisins)
  for (int iteration = 0; iteration < 2000; ++iteration) {
    if (land_cases.empty()) break;

    // On choisit une case de terre existante au hasard
    int idx = std::rand() % land_cases.size();
    Case* current = land_cases[idx];

    const std::vector<Case*>& neighbors = current->get_neighbors();

    for (Case* neighbor : neighbors) {
      // 30% de chance de transformer le voisin en terre s'il est encore de
      // l'eau
      if (neighbor->get_terrain().get_terrain_type() == TerrainsType::Ocean &&
          (std::rand() % 100 < 30)) {
        neighbor->set_terrain_type(TerrainsType::Plains);
        land_cases.push_back(neighbor);
      }
    }
  }
}

void Map::_generate_snow() {
  // Mettre la neige à moins de 5 blocs en haut
  for (int row = 0; row < 4; row++) {
    for (int col = 0; col < SIZE_W; col++) {
      if (_cases[row][col].get_terrain().get_terrain_type() ==
          TerrainsType::Plains) {
        _cases[row][col].set_terrain_type(TerrainsType::Snow);
      }
    }
  }

  // Mettre la neige à moins de 5 blocs en bas
  for (int row = SIZE_H - 4; row < SIZE_H; row++) {
    for (int col = 0; col < SIZE_W; col++) {
      if (_cases[row][col].get_terrain().get_terrain_type() ==
          TerrainsType::Plains) {
        _cases[row][col].set_terrain_type(TerrainsType::Snow);
      }
    }
  }
}

void Map::_generate_coasts() {
  // On répète 3 fois pour la distance de 3 cases
  for (int dist = 0; dist < 2; dist++) {
    std::vector<Case*> ocean_to_coast;

    // On parcourt toute la carte
    for (int row = 0; row < SIZE_H; row++) {
      for (int col = 0; col < SIZE_W; col++) {
        // Si la case est de l'eau
        if (_cases[row][col].get_terrain().get_terrain_type() ==
            TerrainsType::Ocean) {
          // On vérifie si elle a un voisin qui est de la terre ou déjà une côte
          for (Case* neighbor : _cases[row][col].get_neighbors()) {
            TerrainsType n_type = neighbor->get_terrain().get_terrain_type();

            if (n_type == TerrainsType::Plains ||
                n_type == TerrainsType::CoastLake ||
                n_type == TerrainsType::Snow) {
              ocean_to_coast.push_back(&_cases[row][col]);
              break;  // Pas besoin de vérifier les autres voisins
            }
          }
        }
      }
    }

    // On transforme les cases trouvées en CoastLake
    for (Case* c : ocean_to_coast) {
      c->set_terrain_type(TerrainsType::CoastLake);
    }
  }
}

void Map::_generate_mountains() {
  for (int row = 0; row < SIZE_H; row++) {
    for (int col = 0; col < SIZE_W; col++) {
      Case& current = _cases[row][col];

      // On ne place des montagnes que sur les Plaines (pas sur l'Océan ou la
      // Neige)
      if (current.get_terrain().get_terrain_type() == TerrainsType::Plains) {
        int land_neighbors = 0;

        // On compte combien de voisins sont de la terre
        for (Case* neighbor : current.get_neighbors()) {
          TerrainsType n_type = neighbor->get_terrain().get_terrain_type();
          if (n_type == TerrainsType::Plains ||
              n_type == TerrainsType::Mountains) {
            land_neighbors++;
          }
        }

        // Si la case est bien entourée (ex: 5 ou 6 voisins terrestres)
        // On ajoute un facteur chance (15%) pour ne pas avoir trop de montagnes
        if (land_neighbors >= 5 && (std::rand() % 100 < 10)) {
          current.set_terrain_type(TerrainsType::Mountains);
        }
      }
    }
  }
}

void Map::render_debug() {
  for (int row = 0; row < SIZE_H; row++) {
    if (row % 2 != 0) std::cout << "  ";

    for (int col = 0; col < SIZE_W; col++) {
      TerrainsType t = _cases[row][col].get_terrain().get_terrain_type();

      if (t == TerrainsType::Ocean)
        // Bleu foncé profond (0, 0, 150)
        std::cout << "\033[38;2;0;0;150m[O]\033[0m ";
      else if (t == TerrainsType::CoastLake)
        // Bleu clair turquoise (0, 180, 220)
        std::cout << "\033[38;2;0;180;220m[C]\033[0m ";
      else if (t == TerrainsType::Plains)
        // Vert herbe (50, 160, 50)
        std::cout << "\033[38;2;50;160;50m[P]\033[0m ";
      else if (t == TerrainsType::Snow)
        // Blanc pur brillant (255, 255, 255)
        std::cout << "\033[38;2;255;255;255m[S]\033[0m ";
      else if (t == TerrainsType::Mountains)
        // Gris roche / Marron (110, 90, 70)
        std::cout << "\033[38;2;110;90;70m[M]\033[0m ";
      else
        std::cout << "[" << _cases[row][col].get_terrain().get_debug_char()
                  << "] ";
    }
    std::cout << std::endl;
  }
}