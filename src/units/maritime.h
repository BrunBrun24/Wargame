#pragma once

#include "units.h"

class Maritime : public Unit {
 public:
  Maritime(UnitName name, Player* player, Case* case_unit);
};