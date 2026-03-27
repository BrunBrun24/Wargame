#include "terrestrial.h"

Terrestrial::Terrestrial(UnitName name, Country country, Case* case_unit)
    : Unit(name, country, case_unit,
           {TerrainsType::Plains, TerrainsType::Snow}) {}