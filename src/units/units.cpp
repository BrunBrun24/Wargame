#include "units.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>

#include "aerial.h"
#include "case.h"
#include "maritime.h"
#include "terrestrial.h"

int Unit::_id_counter = 0;

Unit::Unit(UnitName name, Player* player, Case* case_unit,
           std::vector<TerrainsType> allow_terrain)
    : id(_id_counter++),
      name(name),
      player(player),
      case_unit(case_unit),
      stats(UNIT_STATS.at(name)),
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

UnitName Unit::string_to_unit_name(const std::string& name) {
  auto it = UNIT_STRING_NAME.find(name);
  if (it != UNIT_STRING_NAME.end()) {
    return it->second;
  }
  std::string error_msg =
      "ERREUR FATALE : L'unite '" + name +
      "' est introuvable dans le dictionnaire de correspondance. " +
      "Verifiez l'orthographe dans le fichier CSV.";

  throw std::runtime_error(error_msg);
}

StrengthWeaknessMatrix Unit::load_strength_weakness_matrix() {
  StrengthWeaknessMatrix matrix;
  std::ifstream file("../../src/files/csv/matrix_units_strength_weakness.csv");

  if (!file.is_open()) {
    std::cerr << "Impossible d'ouvrir le fichier CSV." << std::endl;
    return matrix;
  }

  std::string line;
  int line_count = 1;        // Pour le débug
  std::getline(file, line);  // Ignore l'en-tête

  // L'ordre des colonnes doit correspondre au vecteur statique
  // _unit_type_order On suppose que l'ordre du CSV est le même que UnitName
  std::vector<UnitName> all_units = get_all_units();

  try {
    while (std::getline(file, line)) {
      std::stringstream ss(line);
      std::string attacker_str;
      std::getline(ss, attacker_str,
                   ',');  // Récupérer le nom de l'attaquant (1ère colonne)

      // Convertir le string en UnitName
      UnitName attacker = string_to_unit_name(attacker_str);

      std::string value_str;
      int col_index = 0;
      while (std::getline(ss, value_str, ',')) {
        if (!value_str.empty()) {
          double val = std::stod(value_str);
          UnitName target = all_units[col_index];
          matrix[attacker][target] = val;
          col_index++;
        }
      }
      line_count++;
    }
    return matrix;
  } catch (const std::exception& e) {
    std::cerr << "\n========================================" << std::endl;
    std::cerr << "ERREUR DE CHARGEMENT (Ligne " << line_count << ")"
              << std::endl;
    std::cerr << e.what() << std::endl;
    std::cerr << "========================================\n" << std::endl;
    exit(1);
  }
}

