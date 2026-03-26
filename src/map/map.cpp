#include "map.h"

#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <random>

Map::Map(int nb_player) {
  // Exemple de logique : 20 cases de côté par joueur, avec un minimum de 30
  if (nb_player <= 2) {
    _size_h = 44;
    _size_w = 26;
  } else if (nb_player <= 4) {
    _size_h = 60;
    _size_w = 38;
  } else if (nb_player <= 6) {
    _size_h = 74;
    _size_w = 46;
  } else if (nb_player <= 8) {
    _size_h = 84;
    _size_w = 54;
  } else {
    _size_h = 96;
    _size_w = 60;
  }

  _cases.resize(_size_h, std::vector<Case>(_size_w));
}

void Map::create_map(const std::vector<std::unique_ptr<Player>>& players) {
  std::srand(static_cast<unsigned int>(std::time(nullptr)));

  _generate_ocean();
  _generate_plains(players);
  _generate_snow();
  _generate_coasts();
  _generate_mountains();
  _generate_player(players);
  // _generate_building();
}

int Map::distance_between(Coordinate c1, Coordinate c2) {
  Case case1 = _cases[c1.x][c1.y];
  Case case2 = _cases[c2.x][c2.y];

  return static_cast<int>(
      case1.distance_between(case2).distance_traveled.size());
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

    if (r >= 0 && r < _size_h && c >= 0 && c < _size_w) {
      _cases[row][col].add_neighbor(&_cases[r][c]);
    }
  }
}

void Map::_generate_ocean() {
  for (int row = 0; row < _size_h; row++) {
    for (int col = 0; col < _size_w; col++) {
      _cases[row][col] = Case(TerrainsType::Ocean);
      _link_hex_neighbors(row, col);
    }
  }
}

void Map::_generate_plains(
    const std::vector<std::unique_ptr<Player>>& players) {
  // Placer des graines de terre (ex: 5 graines)
  int num_seeds = 10;
  std::vector<Case*> land_cases;

  for (int i = 0; i < num_seeds; ++i) {
    int r = std::rand() % _size_h;
    int c = std::rand() % _size_w;
    _cases[r][c].set_terrain_type(TerrainsType::Plains);
    land_cases.push_back(&_cases[r][c]);
  }

  // Propagation (on étend la terre à certains voisins)
  for (int iteration = 0; iteration < (750 * players.size()); ++iteration) {
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
    for (int col = 0; col < _size_w; col++) {
      if (_cases[row][col].get_terrain().get_terrain_type() ==
          TerrainsType::Plains) {
        _cases[row][col].set_terrain_type(TerrainsType::Snow);
      }
    }
  }

  // Mettre la neige à moins de 5 blocs en bas
  for (int row = _size_h - 4; row < _size_h; row++) {
    for (int col = 0; col < _size_w; col++) {
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
    for (int row = 0; row < _size_h; row++) {
      for (int col = 0; col < _size_w; col++) {
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
  for (int row = 0; row < _size_h; row++) {
    for (int col = 0; col < _size_w; col++) {
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
  for (int row = 0; row < _size_h; row++) {
    if (row % 2 != 0) std::cout << "  ";

    for (int col = 0; col < _size_w; col++) {
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

void Map::_generate_player(
    const std::vector<std::unique_ptr<Player>>& players) {
  std::vector<std::pair<int, int>> spawn_points;
  std::vector<std::pair<int, int>> coordinate_possible;

  // On récupère toutes les coordonnées la ou l'on peut placer les joueurs
  for (int row = 0; row < _size_h; row++) {
    for (int col = 0; col < _size_w; col++) {
      if (_cases[row][col].get_terrain().get_terrain_type() ==
          TerrainsType::Plains) {
        coordinate_possible.push_back({row, col});
      }
    }
  }

  // On mélange le vecteur des coordonnées possibles pour que le placement soit
  // aléatoire
  // On créer générateur de nombres aléatoires (moteur Mersenne Twister)
  std::random_device rd;
  std::mt19937 g(rd());
  std::shuffle(coordinate_possible.begin(), coordinate_possible.end(), g);

  for (const auto& player : players) {
    bool placed = false;

    while (!placed && !coordinate_possible.empty()) {
      // On récupère la dernière coordonnée
      std::pair<int, int> coord = coordinate_possible.back();
      int r = coord.first;
      int c = coord.second;

      coordinate_possible.pop_back();

      if (_cases[r][c].get_terrain().get_terrain_type() !=
          TerrainsType::Plains) {
        continue;
      }

      bool too_close = false;
      for (const auto& other : spawn_points) {
        int dist = _get_hex_distance(r, c, other.first, other.second);

        if (dist < 20) {
          too_close = true;
          break;
        }
      }

      if (!too_close) {
        spawn_points.push_back({r, c});

        // On ajoute un colon et un warrior sur la case en question

        std::cout << "[Spawn] Joueur placé en (" << r << ", " << c << ")"
                  << std::endl;
        placed = true;
      }
    }
    // Si le dernier joueur n'a pas été placé on recréer la map
    if (!placed) {
      std::cout
          << "[Echec] Plus de cases disponibles. Relance de la génération..."
          << std::endl
          << std::endl;
      create_map(players);
      return;
    }
  }

  std::cout << std::endl;
}

/**
 * @brief Calcule la distance réelle sur une grille hexagonale.
 * Utilise la conversion en coordonnées cubiques pour un résultat exact en O(1).
 */
int Map::_get_hex_distance(int r1, int c1, int r2, int c2) const {
  // Conversion Row/Col (offset) vers Cube (x, y, z)
  int x1 = c1 - (r1 + (r1 & 1)) / 2;
  int z1 = r1;
  int y1 = -x1 - z1;

  int x2 = c2 - (r2 + (r2 & 1)) / 2;
  int z2 = r2;
  int y2 = -x2 - z2;

  // La distance hexagonale est le maximum des différences absolues des axes
  return std::max({std::abs(x1 - x2), std::abs(y1 - y2), std::abs(z1 - z2)});
}