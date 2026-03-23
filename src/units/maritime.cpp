#include "maritime.h"

Maritime::Maritime(int pv, int speed, int power, int defense,
                   std::vector<TerrainsType> allow_terrain)
    : Unit(pv, speed, power, defense,
           {TerrainsType::CoastLake, TerrainsType::Ocean}) {}