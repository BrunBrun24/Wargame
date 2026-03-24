#pragma once

enum class TerrainsType { CoastLake, Mountains, Ocean, Plains, Snow };

class Terrains {
 public:
  Terrains() = default;
  Terrains(TerrainsType type);

  TerrainsType get_terrain_type() const { return _type; }
  char get_debug_char() const;

  void set_terrain(TerrainsType type) { _type = type; }

 private:
  TerrainsType _type;
  // int debuff;
};