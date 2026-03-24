#include <iostream>
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



class Unit {
 public:
  Unit(UnitName name, Country country, int pv, int speed, int power,
       int defense, int range, std::vector<TerrainsType> allow_terrain);
  ~Unit();

  void attack(Unit* ennemy);
  bool find_terrain(const TerrainsType& target_terrain) const;

  int get_id() const { return _id; }
  std::vector<TerrainsType> get_allow_terrain() const { return allow_terrain; }
  Country get_country() const { return _country; }
  int get_pv() const { return _pv; }
  int get_power() const { return _power; }
  int get_def() const { return _defense; }
  int get_speed() const { return _speed; }

  void set_pv(const int atq) { _pv -= atq; }

 protected:
  std::vector<TerrainsType> allow_terrain;

 private:
  static int _id_counter;
  static std::vector<UnitName> sens_troupes;

  int _id;
  UnitName _name;
  Country _country;
  int _pv;
  int _speed;  // Portée de déplacement case par case
  int _power;
  int _defense;
  int _range;  // Portée d'attaque
  std::vector<int> _avantage;
};