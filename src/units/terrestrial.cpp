#include "terrestrial.h"

Terrestrial::Terrestrial(std::string name, Country country, int pv, int speed,
                         int power, int defense, int range,
                         std::vector<TerrainsType> allow_terrain)
    : Unit(name, country, pv, speed, power, defense, range,
           {TerrainsType::Desert, TerrainsType::Grassland, TerrainsType::Plains,
            TerrainsType::Snow}) {}