#pragma once

#include <map>

#include "../type.h"

struct TechnologyInfo {
  int required_technology;
  std::vector<TechnologyName> requires_technology;
};

class Technology {
 public:
  static const TechnologyInfo& get_info(TechnologyName name) {
    return _data.at(name);
  }
  static std::string get_technology_name_str(TechnologyName name);

 private:
  static std::map<TechnologyName, TechnologyInfo> _data;
};
