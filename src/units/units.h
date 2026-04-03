#pragma once

#include <iostream>
#include <map>
#include <vector>

#include "player.h"
#include "type.h"

class Case;

using StrengthWeaknessMatrix = std::map<UnitName, std::map<UnitName, double>>;

class Unit {
 public:
  Unit(UnitName name, Player* player, Case* case_unit,
       std::vector<TerrainsType> allow_terrain);
  virtual ~Unit();

  static std::vector<UnitName> get_all_units();
  static UnitName string_to_unit_name(const std::string& name);
  static StrengthWeaknessMatrix load_strength_weakness_matrix();

  /** @brief Calcule les dégâts infligés à un ennemi. */
  int calculate_damage(const Unit* ennemy) const;

  /** @brief Combat entre deux unités. */
  void fight(Unit* ennemy);

  /** @brief Soigne l'unité de 20% de ses PV maximum. */
  void heal();

  /** @brief Vérifie si l'unité peut se déplacer sur un type de terrain
   * spécifique. */
  bool find_terrain(const TerrainsType& target_terrain) const;

  bool destroy_building_is_possible() const;
  void destroy_building();

  bool is_military() const;
  UnitStats get_stats() const { return stats; }
  int get_movement() const { return stats.movement; }
  std::vector<TerrainsType> get_allow_terrain() const { return allow_terrain; }

  int get_id() const { return id; }
  UnitName get_name() const { return name; }

  Player* get_player() const { return player; }
  void set_player(Player* p) { player = p; }

  Case* get_case_unit() const { return case_unit; }
  void set_unit_case(Case* c) { case_unit = c; }

  bool is_active() const { return active; }
  void switch_active() { active = !active; }

  bool is_on_guard() const { return on_guard; }
  void switch_on_guard() { on_guard = !on_guard; }

 protected:
  int id;
  UnitName name;
  Player* player;
  Case* case_unit;
  UnitStats stats;
  std::vector<TerrainsType> allow_terrain;
  bool active;    // L'unité en attente d'ordre
  bool on_guard;  // L'unité se protège

 private:
  static int _id_counter;
};