#include "maritime.h"

Maritime::Maritime(UnitName name, Player* player, Case* case_unit)
    : Unit(name, player, case_unit,
           {TerrainsType::CoastLake, TerrainsType::Ocean}) {}