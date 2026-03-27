#include "aerial.h"

Aerial::Aerial(UnitName name, Country country, Case* case_unit)
    : Unit(name, country, case_unit,
           {TerrainsType::CoastLake, TerrainsType::Ocean, TerrainsType::Plains,
            TerrainsType::Snow}) {}