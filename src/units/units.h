#pragma once

#include <iostream>
#include <map>
#include <vector>

#include "player.h"
#include "terrains.h"

enum class UnitName {
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
  Unit(UnitName name, Country country, std::vector<TerrainsType> allow_terrain);

  int calculate_damage(const Unit& ennemy) const;
  void attack(Unit& ennemy);
  bool find_terrain(const TerrainsType& target_terrain) const;
  void switch_travel_possible() { _travel_possible = !_travel_possible; }
  void switch_on_guard() { _on_guard = !_on_guard; }
  void heal();

  int get_id() const { return _id; }
  std::vector<TerrainsType> get_allow_terrain() const { return allow_terrain; }
  Stats get_stats() const { return _stats; }
  UnitName get_name() const { return _name; }
  Country get_country() const { return _country; }
  int get_speed() const { return _stats.speed; }
  bool get_travel_possible() const { return _travel_possible; }
  bool get_on_guard() const { return _on_guard; }

 protected:
  std::vector<TerrainsType> allow_terrain;

 private:
  static int _id_counter;

  int _id;
  UnitName _name;
  Country _country;
  Stats _stats;
  bool _travel_possible;  // par default à true quand l'unité apparaît
  bool _on_guard;         // par default à false quand l'unité apparaît
};