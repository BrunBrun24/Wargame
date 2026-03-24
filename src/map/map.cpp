#include "map.h"

#include <cstdlib>
#include <ctime>

Case::Case() : _terrains(TerrainsType::Grassland) {}

Case::Case(TerrainsType type) : _terrains(type) {}

void Map::create_map() {
  std::srand(std::time(nullptr)); // Initialisation du hasard
  _cases.resize(SIZE_H, std::vector<Case>(SIZE_W));

  // Initialiser tout en Océan et lier les voisins
  for (int row = 0; row < SIZE_H; row++) {
    for (int col = 0; col < SIZE_W; col++) {
      _cases[row][col] = Case(TerrainsType::Ocean);
      _link_hex_neighbors(row, col);
    }
  }

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

void Map::render_debug() {
  for (int row = 0; row < SIZE_H; row++) {
    if (row % 2 != 0) std::cout << "  ";

    for (int col = 0; col < SIZE_W; col++) {
      TerrainsType t = _cases[row][col].get_terrain().get_terrain_type();

      // Codes couleurs ANSI : Bleu pour Ocean, Vert pour Plains
      if (t == TerrainsType::Ocean)
        std::cout << "\033[1;34m[O]\033[0m ";
      else if (t == TerrainsType::Plains)
        std::cout << "\033[32m[P]\033[0m ";
      else
        std::cout << "[" << _cases[row][col].get_terrain().get_debug_char()
                  << "] ";
    }
    std::cout << std::endl;
  }
}