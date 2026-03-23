#include "units.h"

class Maritime : public Unit {
 public:
  Maritime(std::string name, Country country, int pv, int speed, int power,
           int defense, int range, std::vector<TerrainsType> allow_terrain);
};