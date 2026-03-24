#include "aerial.h"

Aerial::Aerial(UnitName name, Country country, int pv, int speed, int power,
               int defense, int range, std::vector<TerrainsType> allow_terrain)
    : Unit(name, country, pv, speed, power, defense, range,
           {TerrainsType::CoastLake, TerrainsType::Ocean, TerrainsType::Plains,
            TerrainsType::Snow}) {}