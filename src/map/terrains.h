#pragma once

enum class TerrainsType {
  CoastLake,
  Desert,
  Grassland,
  Mountains,
  Ocean,
  Plains,
  Snow
};

class Terrains {
 public:
  Terrains() = default;
  Terrains(TerrainsType type);

  TerrainsType get_terrain_type() const { return _type; }

 private:
  TerrainsType _type;
  // int debuff;
};