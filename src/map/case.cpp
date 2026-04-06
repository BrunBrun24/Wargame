#include "case.h"

#include <algorithm>
#include <queue>
#include <unordered_map>
#include <vector>

#include "aerial.h"
#include "city.h"
#include "improvement_database.h"
#include "maritime.h"
#include "player.h"
#include "resource_database.h"
#include "terrestrial.h"
#include "units.h"

int Case::_id_counter = 0;

Case::Case(Terrain terrain, Country country, Player* player)
    : _id(_id_counter++),
      _terrain(terrain),
      _country(country),
      _player(player),
      _city(nullptr) {}

Unit* Case::select_best_unit(const Unit* unit) const {
  Unit* best_unit = nullptr;
  int damage = 0;

  for (auto* u : _units) {
    int potential_damage = u->calculate_damage(unit);
    if (potential_damage > damage) {
      damage = potential_damage;
      best_unit = u;
    }
  }

  return best_unit;
}

Course Case::distance_between(const Case* target_case) {
  if (this == target_case) {
    return {true, {this}};
  }

  // File pour le parcours en largeur
  std::queue<Case*> queue;
  // Map pour reconstruire le chemin (Enfant -> Parent)
  std::unordered_map<Case*, Case*> parent_map;

  queue.push(this);
  parent_map[this] = nullptr;

  Case* reached_target = nullptr;

  while (!queue.empty()) {
    Case* current = queue.front();
    queue.pop();

    if (current == target_case) {
      reached_target = current;
      break;
    }

    for (Case* neighbor : current->get_neighbors()) {
      // Si le voisin n'a pas encore été visité
      if (parent_map.find(neighbor) == parent_map.end()) {
        parent_map[neighbor] = current;
        queue.push(neighbor);
      }
    }
  }

  // Si on a trouvé la cible, on reconstruit le chemin le plus court
  if (reached_target) {
    std::vector<Case*> path;
    Case* backtrack = reached_target;
    while (backtrack != nullptr) {
      path.insert(path.begin(), backtrack);
      backtrack = parent_map.at(backtrack);
    }
    return {true, path};
  }

  return {false, {}};
}

bool Case::is_valid_for_city() const {
  // 1. Vérification sur la case elle-même
  if (this->_city != nullptr) return false;

  // 2. Vérification des voisins immédiats (distance 1)
  for (Case* neighbor : _neighbors) {
    if (neighbor->get_city() != nullptr) return false;

    // 3. Vérification des voisins des voisins (distance 2)
    for (Case* second_neighbor : neighbor->get_neighbors()) {
      if (second_neighbor->get_city() != nullptr) return false;
    }
  }
  return true;
}

void Case::create_city(Player* player) { _city = new City(this, player); }

Yields Case::get_total_yields() const {
  // 1. Rendement de la case
  Yields total = get_base_yields();

  if ((_terrain.elevation == TerrainElevation::Hill) &&
      (_terrain.feature == TerrainFeature::Forest)) {
    total.production += 1;
  } else if (_terrain.feature == TerrainFeature::Forest) {
    total.production -= 1;
  }

  // 2. Bonus des ressources et des aménagements
  if (_terrain.resource != ResourceName::None) {
    // On regarde s'il y a une ressource sur le terrain
    const Bonus& bonus = ResourceDatabase::get_info(_terrain.resource);
    total.food += bonus.resource_bonus.yields.food;
    total.production += bonus.resource_bonus.yields.production;
    total.commerce += bonus.resource_bonus.yields.commerce;
    total.culture += bonus.resource_bonus.yields.culture;
    total.science += bonus.resource_bonus.yields.science;
    total.happiness += bonus.resource_bonus.yields.happiness;
    total.health += bonus.resource_bonus.yields.health;
    total.sickness += bonus.resource_bonus.yields.sickness;
    total.defense += bonus.resource_bonus.yields.defense;

    // On regarde si il y a un aménagement sur la ressource
    if (_terrain.improvement != ImprovementName::None) {
      total.food += bonus.improvement_bonus.yields.food;
      total.production += bonus.improvement_bonus.yields.production;
      total.commerce += bonus.improvement_bonus.yields.commerce;
      total.culture += bonus.resource_bonus.yields.culture;
      total.science += bonus.resource_bonus.yields.science;
      total.happiness += bonus.improvement_bonus.yields.happiness;
      total.health += bonus.improvement_bonus.yields.health;
      total.sickness += bonus.improvement_bonus.yields.sickness;
      total.defense += bonus.improvement_bonus.yields.defense;
    }
  }

  // 3. Bonus des bâtiments de la ville

  return total;
}

