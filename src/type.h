#pragma once

#include <map>
#include <string>
#include <vector>

class Case;

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
  Aluminium,
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
  Road,              // Route
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

enum class TechnologyName {
  None,
  AdvancedFlight,
  Aesthetics,
  Agriculture,
  Alphabet,
  AnimalHusbandry,
  Archery,
  Artillery,
  AssemblyLine,
  Astronomy,
  Banking,
  Biology,
  BronzeWorking,
  Calendar,
  Chemistry,
  CivilService,
  CodeOfLaws,
  Combustion,
  Communism,
  Compass,
  Composites,
  Computers,
  Constitution,
  Construction,
  Corporation,
  Currency,
  Democracy,
  DivineRight,
  Drama,
  Ecology,
  Economics,
  Education,
  Electricity,
  Engineering,
  Fascism,
  Feudalism,
  FiberOptics,
  Fishing,
  Fission,
  Flight,
  Fusion,
  FutureTech,
  Genetics,
  Guilds,
  Gunpowder,
  HorsebackRiding,
  Hunting,
  Industrialism,
  IronWorking,
  Laser,
  Liberalism,
  Literature,
  Machinery,
  Masonry,
  MassMedia,
  Mathematics,
  Medicine,
  Meditation,
  MetalCasting,
  MilitaryScience,
  MilitaryTradition,
  Mining,
  Monarchy,
  Monotheism,
  Music,
  Mysticism,
  Nationalism,
  Optics,
  Paper,
  Philosophy,
  Physics,
  Plastics,
  Polytheism,
  Pottery,
  Priesthood,
  PrintingPress,
  Radio,
  Railroad,
  Refrigeration,
  ReplaceableParts,
  Rifling,
  Robotics,
  Rocketry,
  Sailing,
  Satellites,
  ScientificMethod,
  Stealth,
  SteamPower,
  Steel,
  Superconductors,
  TheWheel,
  Theology,
  Writing
};

extern const std::map<TechnologyName, std::string> TECHNOLOGY_STRING_NAME;

struct Yields {
  double food = 0;
  double production = 0;
  double commerce = 0;
  double culture = 0;
  double science = 0;
  double happiness = 0;
  double unhappiness = 0;
  double health = 0;
  double sickness = 0;
  double defense = 0;
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
  None,

  // Civil
  Settler,
  Worker,
  WorkBoat,

  // Melee
  Warrior,
  Swordsman,
  Axeman,
  Maceman,
  Pikeman,

  // Gunpowder
  Musketman,
  Rifleman,
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
  Longbowman,
  Crossbowman,

  // Mounted
  Chariot,
  HorseArcher,
  Cuirassier,
  Cavalry,
  WarElephant,
  BallistaElephant,

  // Armored
  Tank,
  ModernArmor,
  Gunship,
  MobileSAM,

  // Siege
  Catapult,
  Trebuchet,
  Cannon,
  MachineGun,
  Artillery,
  MobileArtillery,

  // Naval
  Galley,
  Trireme,
  Caravel,
  Galleon,
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
  double PM = 0;
  int production = 0;
  int distance = 0;
};

extern const std::map<UnitName, UnitStats> UNIT_STATS;
extern const std::map<UnitName, UnitType> UNIT_TYPE;
extern const std::map<std::string, UnitName> UNIT_STRING_NAME;

struct UnitRequired {
  std::vector<TechnologyName> technologies;
  std::vector<ResourceName> resource;
};

extern const std::map<UnitName, UnitRequired> UNIT_TECHNOLOGY;

struct PowerBonusUnitType {
  UnitType type;
  double attack = 0.0;  // force en plus en pourcentage
};

struct PowerBonusUnitName {
  UnitName name;
  double attack = 0.0;
};

struct PowerBonusTerrain {
  TerrainElevation type;
  double attack = 0.0;
};

struct PowerBonusCity {
  double attack = 0.0;
  double defense = 0.0;
};

struct PowerBonus {
  std::vector<PowerBonusUnitType> units_types = {};
  std::vector<PowerBonusUnitName> units_names = {};
  std::vector<PowerBonusTerrain> terrain = {};
  PowerBonusCity city = {};
};

extern const std::map<UnitName, PowerBonus> UNIT_POWER_BONUS;

enum class UnitAction {
  None,

  // --- All ---
  Sleep,
  SkipTurn,
  Delete,
  Wake,
  GoToMove,
  RegroupUnit,
  RegroupSameUnit,

  // --- Unité Terrestre ---
  Fortify,    // Mise en garde
  UnFortify,  // Mise en garde
  Pillage,
  Bombard,  // Affaiblir les défenses d'une ville

  // --- Settler ---
  BuildCity,

  // --- Worker ---
  ChopDownForest,
  BuildRoad,
  BuildCamp,
  BuildCottage,
  BuildFarm,
  BuildMine,
  BuildForestPreserve,
  BuildFort,
  BuildLumberMill,
  BuildOffshorePlatform,
  BuildPasture,
  BuildPlantation,
  BuildQuarry,
  BuildWatermill,
  BuildWell,
  BuildWindmill,
  BuildWinery,
  BuildWorkshop,

  // --- WorkBoat ---
  BuildFishingBoats,
  BuildWhalingBoats,
};

enum class BuildingName {
  None,
  Airport,
  Aqueduct,
  Bank,
  Barracks,
  BombShelters,
  BroadcastTower,
  BuddhistMonastery,
  Bunker,
  Castle,
  ChristianCathedral,
  ChristianMonastery,
  ChristianTemple,
  Citadel,
  CoalPlant,
  Colosseum,
  Courthouse,
  Drydock,
  Factory,
  Forge,
  Granary,
  Grocer,
  Harbor,
  Hospital,
  HydroPlant,
  IndustrialPark,
  Jail,
  Laboratory,
  Levee,
  Library,
  Lighthouse,
  Market,
  Monument,
  NuclearPlant,
  Observatory,
  PublicTransportation,
  RecyclingCenter,
  SecurityBureau,
  Stable,
  Supermarket,
  Theatre,
  University,
  Walls
};

struct Course {
  bool is_possible;
  std::vector<Case*> distance_traveled;
  double PM;  // PM utilisé lors du trajet
};