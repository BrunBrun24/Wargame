#include "units.h"

class Terrestrial : public Unit {
 public:
  Terrestrial(UnitName name, Country country, int pv, int speed, int power,
              int defense, int range);
};
