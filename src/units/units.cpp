#pragma once
#include "units.h"

#include <algorithm>
#include <string>

#include "case.h"
#include "unit_utils.h"

const std::map<UnitName, Stats> unitData = {
    // Neutre
    {UnitName::Settler, {1, 0, 1, 0}},
    {UnitName::Worker, {1, 0, 1, 0}},

    // Terrestre
    {UnitName::Warrior, {100, 20, 2, 1}},
    {UnitName::Swordsman, {120, 35, 2, 1}},
    {UnitName::Musketman, {150, 55, 2, 1}},
    {UnitName::Infantry, {180, 75, 3, 1}},
    {UnitName::MechanizedInfantry, {220, 85, 4, 1}},

    // A distance
    {UnitName::Archer, {80, 15, 2, 2}},
    {UnitName::Crossbowman, {100, 30, 2, 2}},
    {UnitName::FieldCannon, {130, 50, 2, 2}},
    {UnitName::MachineGun, {160, 70, 2, 1}},

    // Cavalerie
    {UnitName::Horseman, {110, 36, 4, 1}},
    {UnitName::Knight, {140, 48, 4, 1}},
    {UnitName::Cuirassier, {170, 65, 4, 1}},
    {UnitName::Tank, {250, 80, 4, 1}},
    {UnitName::ModernArmor, {300, 90, 5, 1}},

    // Naval
    {UnitName::Galley, {120, 30, 3, 1}},
    {UnitName::Caravel, {160, 50, 4, 1}},
    {UnitName::Ironclad, {220, 70, 4, 1}},
    {UnitName::Destroyer, {280, 85, 5, 1}},
    {UnitName::Submarine, {150, 75, 4, 3}},
    {UnitName::AircraftCarrier, {350, 65, 3, 1}},

    // Aérienne
    {UnitName::Biplane, {100, 60, 6, 3}},
    {UnitName::Fighter, {140, 80, 8, 4}},
    {UnitName::JetFighter, {180, 95, 10, 5}},
    {UnitName::Bomber, {150, 85, 7, 6}},
    {UnitName::JetBomber, {200, 110, 9, 8}}};

const StrengthWeaknessMatrix unit_strength_weakness_matrix =
    UnitParser::load_strength_weakness_matrix();

int Unit::_id_counter = 0;

Unit::Unit(UnitName name, Player* player, Case* case_unit,
           std::vector<TerrainsType> allow_terrain)
    : name(name),
      player(player),
      case_unit(case_unit),
      stats(unitData.at(name)),
      allow_terrain(allow_terrain),
      active(true),
      on_guard(false) {
  this->id = _id_counter++;
}

bool Unit::destroy_building_is_possible() {
  // 1. L'unité est une troupe
  if ((name == UnitName::Settler) || (name == UnitName::Worker)) {
    return false;
  }

  // 2. Un bâtiment est sur la case de l'unité ET le bâtiment doit-être
  // différents du pays de l'unité
  if ((case_unit->get_building() == BuildingName::None) &&
      (case_unit->get_country() != player->get_country())) {
    return false;
  }

  return true;
}

void Unit::destroy_building() {
  // On détruit le bâtiment
  case_unit->set_building(BuildingName::None);

  // L'unité passe son tour
  switch_active();
}

int Unit::calculate_damage(const Unit* ennemy) const {
  // Si l'unité est affaiblie, elle fait moins de dégâts
  double percentage_hp_remaining =
      static_cast<double>(stats.hp) / unitData.at(this->get_name()).hp;
  if (percentage_hp_remaining < 0.3) {
    percentage_hp_remaining = 0.3;
  }

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
  int max_hp = unitData.at(name).hp;

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

bool Unit::_is_military() const {
  if ((name != UnitName::Settler) && (name != UnitName::Worker)) {
    return true;
  }
  return false;
}