#pragma once

#include <memory>
#include <vector>

class Unit;
class Terrains;

enum class Country {
  Neutral,
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

using vector_units = std::vector<std::unique_ptr<Unit>>;
using vector_terrains = std::vector<std::unique_ptr<Terrains>>;

class Player {
 public:
  Player(Country country);

  void add_unit(std::unique_ptr<Unit> unit);
  void add_building(std::unique_ptr<Terrains> building);

  int get_id() const { return _id; }
  Country get_country() const { return _country; }

 private:
  static int _id_counter;

  int _id;
  Country _country;
  vector_units _units;
  vector_terrains _terrains;
};