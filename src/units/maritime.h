#pragma once

#include "units.h"

class Maritime : public Unit {
 public:
  Maritime(UnitName name, Country country, Case* case_unit);
};