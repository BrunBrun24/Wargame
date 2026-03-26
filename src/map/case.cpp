#include "case.h"

Case::Case() : _terrains(TerrainsType::Plains) {}

Case::Case(TerrainsType type) : _terrains(type) {}

void Case::add_neighbor(Case* neighbor) {
  if (neighbor != nullptr) {
    _neighbors.push_back(neighbor);
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
Course Case::movement_is_possible(const Case& target_case,
                                  const Unit& unit) const {
  if (this == &target_case) {
    return {true, {const_cast<Case*>(this)}};
  }

  std::vector<const Case*> visited;
  return movement_is_possible_rec(target_case, unit, unit.get_speed(), visited);
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
Course Case::movement_is_possible_rec(const Case& target_case, const Unit& unit,
                                      int speed,
                                      std::vector<const Case*>& visited) const {
  if (speed < 0) {
    return {false, {}};
  }

  if (this == &target_case) {
    return {true, {const_cast<Case*>(this)}};
  }

  visited.push_back(this);

  for (const Case* neighbor : _neighbors) {
    // Vérification terrain
    if (!unit.find_terrain(neighbor->_terrains.get_terrain_type())) {
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

void Case::movement(Case& target_case, Unit& unit_to_move) {
  Course course = movement_is_possible(target_case, unit_to_move);
  if (!course.is_possible) return;
  // Si la case n'est pas occupée ou que les unités dessus font partie du même
  // pays
  Country country_in_target_case = get_unit_country();
  if ((country_in_target_case == Country::Neutral) ||
      (country_in_target_case == unit_to_move.get_country())) {
    delete_unit(unit_to_move);
    target_case.add_unit(unit_to_move);
  } else {
    // Un ennemi est présent sur la case cible, on lance le combat
    // On demande à la case cible de choisir son meilleur défenseur contre notre
    // unité
    Unit& best_defender = target_case.select_best_unit(unit_to_move);
    unit_to_move.attack(best_defender);

    // On vérifie si le défenseur est toujours en vie
    if (best_defender.get_stats().hp > 0) {
      // On vérifie si l'unité qui se déplace est toujours en vie
      if (unit_to_move.get_stats().hp > 0) {
        // On le met une case -1 de son parcours
        delete_unit(unit_to_move);
        course.distance_traveled[course.distance_traveled.size() - 1]->add_unit(
            unit_to_move);
      }
    } else {
      target_case.delete_unit(best_defender);

      // On vérifie si l'unité qui se déplace est toujours en vie
      if (unit_to_move.get_stats().hp > 0) {
        // On vérifie s'il y a toujours des unités sur la case cible
        Country country_in_target_case = get_unit_country();
        if (country_in_target_case == Country::Neutral) {
          // S'il y en a plus, on déplace l'unité dessus
          delete_unit(unit_to_move);
          target_case.add_unit(unit_to_move);
        } else {
          // Sinon on le met une case -1 de son parcours
          delete_unit(unit_to_move);
          course.distance_traveled[course.distance_traveled.size() - 1]
              ->add_unit(unit_to_move);
        }
      } else {
        delete_unit(unit_to_move);
      }
    }
  }
}

void Case::delete_unit(Unit& unit_to_move) {
  // On cherche l'unité par son ID
  auto it = std::find_if(_units.begin(), _units.end(), [&](Unit* u) {
    return u->get_id() == unit_to_move.get_id();
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
Unit& Case::select_best_unit(Unit& ennemy) const {
  Unit* best_unit = nullptr;
  int damage = 0;

  for (auto* unit : _units) {
    int potential_damage = unit->calculate_damage(ennemy);
    if (potential_damage > damage) {
      damage = potential_damage;
      best_unit = unit;
    }
  }

  return *best_unit;
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
}
