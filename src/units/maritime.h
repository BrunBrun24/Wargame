#include "units.h"

class Maritime : public Unit {
 public:
  Maritime(int pv, int speed, int power, int defense,
           std::vector<TerrainsType> allow_terrain);
};