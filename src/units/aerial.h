#include "units.h"

class Aerial : public Unit {
 public:
  Aerial(int pv, int speed, int power, int defense,
         std::vector<TerrainsType> allow_terrain);
};