Yields Case::get_base_yields() const {
  switch (_terrain.type) {
    case TerrainsType::Coast:
      return {1, 0, 3, 0, 0, 0, 0, 0, 0};
    case TerrainsType::Grassland:
      return {2, 0, 0, 0, 0, 0, 0, 0, 0};
    case TerrainsType::Ocean:
      return {1, 0, 1, 0, 0, 0, 0, 0, 0};
    case TerrainsType::Plains:
      return {1, 1, 0, 0, 0, 0, 0, 0, 0};
    case TerrainsType::Tundra:
      return {1, 0, 0, 0, 0, 0, 0, 0, 0};
    default:
      return {0, 0, 0, 0, 0, 0, 0, 0, 0};
  }
}

void Case::add_unit(Unit* unit) {
  // On ajoute l'unité sur la case
  _units.push_back(unit);

  // On change le pays se trouvant sur la case
  set_country(unit->get_player()->get_country());
};

void Case::remove_unit(Unit* unit) {
  auto it = std::find(_units.begin(), _units.end(), unit);
  if (it != _units.end()) {
    _units.erase(it);
  }
}

void Case::add_neighbor(Case* neighbor) {
  if (neighbor != nullptr) {
    _neighbors.push_back(neighbor);
  }
}

Country Case::get_unit_country() const {
  if (_units.empty()) {
    return Country::Neutral;
  }

  return _units.front()->get_player()->get_country();
}

std::string Case::get_description() const {
  // 1. Priorité maximale : La Ville
  if (get_city() != nullptr) {
    return "City";
  }

  // 2. Priorité 2 : Les Unités
  if (!_units.empty()) {
    UnitName name = _units[0]->get_name();

    for (const auto& [str, enum_val] : UNIT_STRING_NAME) {
      if (enum_val == name) {
        return "Unit";
      }
    }

    std::string error_msg = "L'unité n'existe pas";
    throw std::runtime_error(error_msg);
  }

  // 3. Priorité 3 : TOUTES les Ressources Naturelles
  ResourceName res = get_terrain().resource;
  if (res != ResourceName::None) {
    switch (res) {
      // Stratégiques
      case ResourceName::Horse:
        return "Res_Horse";
      case ResourceName::Iron:
        return "Res_Iron";
      case ResourceName::Copper:
        return "Res_Copper";
      case ResourceName::Coal:
        return "Res_Coal";
      case ResourceName::Oil:
        return "Res_Oil";
      case ResourceName::Uranium:
        return "Res_Uranium";
      case ResourceName::Aluminum:
        return "Res_Aluminum";

      // Santé
      case ResourceName::Corn:
        return "Res_Corn";
      case ResourceName::Rice:
        return "Res_Rice";
      case ResourceName::Wheat:
        return "Res_Wheat";
      case ResourceName::Banana:
        return "Res_Banana";
      case ResourceName::Deer:
        return "Res_Deer";
      case ResourceName::Pig:
        return "Res_Pig";
      case ResourceName::Cow:
        return "Res_Cow";
      case ResourceName::Sheep:
        return "Res_Sheep";
      case ResourceName::Fish:
        return "Res_Fish";
      case ResourceName::Clam:
        return "Res_Clam";
      case ResourceName::Crab:
        return "Res_Crab";

      // Luxe
      case ResourceName::Silver:
        return "Res_Silver";
      case ResourceName::Gems:
        return "Res_Gems";
      case ResourceName::Ivory:
        return "Res_Ivory";
      case ResourceName::Fur:
        return "Res_Fur";
      case ResourceName::Dye:
        return "Res_Dye";
      case ResourceName::Incense:
        return "Res_Incense";
      case ResourceName::Silk:
        return "Res_Silk";
      case ResourceName::Sugar:
        return "Res_Sugar";
      case ResourceName::Spices:
        return "Res_Spices";
      case ResourceName::Wine:
        return "Res_Wine";
      case ResourceName::Whale:
        return "Res_Whale";

      default:
        return "Resource";
    }
  }

  // 4. Priorité 4 : Les arbres
  if (get_terrain().feature != TerrainFeature::None) {
    switch (_terrain.feature) {
      case TerrainFeature::Forest:
        return "Forest";
      case TerrainFeature::Jungle:
        return "Jungle";
      default:
        "?";
    }
  }

  // 5. Priorité 4 : Les charactéristiques du terrain
  if (get_terrain().elevation != TerrainElevation::Flat) {
    switch (_terrain.elevation) {
      case TerrainElevation::Hill:
        return "Hill";
      case TerrainElevation::Mountain:
        return "Mountain";
      default:
        "?";
    }
  }

  // 6. Par défaut : Le type de terrain
  switch (_terrain.type) {
    case TerrainsType::Coast:
      return "Coast";
    case TerrainsType::Desert:
      return "Desert";
    case TerrainsType::Grassland:
      return "Grassland";
    case TerrainsType::Ice:
      return "Ice";
    case TerrainsType::Ocean:
      return "Ocean";
    case TerrainsType::Plains:
      return "Plains";
    case TerrainsType::Tundra:
      return "Tundra";
    default:
      return "?";
  }
}
