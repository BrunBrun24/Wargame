#include "case.h"

#include <queue>
#include <unordered_map>

Case::Case(TerrainsType type) : _terrains(type), _country(Country::Neutral) {}

Case::Case(TerrainsType type, Country country)
    : _terrains(type), _country(country) {}

void Case::add_neighbor(Case* neighbor) {
  if (neighbor != nullptr) {
    _neighbors.push_back(neighbor);
  }
}

void Case::add_unit(Unit* unit) {
  _units.push_back(unit);

  set_country(unit->get_country());
  unit->set_case_unit(this);
};

void Case::delete_unit(Unit* unit_to_move) {
  // On cherche l'unité par son ID
  auto it = std::find_if(_units.begin(), _units.end(), [&](Unit* u) {
    return u->get_id() == unit_to_move->get_id();
  });

  if (it != _units.end()) {
    _units.erase(it);
  }
}

bool Case::create_city_is_possible(Case* target_case, Unit* unit) {
  // 1. On vérifie si l'unité est un colon
  if (unit->get_name() != UnitName::Settler) {
    return false;
  }

  // 2 On vérifie si le colon se trouve sur un terrain neutre ET à plus de 5
  // cases d'une autre ville
  if ((unit->get_case_unit()->get_country() != Country::Neutral) &&
      (_calculate_first_building_distance(BuildingType::City)
               .distance_traveled.size() -
           1 <
       5)) {
    return false;
  }

  // 3. On vérifie sur la case s'il a un batiment
  if (target_case->get_terrain().get_building() != BuildingType::NoBuilding) {
    return false;
  }

  return true;
}

void Case::create_city(Case* target_case, Unit* unit) {
  target_case->get_terrain().set_building(BuildingType::City);
  target_case->delete_unit(unit);
}

Course Case::movement_is_possible(Case* target_case, const Unit* unit) {
  if (this == target_case) {
    return {true, {this}};
  }

  int max_speed = unit->get_speed();

  // File de paires : la case à explorer et la distance actuelle depuis le
  // départ
  std::queue<std::pair<Case*, int>> queue;
  // Map pour suivre les parents et la distance minimale pour atteindre chaque
  // case
  std::unordered_map<Case*, Case*> parent_map;

  queue.push({this, 0});
  parent_map[this] = nullptr;

  Case* reached_target = nullptr;

  while (!queue.empty()) {
    auto [current, current_dist] = queue.front();
    queue.pop();

    // Si on a trouvé la cible, on s'arrête
    if (current == target_case) {
      reached_target = current;
      break;
    }

    // Si on a atteint la limite de vitesse, on n'explore pas les voisins de
    // cette case
    if (current_dist >= max_speed) {
      continue;
    }

    for (Case* neighbor : current->_neighbors) {
      // Vérification du terrain (Spécifique aux unités)
      if (!unit->find_terrain(neighbor->_terrains.get_terrain_type())) {
        continue;
      }

      // Vérification s'il y a une unité ennemie sur le terrain
      if (!unit->find_terrain(neighbor->_terrains.get_terrain_type())) {
        continue;
      }

      // Vérification si déjà visité
      if (parent_map.find(neighbor) == parent_map.end()) {
        parent_map[neighbor] = current;
        queue.push({neighbor, current_dist + 1});
      }
    }
  }

  // Reconstruction du chemin si la cible a été atteinte
  if (reached_target) {
    std::vector<Case*> path;
    Case* backtrack = reached_target;
    while (backtrack != nullptr) {
      path.insert(path.begin(), backtrack);
      backtrack = parent_map[backtrack];
    }
    return {true, path};
  }

  return {false, {}};
}

void Case::movement(Case* target_case, Unit* unit_to_move) {
  Course course = movement_is_possible(target_case, unit_to_move);
  if (!course.is_possible || course.distance_traveled.size() < 2) return;

  // Case de repli en cas de défaite ou de case cible encore occupée
  Case* backtrack_case =
      course.distance_traveled[course.distance_traveled.size() - 2];

  // Cas 1 : La case est libre ou alliée
  if ((target_case->get_country() == Country::Neutral) ||
      (target_case->get_country() == unit_to_move->get_country())) {
    delete_unit(unit_to_move);
    set_country_neutral();

    target_case->add_unit(unit_to_move);
    unit_to_move->set_case_unit(target_case);
    return;
  }

  // Cas 2 : Un ennemi est présent
  Unit* best_defender = target_case->select_best_unit(unit_to_move);
  unit_to_move->attack(best_defender);

  // Si le défenseur meurt
  if (!(best_defender->get_stats().hp > 0)) {
    target_case->delete_unit(best_defender);
    target_case->set_country_neutral();
  }

  // Gestion de l'attaquant après combat
  if (unit_to_move->get_stats().hp > 0) {
    delete_unit(unit_to_move);
    set_country_neutral();

    // Si la case cible est maintenant vide, on avance
    if (target_case->get_country() == Country::Neutral) {
      target_case->add_unit(unit_to_move);
      unit_to_move->set_case_unit(target_case);
    } else {
      // Sinon on recule sur la case précédente
      backtrack_case->add_unit(unit_to_move);
      unit_to_move->set_case_unit(backtrack_case);
    }
  } else {
    // L'attaquant est mort
    delete_unit(unit_to_move);
    set_country_neutral();
  }
}

