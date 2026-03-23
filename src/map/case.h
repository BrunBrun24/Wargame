#include <vector>

#include "terrains.h"
#include "units.h"

class Case {
 public:
  void add_neighbor(Case* neighbor);

 private:
  std::vector<Case*> _neighbors;  // Cases adjacentes
  std::vector<Unit*> _units;      // Unité sur la case
  Terrains _terrains;
};