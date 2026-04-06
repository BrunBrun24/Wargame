#pragma once

#include <map>
#include <vector>

#include "type.h"

struct ImprovementData {
  std::vector<TerrainsType> terrain_type =
      {};  // Type de terrain où l'on peut construire
  std::vector<TerrainElevation> terrain_elevation =
      {};  // Relief du terrain  où l'on peut construire
  std::vector<TerrainFeature> terrain_feature =
      {};  // Spécificité du terrain où l'on peut construire
  Yields yields = {};
  int pillage = 0;
  int turns = 0;  // Nombre de tours avant que la production ne soit finie
};

class ImprovementDatabase {
 public:
  static const ImprovementData& get_info(ImprovementName name) {
    return _data.at(name);
  }

 private:
  static std::map<ImprovementName, ImprovementData> _data;
};