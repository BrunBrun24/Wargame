#include "units.h"

#include <algorithm>
#include <iostream>
#include <queue>
#include <random>
#include <unordered_map>
#include <ctime>

#include "aerial.h"
#include "case.h"
#include "improvement_database.h"
#include "maritime.h"
#include "resource_database.h"
#include "terrestrial.h"

int Unit::_id_counter = 0;

Unit::Unit(UnitName name, Player* player, Case* case_unit,
           std::vector<TerrainsType> allow_terrain)
    : id(_id_counter++),
      name(name),
      player(player),
      case_unit(case_unit),
      stats(UNIT_STATS.at(name)),
      orders({UnitAction::None, 0, nullptr}),
      allow_terrain(allow_terrain),
      active(true),
      on_guard(false) {
  // On associe l'unité au joueur
  player->add_unit(this);
}

Unit::~Unit() {
  // 1. On se retire de la case où l'on se trouve
  if (case_unit != nullptr) {
    case_unit->remove_unit(this);
    case_unit = nullptr;
  }

  // 2. On se retire de la liste du joueur
  if (player != nullptr) {
    player->remove_unit(this);
    player = nullptr;
  }
}

Unit* Unit::create_unit(const UnitName name, Player* player, Case* c) {
  auto it = UNIT_TYPE.find(name);
  if (it == UNIT_TYPE.end()) {
    throw std::runtime_error("Unit name not found in UNIT_TYPE!");
  }

  UnitType type = it->second;
  switch (type) {
    case UnitType::Air:
    case UnitType::Missile:
      return new Aerial(name, player, c);
    case UnitType::Naval:
      return new Maritime(name, player, c);
    default:
      return new Terrestrial(name, player, c);
  }
}

std::vector<UnitName> Unit::get_all_units() {
  std::vector<UnitName> units;
  // On convertit l'entier en UnitName via static_cast
  for (int i = 0; i <= static_cast<int>(UnitName::Explorer); ++i) {
    units.push_back(static_cast<UnitName>(i));
  }
  return units;
}

ImprovementName Unit::action_to_improvement(UnitAction action) {
  switch (action) {
    case UnitAction::BuildRoad:
      return ImprovementName::Road;
    case UnitAction::BuildFarm:
      return ImprovementName::Farm;
    case UnitAction::BuildMine:
      return ImprovementName::Mine;
    case UnitAction::BuildCamp:
      return ImprovementName::Camp;
    case UnitAction::BuildCottage:
      return ImprovementName::Cottage;
    case UnitAction::BuildForestPreserve:
      return ImprovementName::ForestPreserve;
    case UnitAction::BuildPasture:
      return ImprovementName::Pasture;
    case UnitAction::BuildPlantation:
      return ImprovementName::Plantation;
    case UnitAction::BuildQuarry:
      return ImprovementName::Quarry;
    case UnitAction::BuildLumberMill:
      return ImprovementName::LumberMill;
    case UnitAction::BuildFishingBoats:
      return ImprovementName::FishingBoats;
    case UnitAction::BuildWatermill:
      return ImprovementName::Watermill;
    case UnitAction::BuildWell:
      return ImprovementName::Well;
    case UnitAction::BuildWorkshop:
      return ImprovementName::Workshop;
    case UnitAction::BuildWinery:
      return ImprovementName::Winery;
    case UnitAction::BuildWindmill:
      return ImprovementName::Windmill;
    case UnitAction::BuildWhalingBoats:
      return ImprovementName::WhalingBoats;
    case UnitAction::BuildOffshorePlatform:
      return ImprovementName::OffshorePlatform;
    case UnitAction::BuildFort:
      return ImprovementName::Fort;

    default:
      return ImprovementName::None;
  }
}

