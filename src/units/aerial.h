#include "units.h"

class Aerial : public Unit {
 public:
  Aerial(UnitName name, Country country, int pv, int speed, int power,
         int defense, int range);
};