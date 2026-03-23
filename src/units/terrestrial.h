#include "units.h"

class Terrestrial : public Unit {
 public:
  Terrestrial(int pv, int speed, int power, int defense,
              std::vector<TerrainsType> allow_terrain);
};