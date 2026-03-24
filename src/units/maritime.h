#include "units.h"

class Maritime : public Unit {
 public:
  Maritime(UnitName name, Country country, int pv, int speed, int power,
           int defense, int range);
};