#include "terrestrial.h"

#include <vector>

#include "case.h"
#include "player.h"
#include "resource_database.h"

Terrestrial::Terrestrial(UnitName name, Player* player, Case* case_unit)
    : Unit(name, player, case_unit,
           {TerrainsType::Desert, TerrainsType::Grassland, TerrainsType::Ice,
            TerrainsType::Plains, TerrainsType::Tundra}) {}

std::vector<UnitAction> Terrestrial::get_unit_actions(const Unit* unit) {
  std::vector<UnitAction> available_actions = Unit::get_unit_actions();
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

bool Terrestrial::can_build_city() const {
  // 1. Vérification du type d'unité
  if (this->name != UnitName::Settler) {
    return false;
  }

  // 2. On délègue la vérification de distance à la case
  return this->case_unit->is_valid_for_city();
}

void Terrestrial::found_city() {
  if (!can_build_city()) return;

  // On demande à la case de générer la ville
  this->case_unit->create_city(this->player);

  // On supprime le colon
  this->case_unit->remove_unit(this);

  // On retire l'unité de la liste du joueur
  this->player->remove_unit(this);

  // On supprime l'unité
  delete this;
}

bool Terrestrial::chop_down_forest_is_possible() {
  if (this->get_name() != UnitName::Worker) return false;

  TerrainFeature feature = this->get_case_unit()->get_terrain().feature;
  return (feature == TerrainFeature::Forest ||
          feature == TerrainFeature::Jungle);
}

void Terrestrial::chop_down_forest() {
  this->get_case_unit()->get_terrain().feature = TerrainFeature::None;

  this->switch_active();
}
