#include "case.h"

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
};

void Case::set_country_neutral() {
  if (_units.size() == 0) {
    _country = Country::Neutral;
  }
}

/**
 * @brief Vérifie si un déplacement vers une case cible est possible et calcule
 * le chemin.
 * @param target_case La case de destination visée.
 * @param unit L'unité qui effectue le mouvement.
 * @return course Structure contenant la validité du trajet et la liste des
 * cases traversées.
 */
Course Case::movement_is_possible(const Case* target_case,
                                  const Unit* unit) const {
  if (this == target_case) {
    return {true, {const_cast<Case*>(this)}};
  }

  std::vector<const Case*> visited;
  return movement_is_possible_rec(target_case, unit, unit->get_speed(),
                                  visited);
}

/**
 * @brief Vérifie récursivement si un déplacement vers une case cible est
 * possible.
 * * @param target_case La case de destination visée.
 * @param unit L'unité qui effectue le mouvement.
 * @param speed Les points de mouvement restants.
 * @param visited Liste des cases déjà explorées pour éviter les boucles
 * infinies.
 * @return course Structure contenant la validité du chemin et la liste des
 * cases traversées.
 */
Course Case::movement_is_possible_rec(const Case* target_case, const Unit* unit,
                                      int speed,
                                      std::vector<const Case*>& visited) const {
  if (speed < 0) {
    return {false, {}};
  }

  if (this == target_case) {
    return {true, {const_cast<Case*>(this)}};
  }

  visited.push_back(this);

  for (const Case* neighbor : _neighbors) {
    // Vérification terrain
    if (!unit->find_terrain(neighbor->_terrains.get_terrain_type())) {
      continue;
    }

    // Éviter de repasser sur une case déjà vue
    if (std::find(visited.begin(), visited.end(), neighbor) != visited.end()) {
      continue;
    }

    // Appel récursif
    Course result = neighbor->movement_is_possible_rec(target_case, unit,
                                                       speed - 1, visited);

    // Si un chemin a été trouvé par ce voisin
    if (result.is_possible) {
      result.distance_traveled.insert(result.distance_traveled.begin(),
                                      const_cast<Case*>(this));
      return result;
    }
  }

  return {false, {}};
}

void Case::movement(Case* target_case, Unit* unit_to_move) {
  Course course = movement_is_possible(target_case, unit_to_move);
  if (!course.is_possible) return;
  // Si la case n'est pas occupée ou que les unités dessus font partie du même
  // pays
  if ((target_case->get_country() == Country::Neutral) ||
      (target_case->get_country() == unit_to_move->get_country())) {
    delete_unit(unit_to_move);
    set_country_neutral();
    target_case->add_unit(unit_to_move);
  } else {
    // Un ennemi est présent sur la case cible, on lance le combat
    // On demande à la case cible de choisir son meilleur défenseur contre notre
    // unité
    Unit* best_defender = target_case->select_best_unit(unit_to_move);
    unit_to_move->attack(best_defender);

    // On vérifie si le défenseur est toujours en vie
    if (best_defender->get_stats().hp > 0) {
      // On vérifie si l'unité qui se déplace est toujours en vie
      if (unit_to_move->get_stats().hp > 0) {
        // On le met une case -1 de son parcours
        delete_unit(unit_to_move);
        set_country_neutral();
        course.distance_traveled[course.distance_traveled.size() - 1]->add_unit(
            unit_to_move);
      }
    } else {
      target_case->delete_unit(best_defender);
      target_case->set_country_neutral();

      // On vérifie si l'unité qui se déplace est toujours en vie
      if (unit_to_move->get_stats().hp > 0) {
        // On vérifie s'il y a toujours des unités sur la case cible
        if (target_case->get_country() == Country::Neutral) {
          // S'il y en a plus, on déplace l'unité dessus
          delete_unit(unit_to_move);
          set_country_neutral();
          target_case->add_unit(unit_to_move);
        } else {
          // Sinon on le met une case -1 de son parcours
          delete_unit(unit_to_move);
          set_country_neutral();
          course.distance_traveled[course.distance_traveled.size() - 1]
              ->add_unit(unit_to_move);
        }
      } else {
        delete_unit(unit_to_move);
        set_country_neutral();
      }
    }
  }
}

void Case::delete_unit(Unit* unit_to_move) {
  // On cherche l'unité par son ID
  auto it = std::find_if(_units.begin(), _units.end(), [&](Unit* u) {
    return u->get_id() == unit_to_move->get_id();
  });

  if (it != _units.end()) {
    _units.erase(it);
  }
}

Country Case::get_unit_country() const {
  if (_units.empty()) {
    return Country::Neutral;
  }

  return _units.front()->get_country();
}

/**
 * @brief Sélectionne l'unité de la case ayant le plus de chances d'anéantir
 * l'ennemi.
 * @param ennemy L'unité adverse cible pour laquelle on cherche le meilleur
 * contre.
 * @return Unit L'objet unité ayant le score d'efficacité le plus élevé.
 */
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

Course Case::distance_between(const Case& target_case) const {
  if (this == &target_case) {
    return {true, {const_cast<Case*>(this)}};
  }

  std::vector<const Case*> visited;
  return _distance_between_rec(target_case, visited);
}

Course Case::_distance_between_rec(const Case& target_case,
                                   std::vector<const Case*>& visited) const {
  if (this == &target_case) {
    return {true, {const_cast<Case*>(this)}};
  }

  visited.push_back(this);

  for (const Case* neighbor : _neighbors) {
    // Éviter de repasser sur une case déjà vue
    if (std::find(visited.begin(), visited.end(), neighbor) != visited.end()) {
      continue;
    }

    // Appel récursif
    Course result = neighbor->_distance_between_rec(target_case, visited);

    // Si un chemin a été trouvé par ce voisin
    if (result.is_possible) {
      result.distance_traveled.insert(result.distance_traveled.begin(),
                                      const_cast<Case*>(this));
      return result;
    }
  }

  // Case non trouvé
  return {false, {}};
}

void Case::create_city(Case* target_case, Unit* unit) {
  // 1. On vérifie si l'unité est un colon
  if (unit->get_name() != UnitName::Settler) {
    return;
  }

  // 2 On vérifie si le colon se trouve sur un terrain neutre ET à plus de 5
  // cases d'une autre ville
  // if (unit->get_case_unit()->get_terrain().)

  // 3. On vérifie sur la case s'il a un batiment
  if (target_case->get_terrain().get_building() != BuildingType::NoBuilding) {
    return;
  }
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