Course Unit::can_move_to(const Case* target_case) {
  // 1. Cas d'arrêt immédiat : déjà sur la case
  if (this->case_unit == target_case) {
    return {true, {this->case_unit}, 0.0};
  }

  // On utilise les points de mouvement restants (double)
  double max_mp = this->stats.PM;

  // File : <Case à explorer, PM consommés pour y arriver>
  std::queue<std::pair<Case*, double>> queue;
  std::unordered_map<Case*, Case*> parent_map;
  std::unordered_map<Case*, double> min_cost_map;

  queue.push({this->case_unit, 0.0});
  parent_map[this->case_unit] = nullptr;
  min_cost_map[this->case_unit] = 0.0;

  Case* reached_target = nullptr;
  double final_pm_cost = 0.0;

  while (!queue.empty()) {
    auto [current, current_cost] = queue.front();
    queue.pop();

    // Si on a trouvé la cible
    if (current == target_case) {
      // Vérification spécifique pour les unités civiles (pas de case ennemie
      // occupée)
      if (!this->is_military() && !target_case->get_units().empty()) {
        if (target_case->get_units()[0]->get_player() != this->player) {
          continue;
        }
      }
      reached_target = current;
      final_pm_cost = current_cost;
      break;
    } else {
      // Sinon si sur le chemin il y a une unité militaire ennemie
      // On ne peut pas passer par là
      if (!target_case->get_units().empty()) {
        if (target_case->get_units()[0]->get_player() != this->player) {
          continue;
        }
      }
    }

    for (Case* neighbor : current->get_neighbors()) {
      // Vérification du type de terrain autorisé (Terre/Mer/Air)
      if (!this->find_terrain(neighbor->get_terrain().type)) {
        continue;
      }

      // Calcul du coût via la nouvelle méthode de Terrain
      double move_cost = neighbor->get_terrain().calculate_PM();
      double total_cost = current_cost + move_cost;

      // Vérification de la réserve de PM
      if (total_cost > max_mp) {
        continue;
      }

      // Mise à jour du chemin le plus court (Dijkstra simplifié)
      if (parent_map.find(neighbor) == parent_map.end() ||
          total_cost < min_cost_map[neighbor]) {
        parent_map[neighbor] = current;
        min_cost_map[neighbor] = total_cost;
        queue.push({neighbor, total_cost});
      }
    }
  }

  // 2. Reconstruction du chemin si la cible a été atteinte
  if (reached_target) {
    std::vector<Case*> path;
    Case* backtrack = reached_target;

    // On remonte la chaîne des parents
    while (backtrack != nullptr) {
      path.insert(path.begin(), backtrack);
      backtrack = parent_map[backtrack];
    }

    // On retourne l'objet Course avec le coût PM calculé
    return {true, path, final_pm_cost};
  }

  return {false, {}, 0.0};
}

void Unit::execute_movement(Course course) {
  Case* target_case = course.distance_traveled.back();

  // Case de repli (l'avant-dernière case du chemin parcouru)
  Case* backtrack_case =
      course.distance_traveled[course.distance_traveled.size() - 2];
  Case* current_case = this->case_unit;

  // Cas 1 : La case est libre ou occupée par un allié
  if (target_case->get_country() == Country::Neutral ||
      target_case->get_country() == this->player->get_country()) {
    current_case->remove_unit(this);
    target_case->add_unit(this);

    // On enlève les PM du parcours de l'unité
    this->set_PM(course.PM);
    return;
  }

  // Cas 2 : Un ennemi est présent
  Unit* best_defender = target_case->select_best_unit(this);

  // Si le défenseur n'est pas militaire, capture immédiate
  if (!best_defender->is_military()) {
    current_case->remove_unit(this);
    this->_handle_capture_and_move(target_case);
  } else {
    // Sinon, déclenchement du combat
    this->fight(best_defender);

    // Si le défenseur succombe
    if (best_defender->get_stats().hp <= 0) {
      delete best_defender;

      // On retire l'attaquant de sa case actuelle
      current_case->remove_unit(this);

      // On vérifie si la case est libérée après le combat
      if (target_case->get_units().empty()) {
        target_case->add_unit(this);
      } else {
        Unit* next_defender = target_case->select_best_unit(this);

        // Si le défenseur suivant est civil, on capture
        if (!next_defender->is_military()) {
          this->_handle_capture_and_move(target_case);
        } else {
          // Sinon, on reste sur la case de repli
          backtrack_case->add_unit(this);
        }
      }
      this->switch_active();
    } else {
      // L'unité est détruite
      current_case->remove_unit(this);
      delete this;
    }
  }
}

