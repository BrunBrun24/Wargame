#pragma once

#include <iostream>
#include <map>
#include <vector>

#include "terrains.h"

enum class Country {
  None,
  France,
  Germany,
  UnitedKingdom,
  Russia,
  Egypt,
  Switzerland,
  Japan,
  Spain,
  Italy,
  UnitedStates
};

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
  int defense;
  int speed;
  int range;
};

extern const std::map<UnitName, Stats> unitData;

using StrengthWeaknessMatrix = std::map<UnitName, std::map<UnitName, double>>;
extern const StrengthWeaknessMatrix unit_strength_weakness_matrix;

class Unit {
 public:
  Unit(UnitName name, Country country, std::vector<TerrainsType> allow_terrain);
  ~Unit();

  int calculate_damage(Unit& ennemy) const;
  void attack(Unit& ennemy);
  bool find_terrain(const TerrainsType& target_terrain) const;

  int get_id() const { return _id; }
  std::vector<TerrainsType> get_allow_terrain() const { return allow_terrain; }
  Stats get_stats() const { return _stats; }
  UnitName get_name() const { return _name; }
  Country get_country() const { return _country; }
  int get_speed() const { return _stats.speed; }

 protected:
  std::vector<TerrainsType> allow_terrain;

 private:
  static int _id_counter;

  int _id;
  UnitName _name;
  Country _country;
  Stats _stats;
};