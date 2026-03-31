#pragma once
#include <map>
#include <vector>

enum class TerrainsType { CoastLake, Mountains, Ocean, Plains, Snow };

enum class ResourceName {
  None,

  // Ressources Stratégiques
  Horse,
  Iron,
  Copper,
  Coal,
  Oil,
  Uranium,
  Aluminum,

  // Ressources de Santé
  Corn,
  Rice,
  Wheat,
  Banana,
  Deer,
  Pig,
  Cow,
  Sheep,
  Fish,
  Clam,
  Crab,

  // Ressources de Luxe
  Gold,
  Silver,
  Gems,
  Ivory,
  Fur,
  Dye,
  Incense,
  Silk,
  Sugar,
  Spices,
  Wine,
  Whale
};

enum class BuildingName {
  None,
  City,
  Farm,
  Mine,
  Workshop,
  Cottage,
  Pasture,
  Plantation,
  Camp,
  WhalingBoats,
  FishingBoats
};

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

enum class UnitName {
  // Neutre
  Settler,
  Worker,

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
