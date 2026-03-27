#include "units.h"

class Neutral : public Unit {
 public:
  Neutral(UnitName name, Country country, Case* case_unit);
};