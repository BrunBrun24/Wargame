#pragma once

#include <map>

#include "type.h"

enum class ResourceType { Strategic, Luxury, Health };

struct ResourceBonus {
  ResourceName name;
  ResourceType type;
  int food;
  int production;
  int commerce;
  int happiness;
  int health;
};

class ResourceDatabase {
 public:
  static const ResourceBonus& get_info(ResourceName name) {
    return _data.at(name);
  }

 private:
  static const std::map<ResourceName, ResourceBonus> _data;
};
