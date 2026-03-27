#pragma once

#include <memory>
#include <vector>

#include "case.h"

struct Coordinate {
  int x;
  int y;
};

using vector_coordinates = std::vector<Coordinate>;
using vector_player = std::vector<std::unique_ptr<Player>>;

class Map {
 public:
  Map(int nb_player);

  void create_map();
  void render_debug();
  int distance_between(Coordinate c1, Coordinate c2);
  void add_unit_to_case(Case& target_case, UnitName name,
                        std::unique_ptr<Player>& player);
  void add_building_to_case(Case& target_case, BuildingType type);

  const vector_player& get_players() const { return _players; }
  void delete_player(Player& player);

 private:
  void _link_hex_neighbors(int row, int col);
  void _generate_ocean();
  void _generate_plains();
  void _generate_snow();
  void _generate_coasts();
  void _generate_mountains();
  void _generate_players();
  void _generate_buildings();
  int _get_hex_distance(int r1, int c1, int r2, int c2) const;

  std::unique_ptr<Unit> _create_unit(UnitName name, Country country);

  int _size_h;
  int _size_w;

  std::vector<std::vector<Case>> _cases;
  vector_player _players;
};