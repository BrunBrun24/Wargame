#include <vector>

#include "case.h"

class Map {
 public:
  void create_map();
  void render_debug();

 private:
  void _link_hex_neighbors(int row, int col);
  void _generate_ocean();
  void _generate_plains();
  void _generate_snow();
  void _generate_coasts();
  void _generate_mountains();

 private:
  static constexpr int SIZE_H = 38;
  static constexpr int SIZE_W = 38;

  std::vector<std::vector<Case>> _cases;
};