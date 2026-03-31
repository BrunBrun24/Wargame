#include "aerial.h"

Aerial::Aerial(UnitName name, Player* player, Case* case_unit)
    : Unit(name, player, case_unit,
           {TerrainsType::CoastLake, TerrainsType::Ocean, TerrainsType::Plains,
            TerrainsType::Snow}) {}