#pragma once

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

class Player {
 public:
  Player(Country country);

 private:
  static int _id_counter;

  int _id;
  Country _country;
};