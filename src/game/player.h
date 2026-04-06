#pragma once

#include <map>
#include <memory>
#include <vector>

#include "type.h"

class Unit;
class Case;
class City;

using ResourceInventory = std::map<ResourceName, int>;

struct PlayerOwns {
  int gold = 0;
  int science = 0;
  int culture = 0;
};

class Player {
 public:
  Player(Country country);
  ~Player();

  /** @brief Permet à l'utilisateur de choisir son pays dans le terminal */
  static Country choice_country(const std::vector<Country>& excluded_countries);

  void clear_units();
  void add_unit(Unit* unit);
  void remove_unit(Unit* unit);

  void add_improvement(Case* c);
  void remove_improvement(Case* c);

  void clear_citys();
  void add_city(City* c);
  void remove_city(City* c);

  void process_turn();

  void add_improvement_resource(ResourceName name) {
    _improvements_resources[name] += 1;
  };

  int get_id() const { return _id; }
  Country get_country() const { return _country; }
  ResourceInventory get_resources() { return _resources; }

 private:
  static int _id_counter;

  int _id;
  Country _country;
  std::vector<Unit*> _units;
  std::vector<Case*> _improvements;
  std::vector<City*> _citys;
  PlayerOwns _data;
  ResourceInventory _resources;  // Nombre de ressources possédant le joueur
  ResourceInventory
      _improvements_resources;  // Nombre de bâtiments générant une ressource
};