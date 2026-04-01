#pragma once

#include <map>
#include <memory>
#include <vector>

#include "type.h"

class Unit;

using ResourceInventory = std::map<ResourceName, int>;

class Player {
 public:
  Player(Country country);

  void add_unit(Unit* unit);
  void remove_unit(Unit* unit);
  void clear_units() { _units.clear(); }
  void add_building_resource(ResourceName name) {
    _buildings_resources[name] += 1;
  };

  int get_id() const { return _id; }
  Country get_country() const { return _country; }
  ResourceInventory get_resources() { return _resources; }

 private:
  static int _id_counter;

  int _id;
  Country _country;
  std::vector<Unit*> _units;
  ResourceInventory _resources;  // Nombre de ressources possédant le joueur
  ResourceInventory
      _buildings_resources;  // Nombre de bâtiments générant une ressource
};