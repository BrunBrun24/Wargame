#include <iostream>

#include "map.h"
#include "terrestrial.h"
#include "units.h"

int main() {
  Map map;
  map.create_map();
//   map.render_debug();

  Terrestrial my_warrior(UnitName::Archer, Country::France, 100, 2, 20, 10, 1);
  my_warrior.get_unit_advantages();

  std::cout << "Good Execution !";
  return 0;
}