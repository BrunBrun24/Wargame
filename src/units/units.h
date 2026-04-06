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

  /** @brief Factory pour instancier le bon type d'unité (Aérienne, Terrestre,
   * etc). */
  static Unit* create_unit(const UnitName name, Player* player, Case* c);
  static std::vector<UnitName> get_all_units();
  static UnitName string_to_unit_name(const std::string& name);
  static StrengthWeaknessMatrix load_strength_weakness_matrix();

  /** @brief Détermine si l'on peut atteindre la case en partant de l'unité */
  Course can_move_to(const Case* target_case);

  /** @brief Déplace l'unité sur la case */
  void execute_movement(Case* target_case);

  /** @brief Calcule les dégâts infligés à un ennemi */
  int calculate_damage(const Unit* ennemy) const;

  /** @brief Combat entre deux unités. */
  void fight(Unit* ennemy);

  /** @brief Soigne l'unité de 20% de ses PV maximum */
  void heal();

  /** @brief Vérifie si l'unité peut se déplacer sur un type de terrain
   * spécifique. */
  bool find_terrain(const TerrainsType& target_terrain) const;

  /** @brief Retourne une liste d'actions possibles que l'unité peut effectuer
   */
  virtual std::vector<UnitAction> get_unit_actions();

  /**
   * @brief Exécute une action spécifique demandée par l'utilisateur.
   * @param action L'action UnitAction à effectuer.
   */
  void execute_action(const UnitAction action);

  virtual void found_city();
  virtual void chop_down_forest();
  bool pillage_is_possible() const;
  void pillage();
  bool can_build_improvement(ImprovementName name);
  void build_improvement(ImprovementName name);
  bool is_military() const;
  UnitStats get_stats() const { return stats; }
  std::vector<TerrainsType> get_allow_terrain() const { return allow_terrain; }

  int get_movement() const { return stats.movement; }
  void set_PM(const double n) {
    this->stats.PM = std::max(0.0, this->stats.PM - n);
    if (this->stats.PM == 0) {
      this->switch_active();
    }
  }

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

  /** @brief On capture les unités civiles et on déplace l'unité qui les a
   * capturées */
  void _handle_capture_and_move(Case* target_case);
};