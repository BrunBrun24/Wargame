#include "terrestrial.h"

Terrestrial::Terrestrial(int pv, int speed, int power, int defense,
                         std::vector<TerrainsType> allow_terrain)
    : Unit(pv, speed, power, defense,
           {TerrainsType::Desert, TerrainsType::Grassland, TerrainsType::Plains,
            TerrainsType::Snow}) {}