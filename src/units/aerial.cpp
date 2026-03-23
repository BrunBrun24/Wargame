#include "aerial.h"

Aerial::Aerial(int pv, int speed, int power, int defense,
               std::vector<TerrainsType> allow_terrain)
    : Unit(pv, speed, power, defense,
           {TerrainsType::CoastLake, TerrainsType::Desert,
            TerrainsType::Grassland, TerrainsType::Ocean, TerrainsType::Plains,
            TerrainsType::Snow}) {}