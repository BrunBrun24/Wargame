#pragma once

#include <map>

#include "../type.h"

enum class ResourceType { Strategic, Production, Food, Commerce };

struct ResourceBonus {
  ResourceType type;
  UnitAction unit_action;
  Yields yields;
};

struct ImprovementBonus {
  ImprovementName improvement;
  Yields yields;
};

struct Bonus {
  ResourceBonus resource_bonus;
  ImprovementBonus improvement_bonus;
};

class ResourceDatabase {
 public:
  static const Bonus& get_info(ResourceName name) { return _data.at(name); }
  static std::string get_improvement_name_str(ResourceName name);

 private:
  static std::map<ResourceName, Bonus> _data;
};
