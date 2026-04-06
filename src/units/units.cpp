#include "units.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

#include "case.h"

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
      on_guard(false) {}

Unit::~Unit() {
  // 1. On se retire de la case où l'on se trouve
  if (case_unit != nullptr) {
    case_unit->remove_unit(this);
  }

  // 2. On se retire de la liste du joueur
  if (player != nullptr) {
    player->remove_unit(this);
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

std::vector<UnitAction> Unit::get_unit_actions(const Unit* unit) {
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
  if ((name != UnitName::Settler) && (name != UnitName::Worker)) {
    return true;
  }
  return false;
}
