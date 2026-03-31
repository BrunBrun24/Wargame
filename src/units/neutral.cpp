#include "neutral.h"

Neutral::Neutral(UnitName name, Player* player, Case* case_unit)
    : Unit(name, player, case_unit,
           {TerrainsType::Plains, TerrainsType::Snow}) {}