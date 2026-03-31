#pragma once

#include <iostream>
#include <map>
#include <vector>

#include "player.h"
#include "terrains.h"

class Case;

enum class UnitName {
  // Neutre
  Settler,
  Worker,

  // Terrestre
  Warrior,
  Swordsman,
  Musketman,
  Infantry,
  MechanizedInfantry,

  // A distance
  Archer,
  Crossbowman,
  FieldCannon,
  MachineGun,

  // Cavalerie
  Horseman,
  Knight,
  Cuirassier,
  Tank,
  ModernArmor,

  // Naval
  Galley,
  Caravel,
  Ironclad,
  Destroyer,
  Submarine,
  AircraftCarrier,

  // Aérienne
  Biplane,
  Fighter,
  JetFighter,
  Bomber,
  JetBomber
};

struct Stats {
  int hp;
  int power;
  int speed;
  int range;
};

extern const std::map<UnitName, Stats> unitData;

using StrengthWeaknessMatrix = std::map<UnitName, std::map<UnitName, double>>;
extern const StrengthWeaknessMatrix unit_strength_weakness_matrix;

class Unit {
 public:
  Unit(UnitName name, Country country, Case* case_unit,
       std::vector<TerrainsType> allow_terrain);
  virtual ~Unit() = default;

  bool destroy_building_is_possible();
  void destroy_building();

  /** @brief Calcule les dégâts infligés à un ennemi. */
  int calculate_damage(const Unit* ennemy) const;

  /** @brief Attaque une unité ennemie. */
  void attack(Unit* ennemy);

  /** @brief Soigne l'unité de 20% de ses PV maximum. */
  void heal();

  /** @brief Vérifie si l'unité peut se déplacer sur un type de terrain. */
  bool find_terrain(const TerrainsType& target_terrain) const;

  void switch_active() { active = !active; }
  void switch_on_guard() { on_guard = !on_guard; }
  void set_case_unit(Case* c) { case_unit = c; }

  int get_id() const { return id; }
  int get_speed() const { return stats.speed; }
  bool is_active() const { return active; }
  bool is_on_guard() const { return on_guard; }

  UnitName get_name() const { return name; }
  Country get_country() const { return country; }
  Stats get_stats() const { return stats; }
  Case* get_case_unit() { return case_unit; }
  std::vector<TerrainsType> get_allow_terrain() const { return allow_terrain; }

 protected:
  int id;
  UnitName name;
  Country country;
  Stats stats;
  bool active;    // par default à true quand l'unité apparaît
  bool on_guard;  // par default à false quand l'unité apparaît
  Case* case_unit;
  std::vector<TerrainsType> allow_terrain;

 private:
  static int _id_counter;
};