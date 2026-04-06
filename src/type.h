#pragma once

#include <map>
#include <string>
#include <vector>

enum class TerrainsType {
  Coast,
  Desert,
  Grassland,
  Ice,
  Ocean,
  Plains,
  Tundra
};

enum class TerrainElevation { Flat, Hill, Mountain };

enum class TerrainFeature { None, Forest, Jungle };

enum class ResourceName {
  None,

  // --- Strategic ---
  Aluminum,
  Coal,
  Copper,
  Horse,
  Iron,
  Ivory,
  Oil,
  Uranium,

  // --- Production ---
  Marble,
  Stone,

  // --- Food ---
  Banana,
  Clam,
  Corn,
  Cow,
  Crab,
  Deer,
  Fish,
  Pig,
  Rice,
  Sheep,
  Wheat,
  Sugar,
  Whale,

  // --- Commerce ---
  Dye,
  Fur,
  Gems,
  Gold,
  Incense,
  Silk,
  Silver,
  Spices,
  Wine
};

enum class ImprovementName {
  None,
  Camp,              // Camp
  Cottage,           // Cottage
  Farm,              // Ferme
  FishingBoats,      // Bateaux de pêche
  ForestPreserve,    // Réserve boisée
  Fort,              // Fort
  Hamlet,            // Hameau
  LumberMill,        // Scierie
  Mine,              // Mine
  OffshorePlatform,  // Plate-forme offshore
  Pasture,           // Pâturage
  Plantation,        // Plantation
  Quarry,            // Carrière
  Town,              // Ville
  TribalVillage,     // Village tribal
  Village,           // Village
  Watermill,         // Moulin à eau
  Well,              // Puits
  WhalingBoats,      // Baleiniers
  Windmill,          // Moulin à vent
  Winery,            // Exploitation vinicole
  Workshop           // Atelier
};

struct Yields {
  int food = 0;
  int production = 0;
  int commerce = 0;
  int culture = 0;
  int science = 0;
  int happiness = 0;
  int health = 0;
  int sickness = 0;
  double defense = 0;
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

enum class Color {
  Transparent,    // Neutral
  RoyalBlue,      // France
  DarkGrey,       // Germany
  Crimson,        // UnitedKingdom
  White,          // Russia
  GoldenRod,      // Egypt
  BrightRed,      // Switzerland
  CherryBlossom,  // Japan
  Orange,         // Spain
  Emerald,        // Italy
  NavyBlue        // UnitedStates
};

extern const std::map<Country, Color> COUNTRY_COLOR;

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
  None,

  // Civil
  Settler,
  Worker,
  FastWorker,
  Spy,
  WorkBoat,

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

enum class UnitAction {
  // --- All ---
  SkipTurn,
  Delete,
  GoToMove,
  RegroupUnit,
  RegroupSameUnit,

  // --- Unité Terrestre ---
  Fortify,  // Mise en garde
  Pillage,
  Bombard,  // Affaiblir les défenses d'une ville

  // --- Settler ---
  BuildCity,

  // --- Worker ---
  RouteToMove,
  ChopDownForest,
  BuildRoad,
  BuildCamp,
  BuildCottage,
  BuildFarm,
  BuildMine,
  BuildForestPreserve,
  BuildFort,
  BuildHamlet,
  BuildLumberMill,
  BuildOffshorePlatform,
  BuildPasture,
  BuildPlantation,
  BuildQuarry,
  BuildTown,
  BuildTribalVillage,
  BuildVillage,
  BuildWatermill,
  BuildWell,
  BuildWindmill,
  BuildWinery,
  BuildWorkshop,

  // --- WorkBoat ---
  BuildFishingBoats,
  BuildWhalingBoats,
};
