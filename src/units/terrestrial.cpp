#include "terrestrial.h"

#include <vector>

#include "case.h"
#include "resource_database.h"

Terrestrial::Terrestrial(UnitName name, Player* player, Case* case_unit)
    : Unit(name, player, case_unit,
           {TerrainsType::Desert, TerrainsType::Grassland, TerrainsType::Ice,
            TerrainsType::Plains, TerrainsType::Tundra}) {}

std::vector<UnitAction> Terrestrial::get_unit_actions(const Unit* unit) {
  std::vector<UnitAction> available_actions = Unit::get_unit_actions(unit);
  Case* unit_case = unit->get_case_unit();
  Terrain terrain = unit_case->get_terrain();

  // Si c'est un ouvrier
  if (unit->get_name() == UnitName::Worker) {
    // On regarde s'il y a des arbres
    if (unit_case->get_terrain().feature != TerrainFeature::None) {
      available_actions.push_back(UnitAction::ChopDownForest);
    }

    // On regarde s'il y a une ressource
    if (terrain.resource != ResourceName::None &&
        terrain.improvement == ImprovementName::None) {
      available_actions.push_back(ResourceDatabase::get_info(terrain.resource)
                                      .resource_bonus.unit_action);
    }
  }
  // Sinon c'est une unité militaire
  else {
    if (terrain.improvement != ImprovementName::None) {
      available_actions.push_back(UnitAction::Pillage);
    }
  }

  return available_actions;
}