void Unit::go_to_move(Case* target_case) {
  Course course = can_move_to(target_case);
  // On vérifie si le chemin est toujours atteignable
  if (course.is_possible) {
    // On vérifie si l'unité peut y aller en un seul tour
    double unit_PM = this->stats.PM;
    if (course.distance_traveled.size() < unit_PM) {
      // Si oui alors on déplace l'unité
      execute_movement(course);
    } else {
      // Sinon on le déplace sur la case la plus loin possible selon ses PMs
      // On construit le chemin juqu'où l'unité peut aller
      Course first_movement = {true, {}, 0};
      for (int c = 0; c < unit_PM; c++) {
        first_movement.distance_traveled.push_back(
            course.distance_traveled.at(c));
        first_movement.PM = course.PM;
      }

      // On le déplace
      execute_movement(first_movement);

      // On ajoute la case cible dans les ordres à effectuer
      this->orders = {UnitAction::GoToMove, 0, course.distance_traveled.back()};
    }
  } else {
    // Si le chemin n'est plus atteignable alors on enlève les ordres
    this->orders = {};
  }
}

double Unit::get_modified_strength_vs(const Unit* opponent,
                                      bool is_attacking) const {
  // 1. Calcul de l'état de santé (Ratio HP actuel / HP Max)
  double hp_ratio =
      static_cast<double>(this->stats.hp) / UNIT_STATS.at(this->get_name()).hp;

  // 2. Initialisation des bonus
  double total_bonus = 0.0;
  PowerBonus data = UNIT_POWER_BONUS.at(this->get_name());

  // 3. Bonus du terrain
  for (const auto& bt : data.terrain) {
    if (this->get_case_unit()->get_terrain().elevation == bt.type) {
      total_bonus += bt.attack;
    }
  }

  // 4. Bonus contre le type d'unité adverse
  UnitType opponent_type = UNIT_TYPE.at(opponent->get_name());
  for (const auto& b_type : data.units_types) {
    if (b_type.type == opponent_type) {
      total_bonus += b_type.attack;
    }
  }

  // 5. Bonus contre l'unité adverse spécifique
  for (const auto& b_name : data.units_names) {
    if (b_name.name == opponent->get_name()) {
      total_bonus += b_name.attack;
    }
  }

  // 6. Bonus de Ville
  if (opponent->get_case_unit()->has_city() && is_attacking) {
    total_bonus += data.city.attack;
  } else if (this->get_case_unit()->has_city() && !is_attacking) {
    total_bonus += data.city.defense;
  }

  // 7. Calcul final : Force * (1 + Somme des Bonus) * Ratio HP
  return this->get_stats().power * (1.0 + total_bonus) * hp_ratio;
}

bool Unit::predict_victory_chance(const Unit* defender) const {
  // 1. Récupération des forces de l'attaquant et du défenseur
  double attacker_strength = this->get_modified_attack_strength(defender);
  double defender_strength = defender->get_modified_defense_strength(this);

  // 2. On calcule la probabilité pour que l'attaquant gagne
  double win_probability =
      attacker_strength / (attacker_strength + defender_strength);

  // 3. Génération d'un nombre aléatoire entre 0.0 et 1.0
  static std::mt19937 generator(static_cast<unsigned int>(std::time(nullptr)));
  std::uniform_real_distribution<double> distribution(0.0, 1.0);

  double dice_roll = distribution(generator);

  // 4. Résultat du combat
  if (dice_roll <= win_probability) {
    // L'attaquant a gagné !
    return true;
  } else {
    // Le défenseur a gagné !
    return false;
  }
}

