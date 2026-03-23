#include "map.h"

void Map::create_map() {
  _cases.resize(SIZE_H, std::vector<Case>(SIZE_W));

  for (int row = 0; row < SIZE_H; row++) {
    for (int col = 0; col < SIZE_W; col++) {
      _link_hex_neighbors(row, col);
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
