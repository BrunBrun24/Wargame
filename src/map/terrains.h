#pragma once

enum class TerrainsType { CoastLake, Mountains, Ocean, Plains, Snow };
enum class BuildingType { NoBuilding, City, GoldMine, IronMine, Oil };

class Terrains {
 public:
  Terrains() = default;
  Terrains(TerrainsType type);
  Terrains(TerrainsType type, BuildingType building);

  TerrainsType get_terrain_type() const { return _type; }
  char get_debug_char() const;
  BuildingType get_building() { return _building; }

  void set_terrain(TerrainsType type) { _type = type; }
  void set_building(BuildingType building) { _building = building; }

 private:
  TerrainsType _type;
  BuildingType _building;
  // int debuff;
};