int Unit::calculate_survivor_damage(const Unit* defender) const {
  // 1. Récupération des forces de l'attaquant et du défenseur
  double winner_power = this->get_modified_attack_strength(defender);
  double loser_power = defender->get_modified_defense_strength(this);

  // 2. On calcule le ratio
  double ratio = loser_power / winner_power;

  // 3. Dégâts de base (par exemple 20 PV maximum pour un combat équilibré)
  const int BASE_DAMAGE = 20;

  // 4. Calcul final : Dégâts = Base * Ratio
  int damage = static_cast<int>(std::round(BASE_DAMAGE * ratio));

  // Le vainqueur perd au moins 1 PV
  return std::max(1, damage);
}

void Unit::fight(Unit* defender) {
  if (predict_victory_chance(defender)) {
    // L'attaquant à gagné
    defender->stats.hp = 0;
    this->stats.hp -= calculate_survivor_damage(defender);
  } else {
    // Le défenseur à gagné
    this->stats.hp = 0;
    defender->stats.hp -= calculate_survivor_damage(defender);
  }
}

void Unit::heal() {
  int max_hp = UNIT_STATS.at(name).hp;

  // Si l'unité est en garde et blessée
  if (this->is_on_guard() && this->stats.hp < max_hp) {
    int amount_to_heal = 0;
    // On regarde à quel joueur appartient la case où se trouve l'unité
    const Player* player = this->get_case_unit()->get_player();

    if (player == this->get_player()) {
      // Territoire allié : 30%
      int amount_to_heal = static_cast<int>(max_hp * 0.3);
    } else if (player == nullptr) {
      // Territoire neutre : 20%
      int amount_to_heal = static_cast<int>(max_hp * 0.2);
    } else {
      // Territoire ennemi : 10%
      int amount_to_heal = static_cast<int>(max_hp * 0.1);
    }

    this->stats.hp = std::min<int>(this->stats.hp + amount_to_heal, max_hp);
  }
}

bool Unit::find_terrain(const TerrainsType& target_terrain) const {
  auto it =
      std::find(allow_terrain.begin(), allow_terrain.end(), target_terrain);

  return it != allow_terrain.end();
}

std::vector<UnitAction> Unit::get_unit_actions() {
  std::vector<UnitAction> available_actions;
  available_actions.push_back(UnitAction::SkipTurn);
  available_actions.push_back(UnitAction::Delete);
  available_actions.push_back(UnitAction::GoToMove);
  available_actions.push_back(UnitAction::RegroupUnit);
  available_actions.push_back(UnitAction::RegroupSameUnit);

  if (this->on_guard) {
    available_actions.push_back(UnitAction::Wake);
  }

  return available_actions;
}

void Unit::get_order(UnitAction action, Case* target_case) {
  switch (action) {
    case UnitAction::SkipTurn:
      this->switch_active();
      break;

    case UnitAction::Sleep:
    case UnitAction::Wake:
    case UnitAction::Fortify:
    case UnitAction::UnFortify:
      this->switch_on_guard();
      this->switch_active();
      break;

    case UnitAction::Delete:
      if (this->player) {
        this->get_player()->remove_unit(this);
        delete this;
      }
      break;

    case UnitAction::GoToMove:
      go_to_move(target_case);
      break;

    case UnitAction::RegroupUnit:
      break;

    case UnitAction::RegroupSameUnit:
      break;

    case UnitAction::Pillage:
      pillage();
      break;

    case UnitAction::Bombard:
      break;

    case UnitAction::BuildRoad:
    case UnitAction::BuildFarm:
    case UnitAction::BuildMine:
    case UnitAction::BuildCamp:
    case UnitAction::BuildCottage:
    case UnitAction::BuildForestPreserve:
    case UnitAction::BuildPasture:
    case UnitAction::BuildPlantation:
    case UnitAction::BuildQuarry:
    case UnitAction::BuildLumberMill:
    case UnitAction::ChopDownForest:
    case UnitAction::BuildFishingBoats:
    case UnitAction::BuildWatermill:
    case UnitAction::BuildWell:
    case UnitAction::BuildWorkshop:
    case UnitAction::BuildWinery:
    case UnitAction::BuildWindmill:
    case UnitAction::BuildWhalingBoats:
    case UnitAction::BuildOffshorePlatform:
    case UnitAction::BuildFort: {
      // On récupère le temps de construction
      ImprovementName improvement_name = action_to_improvement(action);
      int turns = 0;

      if (improvement_name != ImprovementName::None) {
        turns = ImprovementDatabase::get_info(improvement_name).turns;
      } else {
        // Couper une forêt / une jungle
        turns = 3;
      }

      // On ajoute la furur action à effectuer
      this->orders = {action, turns, nullptr};
      break;
    }

    default:
      std::cout << "[DEBUG] L'action " << static_cast<int>(action)
                << " n'est pas encore implémentée pour " << id << std::endl;
      break;
  }
}

