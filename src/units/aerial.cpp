#include "aerial.h"

Aerial::Aerial(UnitName name, Country country)
    : Unit(name, country,
           {TerrainsType::CoastLake, TerrainsType::Ocean, TerrainsType::Plains,
            TerrainsType::Snow}) {}