#include "case.h"

#include <algorithm>
#include <queue>
#include <unordered_map>

#include "building_database.h"
#include "city.h"
#include "player.h"
#include "resource_database.h"
#include "units.h"

int Case::_id_counter = 0;

Case::Case(TerrainsType type, Player* player, Country country)
    : _id(_id_counter++),
      _country(country),
      _terrain(type),
      _resource(ResourceName::None),
      _building(BuildingName::None),
      _player(player),
      _city(nullptr) {}

Course Case::movement_is_possible(const Case* target_case, const Unit* unit) {
  if (this == target_case) {
    return {true, {this}};
  }

  int max_speed = unit->get_movement();

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
      // Si l'unité que l'on veut déplacer n'est pas une troupe et que la case
      // cible contient une unité ennemie, elle ne peut pas y accéder
      std::vector<Unit*> target_units = target_case->get_units();
      if ((!unit->is_military()) && (!target_units.empty())) {
        // On vérifie si la première unité trouvée est ennemie
        if (target_units[0]->get_player() != unit->get_player()) {
          break;
        }
      }
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
      if (!unit->find_terrain(neighbor->get_terrain())) {
        continue;
      }

      // Vérification s'il y a une unité ennemie sur le terrain
      if (!unit->find_terrain(neighbor->get_terrain())) {
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

  // Cas 1 : La case est libre ou alliée (le terrain à déjà été vérifié)
  if ((target_case->get_country() == Country::Neutral) ||
      (target_case->get_country() ==
       unit_to_move->get_player()->get_country())) {
    remove_unit(unit_to_move);

    target_case->add_unit(unit_to_move);
    unit_to_move->switch_active();
    return;
  }

  // Cas 2 : Un ennemi est présent
  Unit* best_defender = target_case->select_best_unit(unit_to_move);

  // Si l'ennemie n'est pas une unité militaire, les ennemies sur la case sont
  // capturés ET l'unité se déplace sur la case cible
  if (!best_defender->is_military()) {
    _capture_and_displace(target_case, unit_to_move);

  } else {
    // Sinon un combat a lieu
    unit_to_move->fight(best_defender);

    // Si le défenseur meurt
    if (!(best_defender->get_stats().hp > 0)) {
      delete best_defender;
    }

    // Gestion de l'attaquant après combat
    if (unit_to_move->get_stats().hp > 0) {
      remove_unit(unit_to_move);

      // Si la case cible est maintenant vide, on avance
      if (target_case->get_country() == Country::Neutral) {
        target_case->add_unit(unit_to_move);

      } else {
        Unit* other_best_defender = target_case->select_best_unit(unit_to_move);

        // Si l'ennemie n'est pas une unité militaire, les ennemies sur la case
        // sont capturés ET l'unité se déplace sur la case cible
        if (!other_best_defender->is_military()) {
          _capture_and_displace(target_case, unit_to_move);

        } else {
          // Sinon on recule sur la case précédente
          backtrack_case->add_unit(unit_to_move);
        }
      }

      unit_to_move->switch_active();

    } else {
      // L'attaquant est mort, on retire l'appartenance de l'unité au joueur
      delete unit_to_move;
    }
  }
}

Unit* Case::select_best_unit(const Unit* ennemy) const {
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

Course Case::calculate_first_building_distance(const BuildingName building) {
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
    if (current->get_building() == building) {
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

Course Case::calculate_first_city_distance() {
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

    // 1. Condition d'arrêt : on a trouvé une ville
    if (current->get_city() != nullptr) {
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

Yields Case::get_total_yields() const {
  // 1. Rendement de la case + Rendement du centre-ville (Minimum 2 Food, 1
  // Prod, 1 Commerce)
  Yields total = get_base_yields();
  total.food += 2;
  total.production += 1;
  total.commerce += 1;

  // 2. Bonus des ressources
  if (_resource != ResourceName::None) {
    const ResourceBonus& bonus = ResourceDatabase::get_info(_resource);
    total.food += bonus.food;
    total.production += bonus.production;
    total.commerce += bonus.commerce;
  }

  // 3. Bonus des Bâtiments
  auto bonus = BuildingDatabase::get_info(_building);
  total.food += bonus.food;
  total.production += bonus.production;
  total.commerce += bonus.commerce;

  return total;
}

void Case::add_unit(Unit* unit) {
  _units.push_back(unit);

  set_country(unit->get_player()->get_country());
  unit->set_unit_case(this);
};

void Case::remove_unit(Unit* unit_to_move) {
  // 1. On cherche et on retire l'unité par son ID
  auto it = std::find_if(_units.begin(), _units.end(), [&](Unit* u) {
    return u->get_id() == unit_to_move->get_id();
  });

  if (it != _units.end()) {
    _units.erase(it);
  }

  // 2. On vérifie si la case doit redevenir neutre
  // On vérifie s'il y a toujours des unités sur la case
  if (_units.empty()) {
    _country = Country::Neutral;
  }
}

void Case::add_neighbor(Case* neighbor) {
  if (neighbor != nullptr) {
    _neighbors.push_back(neighbor);
  }
}

bool Case::create_city_is_possible(const Case* target_case, const Unit* unit) {
  // 1. On vérifie si l'unité est un colon
  if (unit->get_name() != UnitName::Settler) {
    return false;
  }

  // 2 On vérifie si le joueur est sur son territoire ou sur un territoire
  // neutre ET à plus de 2 cases d'une autre ville
  if ((unit->get_case_unit()->get_player() != target_case->get_player()) &&
      (calculate_first_city_distance().distance_traveled.size() - 1 < 2)) {
    return false;
  }

  return true;
}

void Case::create_city(Case* target_case, Unit* settler) {
  // 1. S'il y a un bâtiment on le détruit
  target_case->set_building(BuildingName::None);

  // 2. On créer la ville
  City* city = new City(target_case, settler->get_player());

  // 3. On lie la ville au joueur
  target_case->set_player(settler->get_player());
  target_case->get_player()->add_city(city);

  // 4. On détruit le colon
  // Le delete appelle le destructeur ~Unit() qui s'occupe de
  // nettoyer les listes dans Case et Player.
  delete settler;
}

bool Case::create_building_is_possible(const Case* target_case,
                                       const Unit* unit) {
  // 1. On vérifie si l'unité est un ouvrier
  if (unit->get_name() != UnitName::Worker) {
    return false;
  }

  // 2. Renvoyez la liste de constructions selon le type de ressoucre sur la
  // case

  return true;
}

void Case::create_building(Case* target_case, Unit* worker,
                           const BuildingName building) {
  // 1. On transforme la case
  target_case->set_building(building);

  // 2. On lie la ville au joueur
  target_case->set_player(worker->get_player());
  target_case->get_player()->add_building(target_case);

  // 3. L'ouvrier passe son tour
  worker->switch_active();
}

Yields Case::get_base_yields() const {
  switch (_terrain) {
    case TerrainsType::Plains:
      return {1, 1, 0, 0};
    case TerrainsType::CoastLake:
      return {1, 0, 2, 0};
    case TerrainsType::Ocean:
      return {1, 0, 1, 0};
    default:
      return {0, 0, 0, 0};
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
        return str;
      }
    }

    std::string error_msg = "L'unité n'existe pas";
    throw std::runtime_error(error_msg);
  }

  // 3. Priorité 3 : TOUTES les Ressources Naturelles
  ResourceName res = get_resource();
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

  // 4. Par défaut : Le type de terrain
  switch (get_terrain()) {
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

char Case::get_debug_char() const {
  switch (_terrain) {
    case TerrainsType::CoastLake:
      return 'C';
    case TerrainsType::Mountains:
      return 'M';
    case TerrainsType::Ocean:
      return 'O';
    case TerrainsType::Plains:
      return 'P';
    case TerrainsType::Snow:
      return 'S';
    default:
      return '?';
  }
}

void Case::_capture_and_displace(Case* target_case, Unit* unit_to_move) {
  // 1. On change le pays des unités sur la case cible en mettant le pays de
  // l'unité qui se déplace
  for (Unit* u : target_case->get_units()) {
    // On retire l'appartenance de l'unité au joueur
    u->get_player()->remove_unit(u);

    // On change l'appartenance de l'unité
    u->set_player(unit_to_move->get_player());

    // On associe les unités aux nouveaux joueurs
    unit_to_move->get_player()->add_unit(u);
  }

  // 2. L'unité se déplace sur la case cible
  remove_unit(unit_to_move);
  target_case->add_unit(unit_to_move);
}
