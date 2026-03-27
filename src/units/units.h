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

  int calculate_damage(const Unit* ennemy) const;
  void attack(Unit* ennemy);
  bool find_terrain(const TerrainsType& target_terrain) const;
  void switch_active() { active = !active; }
  void switch_on_guard() { on_guard = !on_guard; }
  void heal();
  void set_case_unit(Case* c) { case_unit = c; }

  int get_id() const { return id; }
  std::vector<TerrainsType> get_allow_terrain() const { return allow_terrain; }
  Stats get_stats() const { return stats; }
  UnitName get_name() const { return name; }
  Country get_country() const { return country; }
  int get_speed() const { return stats.speed; }
  Case* get_case_unit() { return case_unit; }
  bool is_active() const { return active; }
  bool is_on_guard() const { return on_guard; }

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