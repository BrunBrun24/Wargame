#include "terrains.h"

Terrains::Terrains(TerrainsType type) : _type(type) {}

char Terrains::get_debug_char() const {
  switch (_type) {
    case TerrainsType::CoastLake:
      return 'C';
    case TerrainsType::Desert:
      return 'D';
    case TerrainsType::Grassland:
      return 'G';
    case TerrainsType::Mountains:
      return 'M';
    case TerrainsType::Ocean:
      return 'O';
    case TerrainsType::Plains:
      return 'P';
    case TerrainsType::Snow:
      return 'S';
    default:
      return '?';
  }
}
