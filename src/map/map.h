#pragma once

#include <vector>

#include "case.h"

struct Coordinate {
  int x;
  int y;
};

using vector_coordinates = std::vector<Coordinate>;

class Map {
 public:
  Map(int nb_player);

  void create_map(const std::vector<std::unique_ptr<Player>>& players);
  void render_debug();
  int distance_between(Coordinate c1, Coordinate c2);

 private:
  void _link_hex_neighbors(int row, int col);
  void _generate_ocean();
  void _generate_plains(const std::vector<std::unique_ptr<Player>>& players);
  void _generate_snow();
  void _generate_coasts();
  void _generate_mountains();
  void _generate_player(const std::vector<std::unique_ptr<Player>>& players);
  int _get_hex_distance(int r1, int c1, int r2, int c2) const;

  int _size_h;
  int _size_w;

  std::vector<std::vector<Case>> _cases;
};