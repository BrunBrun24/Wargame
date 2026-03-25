#include "terrestrial.h"

Terrestrial::Terrestrial(UnitName name, Country country)
    : Unit(name, country, {TerrainsType::Plains, TerrainsType::Snow}) {}