Unit* Case::select_best_unit(Unit* ennemy) const {
  Unit* best_unit = nullptr;
  int damage = 0;

  for (auto* unit : _units) {
    int potential_damage = unit->calculate_damage(ennemy);
    if (potential_damage > damage) {
      damage = potential_damage;
      best_unit = unit;
    }
  }

  return best_unit;
}

Course Case::distance_between(Case* target_case) {
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

Course Case::_calculate_first_building_distance(BuildingType type) {
  // La file contient les cases à visiter
  std::queue<Case*> queue;
  // Permet de savoir si une case a été vue ET de stocker d'où on vient (parent)
  // pour reconstruire le chemin à la fin.
  std::unordered_map<Case*, Case*> parent_map;

  queue.push(this);
  parent_map[this] = nullptr;  // La case de départ n'a pas de parent

  Case* target = nullptr;

  while (!queue.empty()) {
    Case* current = queue.front();
    queue.pop();

    // 1. Condition d'arrêt : on a trouvé le bâtiment
    if (current->get_terrain().get_building() == type) {
      target = current;
      break;
    }

    // 2. Exploration des voisins
    for (Case* neighbor : current->_neighbors) {
      // Si le voisin n'a pas encore été visité
      if (parent_map.find(neighbor) == parent_map.end()) {
        parent_map[neighbor] = current;
        queue.push(neighbor);
      }
    }
  }

  // 3. Si on n'a rien trouvé
  if (target == nullptr) {
    return {false, {}};
  }

  // 4. Reconstruction du chemin en remontant les parents
  std::vector<Case*> path;
  Case* backtrack = target;
  while (backtrack != nullptr) {
    path.insert(path.begin(), backtrack);
    backtrack = parent_map[backtrack];
  }

  return {true, path};
}

void Case::set_country_neutral() {
  if (_units.size() == 0) {
    _country = Country::Neutral;
  }
}

Country Case::get_unit_country() const {
  if (_units.empty()) {
    return Country::Neutral;
  }

  return _units.front()->get_country();
}

std::string Case::get_description() {
  // 1. Priorité maximale : La Ville
  if (_terrains.get_building() == BuildingType::City) {
    return "City";
  }

  // 2. Si pas de ville : Nom de l'unité (la première sur la case)
  if (!_units.empty()) {
    UnitName name = _units[0]->get_name();

    // Conversion de l'enum UnitName en string (basé sur ton dictionnaire
    // UnitParser)
    switch (name) {
      case UnitName::Warrior:
        return "Warrior";
      case UnitName::Settler:
        return "Settler";
      case UnitName::Tank:
        return "Tank";
      case UnitName::Swordsman:
        return "Swordsman";
      case UnitName::Archer:
        return "Archer";
      case UnitName::Infantry:
        return "Infantry";
      case UnitName::Worker:
        return "Worker";
      case UnitName::MechanizedInfantry:
        return "MechanizedInfantry";
      case UnitName::Crossbowman:
        return "Crossbowman";
      case UnitName::FieldCannon:
        return "FieldCannon";
      case UnitName::MachineGun:
        return "MachineGun";
      case UnitName::Horseman:
        return "Horseman";
      case UnitName::Knight:
        return "Knight";
      case UnitName::Cuirassier:
        return "Cuirassier";
      case UnitName::ModernArmor:
        return "ModernArmor";
      case UnitName::Galley:
        return "Galley";
      case UnitName::Caravel:
        return "Caravel";
      case UnitName::Ironclad:
        return "Ironclad";
      case UnitName::Destroyer:
        return "Destroyer";
      case UnitName::Submarine:
        return "Submarine";
      case UnitName::AircraftCarrier:
        return "AircraftCarrier";
      case UnitName::Biplane:
        return "Biplane";
      case UnitName::Fighter:
        return "Fighter";
      case UnitName::JetFighter:
        return "JetFighter";
      case UnitName::Bomber:
        return "Bomber";
      case UnitName::JetBomber:
        return "JetBomber";
      default:
        return "UnknownUnit";
    }
  }

  // 3. Si pas d'unité : Nom du bâtiment
  switch (_terrains.get_building()) {
    case BuildingType::GoldMine:
      return "GoldMine";
    case BuildingType::IronMine:
      return "IronMine";
    case BuildingType::Oil:
      return "Oil";
    default:
      break;
  }

  // 4. Par défaut : Le type de terrain
  switch (_terrains.get_terrain_type()) {
    case TerrainsType::Plains:
      return "Plains";
    case TerrainsType::Ocean:
      return "Ocean";
    case TerrainsType::Mountains:
      return "Mountains";
    case TerrainsType::Snow:
      return "Snow";
    case TerrainsType::CoastLake:
      return "CoastLake";
    default:
      return "UnknownTerrain";
  }
}
