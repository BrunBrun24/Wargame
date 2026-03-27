#include "neutral.h"

Neutral::Neutral(UnitName name, Country country)
    : Unit(name, country, {TerrainsType::Plains, TerrainsType::Snow}) {}