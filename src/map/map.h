#include <vector>

#include "case.h"

class Map {
 public:
  void create_map();

 private:
  void _link_hex_neighbors(int row, int col);

 private:
  static constexpr int SIZE_H = 10;
  static constexpr int SIZE_W = 10;

  std::vector<std::vector<Case>> _cases;
};