#include <map>

#include "type.h"

enum class ResourceType { Strategic, Luxury, Health };

struct ResourceBonus {
  ResourceName name;
  ResourceType type;
  int food_bonus;
  int production_bonus;
  int commerce_bonus;
  int happiness_bonus;
  int health_bonus;
};

class ResourceDatabase {
 public:
  static const ResourceBonus& get_info(ResourceName name) {
    return _data.at(name);
  }

 private:
  static const std::map<ResourceName, ResourceBonus> _data;
};
