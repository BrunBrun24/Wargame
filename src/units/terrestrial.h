#pragma once

#include "units.h"

class Terrestrial : public Unit {
 public:
  Terrestrial(UnitName name, Player* player, Case* case_unit);
};