void Unit::execute_action(UnitAction action) {
  switch (action) {
    case UnitAction::BuildRoad:
      break;

    case UnitAction::BuildCity:
      this->found_city();
      break;

    case UnitAction::BuildFarm:
      this->build_improvement(ImprovementName::Farm);
      break;

    case UnitAction::BuildMine:
      this->build_improvement(ImprovementName::Mine);
      break;

    case UnitAction::BuildCamp:
      this->build_improvement(ImprovementName::Camp);
      break;

    case UnitAction::BuildCottage:
      this->build_improvement(ImprovementName::Cottage);
      break;

    case UnitAction::BuildForestPreserve:
      this->build_improvement(ImprovementName::ForestPreserve);
      break;

    case UnitAction::BuildPasture:
      this->build_improvement(ImprovementName::Pasture);
      break;

    case UnitAction::BuildPlantation:
      this->build_improvement(ImprovementName::Plantation);
      break;

    case UnitAction::BuildQuarry:
      this->build_improvement(ImprovementName::Quarry);
      break;

    case UnitAction::BuildLumberMill:
      this->build_improvement(ImprovementName::LumberMill);
      break;

    case UnitAction::ChopDownForest:
      this->chop_down_forest();
      break;

    case UnitAction::BuildFishingBoats:
      this->build_improvement(ImprovementName::FishingBoats);
      break;

    case UnitAction::BuildWatermill:
      this->build_improvement(ImprovementName::Watermill);
      break;

    case UnitAction::BuildWell:
      this->build_improvement(ImprovementName::Well);
      break;

    case UnitAction::BuildWorkshop:
      this->build_improvement(ImprovementName::Workshop);
      break;

    case UnitAction::BuildWinery:
      this->build_improvement(ImprovementName::Winery);
      break;

    case UnitAction::BuildWindmill:
      this->build_improvement(ImprovementName::Windmill);
      break;

    case UnitAction::BuildWhalingBoats:
      this->build_improvement(ImprovementName::WhalingBoats);
      break;

    case UnitAction::BuildOffshorePlatform:
      this->build_improvement(ImprovementName::OffshorePlatform);
      break;

    case UnitAction::BuildFort:
      this->build_improvement(ImprovementName::Fort);
      break;

    default:
      std::cout << "[DEBUG] L'action " << static_cast<int>(action)
                << " n'est pas encore implémentée pour " << id << std::endl;
      break;
  }
}

void Unit::execute_orders() {
  // Si l'unité a des ordres à effectuer
  if (this->orders.action != UnitAction::None) {
    UnitAction action = this->orders.action;
    switch (action) {
      // 1. C'est une construction d'un aménagement
      case UnitAction::BuildRoad:
      case UnitAction::BuildFarm:
      case UnitAction::BuildMine:
      case UnitAction::BuildCamp:
      case UnitAction::BuildCottage:
      case UnitAction::BuildForestPreserve:
      case UnitAction::BuildPasture:
      case UnitAction::BuildPlantation:
      case UnitAction::BuildQuarry:
      case UnitAction::BuildLumberMill:
      case UnitAction::ChopDownForest:
      case UnitAction::BuildFishingBoats:
      case UnitAction::BuildWatermill:
      case UnitAction::BuildWell:
      case UnitAction::BuildWorkshop:
      case UnitAction::BuildWinery:
      case UnitAction::BuildWindmill:
      case UnitAction::BuildWhalingBoats:
      case UnitAction::BuildOffshorePlatform:
      case UnitAction::BuildFort:
        // On retire l'un des tours de constuctions
        this->orders.turns--;
        // S'il n'y a plus d'odre alors la construction est terminé
        if (this->orders.turns == 0) {
          execute_action(action);
          this->orders = {};
        }
        break;

      case UnitAction::GoToMove:
        go_to_move(this->orders.target);
        break;
    }
  }
}

