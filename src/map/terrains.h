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

 private:
  TerrainsType _type;
  // int debuff;
};