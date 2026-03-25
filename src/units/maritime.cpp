#include "maritime.h"

Maritime::Maritime(UnitName name, Country country)
    : Unit(name, country, {TerrainsType::CoastLake, TerrainsType::Ocean}) {}