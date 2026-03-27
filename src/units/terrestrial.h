#pragma once

#include "units.h"

class Terrestrial : public Unit {
 public:
  Terrestrial(UnitName name, Country country, Case* case_unit);
};
