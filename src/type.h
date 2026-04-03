#pragma once

#include <map>
#include <string>
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
  Workshop,          // Atelier
  WhalingBoats,      // Baleiniers
  FishingBoats,      // Bateaux de pêche
  Camp,              // Camp
  Quarry,            // Carrière
  Cottage,           // Cottage
  Winery,            // Exploitation vinicole
  Farm,              // Ferme
  Fort,              // Fort
  Hamlet,            // Hameau
  Mine,              // Mine
  Watermill,         // Moulin à eau
  Windmill,          // Moulin à vent
  Plantation,        // Plantation
  OffshorePlatform,  // Plate-forme offshore
  Well,              // Puits
  Pasture,           // Pâturage
  Ruins,             // Ruines
  ForestPreserve,    // Réserve boisée
  LumberMill,        // Scierie
  Village,           // Village
  TribalVillage,     // Village tribal
};

struct Yields {
  int food = 0;
  int production = 0;
  int commerce = 0;
  double defense = 0;
  int happiness = 0;
  int health = 0;
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

enum class UnitType {
  Air,
  Archery,
  Armored,
  Gunpowder,
  Helicopter,
  Melee,
  Mounted,
  Naval,
  Recon,
  Siege,
  Missile,
  Civil
};

enum class UnitName {
  // Civil
  Settler,
  Worker,
  FastWorker,
  Spy,

  // Melee
  Warrior,
  Quechua,
  Swordsman,
  Jaguar,
  GallicWarrior,
  Praetorian,
  Axeman,
  Phalanx,
  Vulture,
  DogSoldier,
  Maceman,
  Samurai,
  Berserker,
  Spearman,
  Impi,
  Holkan,
  Pikeman,
  Landsknecht,

  // Gunpowder
  Musketman,
  Musketeer,
  Janissary,
  OromoWarrior,
  Rifleman,
  Redcoat,
  Grenadier,
  AntiTank,
  Infantry,
  SAMInfantry,
  Marine,
  NavySEAL,
  Paratrooper,
  MechanizedInfantry,

  // Archery
  Archer,
  Skirmisher,
  Bowman,
  Longbowman,
  Crossbowman,
  ChoKoNu,

  // Mounted
  Chariot,
  WarChariot,
  Immortal,
  HorseArcher,
  NumidianCavalry,
  Keshik,
  Knight,
  CamelArcher,
  Cataphract,
  Conquistador,
  Cuirassier,
  Cavalry,
  Cossack,
  WarElephant,
  BallistaElephant,

  // Armored
  Tank,
  Panzer,
  ModernArmor,
  Gunship,
  MobileSAM,

  // Siege
  Catapult,
  Hwacha,
  Trebuchet,
  Cannon,
  MachineGun,
  Artillery,
  MobileArtillery,

  // Naval
  WorkBoat,
  Galley,
  Trireme,
  Caravel,
  Carrack,
  Galleon,
  EastIndiaman,
  Privateer,
  Frigate,
  ShipOfTheLine,
  Ironclad,
  Transport,
  Destroyer,
  Battleship,
  MissileCruiser,
  StealthDestroyer,
  Submarine,
  AttackSubmarine,
  Carrier,

  // Air
  Airship,
  Fighter,
  JetFighter,
  Bomber,
  StealthBomber,

  // Missile
  GuidedMissile,
  TacticalNuke,
  ICBM,

  // Recon
  Scout,
  Explorer
};

struct UnitStats {
  int hp = 100;
  int power = 0;
  int movement = 0;
  int production = 0;
  int distance = 0;
};

extern const std::map<UnitName, UnitStats> UNIT_STATS;
extern const std::map<UnitName, UnitType> UNIT_TYPE;
extern const std::map<std::string, UnitName> UNIT_STRING_NAME;