bool Unit::pillage_is_possible() const {
  // 1. L'unité est une troupe
  if (!is_military()) {
    return false;
  }

  // 2. Un aménagement est sur la case de l'unité ET l'aménagement doit-être
  // différents du pays de l'unité
  if ((case_unit->get_terrain().improvement == ImprovementName::None) &&
      (case_unit->get_country() != player->get_country())) {
    return false;
  }

  return true;
}

void Unit::pillage() {
  // 1. On récupère l'aménagement
  ImprovementName& improvement = case_unit->get_terrain().improvement;

  // 2. Le joueur récupère l'argent du pillage
  this->player->get_income().gold +=
      ImprovementDatabase::get_info(improvement).pillage;

  // 3. On dissocie l'aménagement du joueur
  case_unit->get_player()->remove_improvement(case_unit);

  // 4. On détruit l'aménagement
  improvement = ImprovementName::None;

  // 4. On enlève les PMs à l'unité
  set_PM(1);
}

bool Unit::can_build_improvement(ImprovementName name) {
  // 1. Vérification du type d'unité
  if (this->name != UnitName::Worker || this->name != UnitName::WorkBoat) {
    return false;
  }

  // 2. On regarde s'il y a une ressource sur la case-
  Terrain terrain = this->case_unit->get_terrain();
  if (terrain.resource != ResourceName::None) {
    const Bonus& bonus = ResourceDatabase::get_info(terrain.resource);

    if (name == bonus.improvement_bonus.improvement) return true;
  }

  // 3. On regarde si le terrain est compatible avec l'aménagement
  ImprovementData data = ImprovementDatabase::get_info(name);

  // On commence par regarder les feature
  for (TerrainFeature& feature : data.terrain_feature) {
    if (feature == terrain.feature) return true;
  }

  // Ensuite l'élévation
  for (TerrainElevation& elevation : data.terrain_elevation) {
    if (elevation == terrain.elevation) return true;
  }

  // Puis le type
  for (TerrainsType& type : data.terrain_type) {
    if (type == terrain.type) return true;
  }

  return false;
}

void Unit::build_improvement(ImprovementName name) {
  this->get_case_unit()->get_terrain().improvement = name;

  this->switch_active();
}

bool Unit::is_military() const {
  if ((name != UnitName::Settler) && (name != UnitName::Worker) &&
      (name != UnitName::WorkBoat)) {
    return true;
  }
  return false;
}

void Unit::_handle_capture_and_move(Case* target_case) {
  // 1. Capture des unités présentes sur la case cible
  // On crée une copie du vecteur pour éviter de modifier la structure pendant
  // l'itération
  std::vector<Unit*> units_to_capture = target_case->get_units();

  for (Unit* u : units_to_capture) {
    // Retrait de l'ancien propriétaire
    if (u->get_player() != nullptr) {
      u->get_player()->remove_unit(u);
    }

    // On change l'appartenance de l'unité
    u->set_player(this->player);
    this->get_player()->add_unit(u);

    // On marque l'unité capturée comme ayant déjà joué ce tour
    if (u->is_active()) {
      u->switch_active();
    }
  }

  // 2. L'unité se déplace sur la case cible
  // On retire l'unité de son ancienne case
  if (this->case_unit != nullptr) {
    this->case_unit->remove_unit(this);
  }

  // On ajoute l'unité à la nouvelle case
  target_case->add_unit(this);

  // On met à jour le pointeur interne de l'unité vers sa nouvelle case
  this->set_unit_case(target_case);
}
