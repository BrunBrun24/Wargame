#include "maritime.h"

#include <vector>

#include "case.h"
#include "resource_database.h"

Maritime::Maritime(UnitName name, Player* player, Case* case_unit)
    : Unit(name, player, case_unit,
           {TerrainsType::Coast, TerrainsType::Ocean}) {}

std::vector<UnitAction> Maritime::get_unit_actions(const Unit* unit) {
  std::vector<UnitAction> available_actions = Unit::get_unit_actions(unit);
  Case* unit_case = unit->get_case_unit();
  Terrain terrain = unit_case->get_terrain();

  if (unit->get_name() == UnitName::WorkBoat) {
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
