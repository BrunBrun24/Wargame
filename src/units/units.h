#include <iostream>
#include <vector>

#include "terrains.h"

enum class Country {
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

class Unit {
 public:
  Unit(UnitName name, Country country, int pv, int speed, int power,
       int defense, int range, std::vector<TerrainsType> allow_terrain);
  ~Unit();

  void attack(Unit* ennemy);

  Country get_team() { return _country; }
  int get_pv() { return _pv; }
  int get_power() { return _power; }
  void set_pv(int atq) { _pv -= atq; }
  int get_def() { return _defense; }

 protected:
  std::vector<TerrainsType> allow_terrain;

 private:
  UnitName _name;
  Country _country;
  int _pv;
  int _speed;  // Portée de déplacement case par case
  int _power;
  int _defense;
  int _range;  // Portée d'attaque
};