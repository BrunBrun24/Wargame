#pragma once

#include <map>
#include <vector>

#include "../type.h"

struct BuildingStats {
  int required_production = 0;
  int food = 0;
  int culture = 0;
  int happiness = 0;
  int health = 0;
  int sickness = 0;
  Yields yields;
};

struct BuildingInfo {
  std::vector<TechnologyName> required_tech;
  BuildingStats building_stats;
};

class BuildingDatabase {
 public:
  static const BuildingInfo& get_info(BuildingName name) {
    return _data.at(name);
  }
  static std::string get_building_name_str(BuildingName name);

 private:
  static std::map<BuildingName, BuildingInfo> _data;
};