Course Unit::can_move_to(const Case* target_case) {
  // Si on est déjà sur la case cible
  if (this->case_unit == target_case) {
    return {true, {this->case_unit}};
  }

  // On utilise les points de mouvement restants de l'unité
  int max_mp = this->stats.movement;

  // File : <Case à explorer, Points de mouvement consommés pour y arriver>
  std::queue<std::pair<Case*, int>> queue;
  std::unordered_map<Case*, Case*> parent_map;
  std::unordered_map<Case*, int> min_cost_map;

  queue.push({this->case_unit, 0});
  parent_map[this->case_unit] = nullptr;
  min_cost_map[this->case_unit] = 0;

  Case* reached_target = nullptr;

  while (!queue.empty()) {
    auto [current, current_cost] = queue.front();
    queue.pop();

    if (current == target_case) {
      // Une unité civile ne peut pas entrer sur une case ennemie occupée
      if (!this->is_military() && !target_case->get_units().empty()) {
        if (target_case->get_units()[0]->get_player() != this->player) {
          continue;
        }
      }
      reached_target = current;
      break;
    }

    for (Case* neighbor : current->get_neighbors()) {
      // 1. Vérification si le terrain est autorisé pour ce type d'unité
      if (!this->find_terrain(neighbor->get_terrain().type)) {
        continue;
      }

      // 2. Calcul du coût pour entrer sur cette case
      int move_cost = neighbor->get_terrain().calculate_move_cost();
      int total_cost = current_cost + move_cost;

      // 3. Si on dépasse les MP de l'unité, on ne peut pas aller sur ce voisin
      if (total_cost > max_mp) {
        continue;
      }

      // 4. Si la case n'a pas été visitée OU si on a trouvé un chemin moins
      // coûteux
      if (parent_map.find(neighbor) == parent_map.end() ||
          total_cost < min_cost_map[neighbor]) {
        parent_map[neighbor] = current;
        min_cost_map[neighbor] = total_cost;
        queue.push({neighbor, total_cost});
      }
    }
  }

  // Reconstruction du chemin
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

void Unit::execute_movement(Case* target_case) {
  // On regarde si le mouvement est possible
  Course course = this->can_move_to(target_case);
  if (!course.is_possible || course.distance_traveled.size() < 2) {
    return;
  }

  // Case de repli (l'avant-dernière case du chemin parcouru)
  Case* backtrack_case =
      course.distance_traveled[course.distance_traveled.size() - 2];
  Case* current_case = this->case_unit;

  // Cas 1 : La case est libre ou occupée par un allié
  if (target_case->get_country() == Country::Neutral ||
      target_case->get_country() == this->player->get_country()) {
    current_case->remove_unit(this);
    target_case->add_unit(this);
    this->switch_active();
    return;
  }

  // Cas 2 : Un ennemi est présent
  Unit* best_defender = target_case->select_best_unit(this);

  // Si le défenseur n'est pas militaire (civil), capture immédiate
  if (!best_defender->is_military()) {
    current_case->remove_unit(this);
    // La méthode _capture_and_displace doit être accessible ou logique déplacée
    // ici
    this->_handle_capture_and_move(target_case);
  } else {
    // Sinon, déclenchement du combat
    this->fight(best_defender);

    // Si le défenseur succombe
    if (best_defender->get_stats().hp <= 0) {
      delete best_defender;
    }

    // Si l'attaquant est toujours en vie
    if (this->get_stats().hp > 0) {
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
          // Sinon, on reste sur la case de repli (le combat a consommé le tour)
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

int Unit::calculate_damage(const Unit* ennemy) const {
  // Si l'unité est affaiblie, elle fait moins de dégâts
  double percentage_hp_remaining =
      static_cast<double>(stats.hp) / UNIT_STATS.at(this->get_name()).hp;
  if (percentage_hp_remaining < 0.3) {
    percentage_hp_remaining = 0.3;
  }

  StrengthWeaknessMatrix unit_strength_weakness_matrix =
      load_strength_weakness_matrix();

  // Calcul des dégâts
  double strength_weakness =
      unit_strength_weakness_matrix.at(this->get_name()).at(ennemy->get_name());
  double raw_damage =
      stats.power * percentage_hp_remaining * (1.0 + strength_weakness);

  // Si l'unité est en garde alors la puissance de l'attaque prend -30%
  if (ennemy->is_on_guard()) {
    raw_damage *= 0.7;
  }

  return static_cast<int>(raw_damage);
}

void Unit::fight(Unit* ennemy) {
  // On attaque l'ennemie
  ennemy->stats.hp -= calculate_damage(this);
  // L'ennemie riposte
  stats.hp -= calculate_damage(ennemy);
}

bool Unit::can_build_city() const {
  // 1. Vérification du type d'unité
  if (this->name != UnitName::Settler) {
    return false;
  }

  // 2. On délègue la vérification de distance à la case
  return this->case_unit->is_valid_for_city();
}

void Unit::found_city() {
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

void Unit::heal() {
  int max_hp = UNIT_STATS.at(name).hp;

  if (stats.hp < max_hp) {
    int amount_to_heal = static_cast<int>(max_hp * 0.2);

    stats.hp = std::min<int>(stats.hp + amount_to_heal, max_hp);
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

  return available_actions;
}

bool Unit::destroy_improvement_is_possible() const {
  // 1. L'unité est une troupe
  if (!is_military()) {
    return false;
  }

  // 2. Un bâtiment est sur la case de l'unité ET le bâtiment doit-être
  // différents du pays de l'unité
  if ((case_unit->get_terrain().improvement == ImprovementName::None) &&
      (case_unit->get_country() != player->get_country())) {
    return false;
  }

  return true;
}

void Unit::destroy_improvement() {
  // 1. On détruit le bâtiment
  case_unit->get_terrain().improvement = ImprovementName::None;

  // 2. On dissocie le bâtiment du joueur
  case_unit->get_player()->remove_improvement(case_unit);
  case_unit->set_player(nullptr);

  // 3. L'unité passe son tour
  switch_active();
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
