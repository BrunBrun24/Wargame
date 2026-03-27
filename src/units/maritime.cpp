#include "maritime.h"

Maritime::Maritime(UnitName name, Country country, Case* case_unit)
    : Unit(name, country, case_unit,
           {TerrainsType::CoastLake, TerrainsType::Ocean}) {}