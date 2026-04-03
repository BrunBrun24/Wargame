#pragma once

#include <map>
#include <vector>

#include "type.h"

class BuildingDatabase {
 public:
  static const Yields& get_info(BuildingName name) { return _data.at(name); }

 private:
  static const std::map<BuildingName, Yields> _data;
};