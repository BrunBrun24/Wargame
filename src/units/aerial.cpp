#include "aerial.h"

Aerial::Aerial(UnitName name, Player* player, Case* case_unit)
    : Unit(name, player, case_unit,
           {TerrainsType::Coast, TerrainsType::Ocean, TerrainsType::Plains,
            TerrainsType::Ice}) {}