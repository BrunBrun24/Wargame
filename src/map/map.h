#include <vector>

#include "case.h"

class Map {
 public:
  void create_map();
  void render_debug();

 private:
  void _link_hex_neighbors(int row, int col);

 private:
  static constexpr int SIZE_H = 38;
  static constexpr int SIZE_W = 38;

  std::vector<std::vector<Case>> _cases;
};