#include "terrestrial.h"

Terrestrial::Terrestrial(UnitName name, Country country, int pv, int speed,
                         int power, int defense, int range)
    : Unit(name, country, pv, speed, power, defense, range,
           {TerrainsType::Plains, TerrainsType::Snow}) {}