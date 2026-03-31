#pragma once

#include "units.h"

class Aerial : public Unit {
 public:
  Aerial(UnitName name, Player* player, Case* case_unit);
};