#include "type.h"

#include <map>

const std::map<Country, Color> COUNTRY_COLOR = {
    {Country::Neutral, Color::Transparent},
    {Country::France, Color::RoyalBlue},
    {Country::Germany, Color::DarkGrey},
    {Country::UnitedKingdom, Color::Crimson},
    {Country::Russia, Color::White},
    {Country::Egypt, Color::GoldenRod},
    {Country::Switzerland, Color::BrightRed},
    {Country::Japan, Color::CherryBlossom},
    {Country::Spain, Color::Orange},
    {Country::Italy, Color::Emerald},
    {Country::UnitedStates, Color::NavyBlue}};

const std::map<UnitName, UnitStats> UNIT_STATS = {
    // --- Civil ---
    {UnitName::Settler, {1, 0, 2, 100}},
    {UnitName::Worker, {1, 0, 2, 60}},
    {UnitName::FastWorker, {1, 0, 3, 60}},
    {UnitName::WorkBoat, {1, 0, 2, 30}},

    // --- Melee ---
    {UnitName::Warrior, {100, 2, 1, 15}},
    {UnitName::Quechua, {100, 2, 1, 15}},
    {UnitName::Swordsman, {100, 6, 1, 40}},
    {UnitName::Jaguar, {100, 5, 1, 35}},
    {UnitName::GallicWarrior, {100, 6, 1, 40}},
    {UnitName::Praetorian, {100, 8, 1, 45}},
    {UnitName::Axeman, {100, 5, 1, 35}},
    {UnitName::Phalanx, {100, 5, 1, 35}},
    {UnitName::Vulture, {100, 6, 1, 35}},
    {UnitName::DogSoldier, {100, 4, 1, 35}},
    {UnitName::Maceman, {100, 8, 1, 70}},
    {UnitName::Samurai, {100, 8, 1, 70}},
    {UnitName::Berserker, {100, 8, 1, 70}},
    {UnitName::Spearman, {100, 4, 1, 35}},
    {UnitName::Impi, {100, 4, 2, 35}},
    {UnitName::Holkan, {100, 4, 1, 35}},
    {UnitName::Pikeman, {100, 6, 1, 60}},
    {UnitName::Landsknecht, {100, 6, 1, 60}},

    // --- Gunpowder ---
    {UnitName::Musketman, {100, 9, 1, 80}},
    {UnitName::Musketeer, {100, 9, 2, 80}},
    {UnitName::Janissary, {100, 9, 1, 80}},
    {UnitName::OromoWarrior, {100, 9, 1, 80}},
    {UnitName::Rifleman, {100, 14, 1, 110}},
    {UnitName::Redcoat, {100, 14, 1, 110}},
    {UnitName::Grenadier, {100, 12, 1, 100}},
    {UnitName::AntiTank, {100, 14, 1, 120}},
    {UnitName::Infantry, {100, 20, 1, 140}},
    {UnitName::SAMInfantry, {100, 18, 1, 150}},
    {UnitName::MobileSAM, {100, 22, 2, 220}},
    {UnitName::Marine, {100, 24, 1, 160}},
    {UnitName::NavySEAL, {100, 24, 1, 160}},
    {UnitName::Paratrooper, {100, 24, 1, 160}},
    {UnitName::MechanizedInfantry, {100, 32, 2, 200}},

    // --- Archery ---
    {UnitName::Archer, {100, 3, 1, 25}},
    {UnitName::Skirmisher, {100, 4, 1, 25}},
    {UnitName::Bowman, {100, 3, 1, 25}},
    {UnitName::Longbowman, {100, 6, 1, 50}},
    {UnitName::Crossbowman, {100, 6, 1, 60}},
    {UnitName::ChoKoNu, {100, 6, 1, 60}},

    // --- Mounted ---
    {UnitName::Chariot, {100, 4, 2, 30}},
    {UnitName::WarChariot, {100, 5, 2, 30}},
    {UnitName::Immortal, {100, 4, 2, 30}},
    {UnitName::HorseArcher, {100, 6, 2, 50}},
    {UnitName::NumidianCavalry, {100, 5, 2, 50}},
    {UnitName::Keshik, {100, 6, 2, 50}},
    {UnitName::Knight, {100, 10, 2, 90}},
    {UnitName::CamelArcher, {100, 10, 2, 90}},
    {UnitName::Cataphract, {100, 12, 2, 90}},
    {UnitName::Conquistador, {100, 12, 2, 100}},
    {UnitName::Cuirassier, {100, 12, 2, 100}},
    {UnitName::Cavalry, {100, 15, 2, 120}},
    {UnitName::Cossack, {100, 15, 2, 120}},
    {UnitName::WarElephant, {100, 8, 1, 60}},
    {UnitName::BallistaElephant, {100, 8, 1, 60}},

    // --- Armored & Helicopter ---
    {UnitName::Tank, {100, 28, 2, 180}},
    {UnitName::Panzer, {100, 28, 2, 180}},
    {UnitName::ModernArmor, {100, 40, 2, 240}},
    {UnitName::Gunship, {100, 24, 4, 160}},

    // --- Siege ---
    {UnitName::Catapult, {100, 5, 1, 50}},
    {UnitName::Hwacha, {100, 5, 1, 50}},
    {UnitName::Trebuchet, {100, 4, 1, 80}},
    {UnitName::Cannon, {100, 12, 1, 100}},
    {UnitName::MachineGun, {100, 18, 1, 125}},
    {UnitName::Artillery, {100, 18, 1, 150}},
    {UnitName::MobileArtillery, {100, 26, 2, 200}},

    // --- Naval ---
    {UnitName::Galley, {100, 2, 2, 50}},
    {UnitName::Trireme, {100, 2, 2, 50}},
    {UnitName::Caravel, {100, 3, 3, 60}},
    {UnitName::Carrack, {100, 3, 3, 60}},
    {UnitName::Galleon, {100, 4, 4, 80}},
    {UnitName::EastIndiaman, {100, 6, 4, 80}},
    {UnitName::Privateer, {100, 6, 4, 80}},
    {UnitName::Frigate, {100, 8, 4, 90}},
    {UnitName::ShipOfTheLine, {100, 8, 3, 120}},
    {UnitName::Ironclad, {100, 12, 2, 100}},
    {UnitName::Transport, {100, 16, 5, 125}},
    {UnitName::Destroyer, {100, 30, 8, 200}},
    {UnitName::Battleship, {100, 40, 6, 225}},
    {UnitName::MissileCruiser, {100, 40, 7, 260}},
    {UnitName::StealthDestroyer, {100, 30, 8, 220}},
    {UnitName::Submarine, {100, 24, 6, 150}},
    {UnitName::AttackSubmarine, {100, 30, 7, 200}},
    {UnitName::Carrier, {100, 16, 5, 175}},

    // --- Air ---
    {UnitName::Airship, {100, 4, 8, 80, 8}},
    {UnitName::Fighter, {100, 12, 6, 100, 6}},
    {UnitName::JetFighter, {100, 24, 10, 150, 10}},
    {UnitName::Bomber, {100, 16, 8, 140, 8}},
    {UnitName::StealthBomber, {100, 20, 12, 200, 12}},

    // --- Missile ---
    {UnitName::GuidedMissile, {100, 40, 1, 60, 4}},
    {UnitName::TacticalNuke, {100, 0, 1, 250, 4}},
    {UnitName::ICBM, {100, 0, 1, 500, 999}},

    // --- Recon ---
    {UnitName::Scout, {100, 1, 2, 15}},
    {UnitName::Explorer, {100, 4, 2, 40}}};

const std::map<UnitName, UnitType> UNIT_TYPE = {
    // --- Civil ---
    {UnitName::Settler, UnitType::Civil},
    {UnitName::Worker, UnitType::Civil},
    {UnitName::FastWorker, UnitType::Civil},
    {UnitName::Spy, UnitType::Civil},

    // --- Melee ---
    {UnitName::Warrior, UnitType::Melee},
    {UnitName::Quechua, UnitType::Melee},
    {UnitName::Swordsman, UnitType::Melee},
    {UnitName::Jaguar, UnitType::Melee},
    {UnitName::GallicWarrior, UnitType::Melee},
    {UnitName::Praetorian, UnitType::Melee},
    {UnitName::Axeman, UnitType::Melee},
    {UnitName::Phalanx, UnitType::Melee},
    {UnitName::Vulture, UnitType::Melee},
    {UnitName::DogSoldier, UnitType::Melee},
    {UnitName::Maceman, UnitType::Melee},
    {UnitName::Samurai, UnitType::Melee},
    {UnitName::Berserker, UnitType::Melee},
    {UnitName::Spearman, UnitType::Melee},
    {UnitName::Impi, UnitType::Melee},
    {UnitName::Holkan, UnitType::Melee},
    {UnitName::Pikeman, UnitType::Melee},
    {UnitName::Landsknecht, UnitType::Melee},

    // --- Gunpowder ---
    {UnitName::Musketman, UnitType::Gunpowder},
    {UnitName::Musketeer, UnitType::Gunpowder},
    {UnitName::Janissary, UnitType::Gunpowder},
    {UnitName::OromoWarrior, UnitType::Gunpowder},
    {UnitName::Rifleman, UnitType::Gunpowder},
    {UnitName::Redcoat, UnitType::Gunpowder},
    {UnitName::Grenadier, UnitType::Gunpowder},
    {UnitName::AntiTank, UnitType::Gunpowder},
    {UnitName::Infantry, UnitType::Gunpowder},
    {UnitName::SAMInfantry, UnitType::Gunpowder},
    {UnitName::Marine, UnitType::Gunpowder},
    {UnitName::NavySEAL, UnitType::Gunpowder},
    {UnitName::Paratrooper, UnitType::Gunpowder},
    {UnitName::MechanizedInfantry, UnitType::Gunpowder},

    // --- Archery ---
    {UnitName::Archer, UnitType::Archery},
    {UnitName::Skirmisher, UnitType::Archery},
    {UnitName::Bowman, UnitType::Archery},
    {UnitName::Longbowman, UnitType::Archery},
    {UnitName::Crossbowman, UnitType::Archery},
    {UnitName::ChoKoNu, UnitType::Archery},

    // --- Mounted ---
    {UnitName::Chariot, UnitType::Mounted},
    {UnitName::WarChariot, UnitType::Mounted},
    {UnitName::Immortal, UnitType::Mounted},
    {UnitName::HorseArcher, UnitType::Mounted},
    {UnitName::NumidianCavalry, UnitType::Mounted},
    {UnitName::Keshik, UnitType::Mounted},
    {UnitName::Knight, UnitType::Mounted},
    {UnitName::CamelArcher, UnitType::Mounted},
    {UnitName::Cataphract, UnitType::Mounted},
    {UnitName::Conquistador, UnitType::Mounted},
    {UnitName::Cuirassier, UnitType::Mounted},
    {UnitName::Cavalry, UnitType::Mounted},
    {UnitName::Cossack, UnitType::Mounted},
    {UnitName::WarElephant, UnitType::Mounted},
    {UnitName::BallistaElephant, UnitType::Mounted},

    // --- Armored & Helicopter ---
    {UnitName::Tank, UnitType::Armored},
    {UnitName::Panzer, UnitType::Armored},
    {UnitName::ModernArmor, UnitType::Armored},
    {UnitName::Gunship, UnitType::Helicopter},
    {UnitName::MobileSAM, UnitType::Armored},

    // --- Siege ---
    {UnitName::Catapult, UnitType::Siege},
    {UnitName::Hwacha, UnitType::Siege},
    {UnitName::Trebuchet, UnitType::Siege},
    {UnitName::Cannon, UnitType::Siege},
    {UnitName::MachineGun, UnitType::Siege},
    {UnitName::Artillery, UnitType::Siege},
    {UnitName::MobileArtillery, UnitType::Siege},

    // --- Naval ---
    {UnitName::WorkBoat, UnitType::Naval},
    {UnitName::Galley, UnitType::Naval},
    {UnitName::Trireme, UnitType::Naval},
    {UnitName::Caravel, UnitType::Naval},
    {UnitName::Carrack, UnitType::Naval},
    {UnitName::Galleon, UnitType::Naval},
    {UnitName::EastIndiaman, UnitType::Naval},
    {UnitName::Privateer, UnitType::Naval},
    {UnitName::Frigate, UnitType::Naval},
    {UnitName::ShipOfTheLine, UnitType::Naval},
    {UnitName::Ironclad, UnitType::Naval},
    {UnitName::Transport, UnitType::Naval},
    {UnitName::Destroyer, UnitType::Naval},
    {UnitName::Battleship, UnitType::Naval},
    {UnitName::MissileCruiser, UnitType::Naval},
    {UnitName::StealthDestroyer, UnitType::Naval},
    {UnitName::Submarine, UnitType::Naval},
    {UnitName::AttackSubmarine, UnitType::Naval},
    {UnitName::Carrier, UnitType::Naval},

    // --- Air ---
    {UnitName::Airship, UnitType::Air},
    {UnitName::Fighter, UnitType::Air},
    {UnitName::JetFighter, UnitType::Air},
    {UnitName::Bomber, UnitType::Air},
    {UnitName::StealthBomber, UnitType::Air},

    // --- Missile ---
    {UnitName::GuidedMissile, UnitType::Missile},
    {UnitName::TacticalNuke, UnitType::Missile},
    {UnitName::ICBM, UnitType::Missile},

    // --- Recon ---
    {UnitName::Scout, UnitType::Recon},
    {UnitName::Explorer, UnitType::Recon}};

const std::map<std::string, UnitName> UNIT_STRING_NAME = {
    // Civil
    {"Settler", UnitName::Settler},
    {"Worker", UnitName::Worker},
    {"FastWorker", UnitName::FastWorker},
    {"Spy", UnitName::Spy},
    {"WorkBoat", UnitName::WorkBoat},

    // Melee
    {"Warrior", UnitName::Warrior},
    {"Quechua", UnitName::Quechua},
    {"Swordsman", UnitName::Swordsman},
    {"Jaguar", UnitName::Jaguar},
    {"GallicWarrior", UnitName::GallicWarrior},
    {"Praetorian", UnitName::Praetorian},
    {"Axeman", UnitName::Axeman},
    {"Phalanx", UnitName::Phalanx},
    {"Vulture", UnitName::Vulture},
    {"DogSoldier", UnitName::DogSoldier},
    {"Maceman", UnitName::Maceman},
    {"Samurai", UnitName::Samurai},
    {"Berserker", UnitName::Berserker},
    {"Spearman", UnitName::Spearman},
    {"Impi", UnitName::Impi},
    {"Holkan", UnitName::Holkan},
    {"Pikeman", UnitName::Pikeman},
    {"Landsknecht", UnitName::Landsknecht},

    // Gunpowder
    {"Musketman", UnitName::Musketman},
    {"Musketeer", UnitName::Musketeer},
    {"Janissary", UnitName::Janissary},
    {"OromoWarrior", UnitName::OromoWarrior},
    {"Rifleman", UnitName::Rifleman},
    {"Redcoat", UnitName::Redcoat},
    {"Grenadier", UnitName::Grenadier},
    {"AntiTank", UnitName::AntiTank},
    {"Infantry", UnitName::Infantry},
    {"SAMInfantry", UnitName::SAMInfantry},
    {"Marine", UnitName::Marine},
    {"NavySEAL", UnitName::NavySEAL},
    {"Paratrooper", UnitName::Paratrooper},
    {"MechanizedInfantry", UnitName::MechanizedInfantry},

    // Archery
    {"Archer", UnitName::Archer},
    {"Skirmisher", UnitName::Skirmisher},
    {"Bowman", UnitName::Bowman},
    {"Longbowman", UnitName::Longbowman},
    {"Crossbowman", UnitName::Crossbowman},
    {"ChoKoNu", UnitName::ChoKoNu},

    // Mounted
    {"Chariot", UnitName::Chariot},
    {"WarChariot", UnitName::WarChariot},
    {"Immortal", UnitName::Immortal},
    {"HorseArcher", UnitName::HorseArcher},
    {"NumidianCavalry", UnitName::NumidianCavalry},
    {"Keshik", UnitName::Keshik},
    {"Knight", UnitName::Knight},
    {"CamelArcher", UnitName::CamelArcher},
    {"Cataphract", UnitName::Cataphract},
    {"Conquistador", UnitName::Conquistador},
    {"Cuirassier", UnitName::Cuirassier},
    {"Cavalry", UnitName::Cavalry},
    {"Cossack", UnitName::Cossack},
    {"WarElephant", UnitName::WarElephant},
    {"BallistaElephant", UnitName::BallistaElephant},

    // Armored
    {"Tank", UnitName::Tank},
    {"Panzer", UnitName::Panzer},
    {"ModernArmor", UnitName::ModernArmor},
    {"Gunship", UnitName::Gunship},
    {"MobileSAM", UnitName::MobileSAM},

    // Siege
    {"Catapult", UnitName::Catapult},
    {"Hwacha", UnitName::Hwacha},
    {"Trebuchet", UnitName::Trebuchet},
    {"Cannon", UnitName::Cannon},
    {"MachineGun", UnitName::MachineGun},
    {"Artillery", UnitName::Artillery},
    {"MobileArtillery", UnitName::MobileArtillery},

    // Naval
    {"Galley", UnitName::Galley},
    {"Trireme", UnitName::Trireme},
    {"Caravel", UnitName::Caravel},
    {"Carrack", UnitName::Carrack},
    {"Galleon", UnitName::Galleon},
    {"EastIndiaman", UnitName::EastIndiaman},
    {"Privateer", UnitName::Privateer},
    {"Frigate", UnitName::Frigate},
    {"ShipOfTheLine", UnitName::ShipOfTheLine},
    {"Ironclad", UnitName::Ironclad},
    {"Transport", UnitName::Transport},
    {"Destroyer", UnitName::Destroyer},
    {"Battleship", UnitName::Battleship},
    {"MissileCruiser", UnitName::MissileCruiser},
    {"StealthDestroyer", UnitName::StealthDestroyer},
    {"Submarine", UnitName::Submarine},
    {"AttackSubmarine", UnitName::AttackSubmarine},
    {"Carrier", UnitName::Carrier},

    // Air
    {"Airship", UnitName::Airship},
    {"Fighter", UnitName::Fighter},
    {"JetFighter", UnitName::JetFighter},
    {"Bomber", UnitName::Bomber},
    {"StealthBomber", UnitName::StealthBomber},

    // Missile
    {"GuidedMissile", UnitName::GuidedMissile},
    {"TacticalNuke", UnitName::TacticalNuke},
    {"ICBM", UnitName::ICBM},

    // Recon
    {"Scout", UnitName::Scout},
    {"Explorer", UnitName::Explorer}};

const std::map<UnitName, PowerBonus> UNIT_POWER_BONUS = {
    {UnitName::Warrior, {{}, {}, {}, {0.0, 0.25}}},
    {UnitName::Spearman, {{{UnitType::Mounted, 1.0}}, {}, {}, {0.0, 0.0}}},
    {UnitName::Axeman, {{{UnitType::Melee, 0.5}}, {}, {}, {0.0, 0.0}}},
    {UnitName::Swordsman, {{}, {}, {}, {0.10, 0.0}}},
    {UnitName::Pikeman, {{{UnitType::Mounted, 1.0}}, {}, {}, {0.0, 0.0}}},
    {UnitName::Maceman, {{{UnitType::Melee, 0.5}}, {}, {}, {0.0, 0.0}}},

    {UnitName::Archer, {{}, {}, {{TerrainElevation::Hill, 0.25}}, {0.0, 0.50}}},
    {UnitName::Longbowman,
     {{}, {}, {{TerrainElevation::Hill, 0.25}}, {0.0, 0.50}}},
    {UnitName::Crossbowman, {{{UnitType::Melee, 0.5}}, {}, {}, {0.0, 0.0}}},

    {UnitName::Musketman, {{}, {}, {}, {0.0, 0.0}}},
    {UnitName::Grenadier, {{}, {{UnitName::Rifleman, 0.5}}, {}, {0.0, 0.0}}},
    {UnitName::Rifleman, {{{UnitType::Mounted, 0.25}}, {}, {}, {0.0, 0.0}}},
    {UnitName::AntiTank, {{{UnitType::Armored, 1.0}}, {}, {}, {0.0, 0.0}}},
    {UnitName::MachineGun, {{{UnitType::Gunpowder, 0.5}}, {}, {}, {0.0, 0.0}}},
    {UnitName::SAMInfantry,
     {{{UnitType::Helicopter, 0.75}}, {}, {}, {0.0, 0.0}}},
    {UnitName::Infantry, {{{UnitType::Gunpowder, 0.25}}, {}, {}, {0.0, 0.0}}},
    {UnitName::MobileSAM, {{{UnitType::Helicopter, 0.50}}, {}, {}, {0.0, 0.0}}},
    {UnitName::Marine,
     {{{UnitType::Siege, 0.50}},
      {{UnitName::MachineGun, 0.5}},
      {},
      {0.0, 0.0}}},
    {UnitName::Paratrooper, {{}, {}, {}, {0.0, 0.0}}},
    {UnitName::MechanizedInfantry, {{}, {}, {}, {0.0, 0.0}}},

    {UnitName::Chariot, {{}, {{UnitName::Axeman, 1.0}}, {}, {0.0, 0.0}}},
    {UnitName::HorseArcher, {{}, {}, {}, {0.0, 0.0}}},
    {UnitName::WarElephant, {{{UnitType::Mounted, 0.5}}, {}, {}, {0.0, 0.0}}},
    {UnitName::Knight, {{}, {}, {}, {0.0, 0.0}}},
    {UnitName::Cuirassier, {{}, {}, {}, {0.0, 0.0}}},
    {UnitName::Cavalry, {{}, {{UnitName::Cannon, 0.5}}, {}, {0.0, 0.0}}},

    {UnitName::Gunship, {{{UnitType::Armored, 1.0}}, {}, {}, {0.0, 0.0}}},

    {UnitName::Catapult, {{}, {}, {}, {0.0, 0.0}}},
    {UnitName::Trebuchet, {{}, {}, {}, {1.0, 0.0}}},
    {UnitName::Cannon, {{}, {}, {}, {0.0, 0.0}}},
    {UnitName::Artillery, {{{UnitType::Siege, 0.5}}, {}, {}, {0.0, 0.0}}},
    {UnitName::MobileArtillery, {{{UnitType::Siege, 0.5}}, {}, {}, {0.0, 0.0}}},

    {UnitName::Tank, {{}, {}, {}, {0.0, 0.0}}},
    {UnitName::ModernArmor, {{}, {}, {}, {0.0, 0.0}}},

    {UnitName::Scout, {{}, {}, {}, {0.0, 0.0}}},
    {UnitName::Explorer, {{}, {}, {}, {0.0, 0.0}}},

    {UnitName::Galley, {{}, {}, {}, {0.0, 0.0}}},
    {UnitName::Trireme, {{}, {{UnitName::Galley, 0.5}}, {}, {0.0, 0.0}}},
    {UnitName::Caravel, {{}, {}, {}, {0.0, 0.0}}},
    {UnitName::Galleon, {{}, {}, {}, {0.0, 0.0}}},
    {UnitName::Privateer, {{}, {}, {}, {0.0, 0.0}}},
    {UnitName::Frigate, {{}, {}, {}, {0.0, 0.0}}},
    {UnitName::ShipOfTheLine, {{}, {{UnitName::Frigate, 0.5}}, {}, {0.0, 0.0}}},
    {UnitName::Ironclad, {{}, {}, {}, {0.0, 0.0}}},
    {UnitName::Transport, {{}, {}, {}, {0.0, 0.0}}},
    {UnitName::Destroyer, {{}, {}, {}, {0.0, 0.0}}},
    {UnitName::Battleship, {{}, {}, {}, {0.0, 0.0}}},
    {UnitName::Submarine, {{}, {}, {}, {0.0, 0.0}}},
    {UnitName::AttackSubmarine,
     {{}, {{UnitName::Submarine, 0.5}}, {}, {0.0, 0.0}}},
    {UnitName::MissileCruiser, {{}, {}, {}, {0.0, 0.0}}},
    {UnitName::StealthDestroyer, {{}, {}, {}, {0.0, 0.0}}},
    {UnitName::Carrier, {{}, {}, {}, {0.0, 0.0}}},

    {UnitName::Airship, {{}, {}, {}, {0.0, 0.0}}},
    {UnitName::Fighter, {{}, {}, {}, {0.0, 0.0}}},
    {UnitName::JetFighter, {{}, {}, {}, {0.0, 0.0}}},
    {UnitName::Bomber, {{{UnitType::Naval, -0.5}}, {}, {}, {0.0, 0.0}}},
    {UnitName::StealthBomber, {{{UnitType::Naval, -0.5}}, {}, {}, {0.0, 0.0}}},

    {UnitName::GuidedMissile, {{}, {}, {}, {0.0, 0.0}}},
    {UnitName::TacticalNuke, {{}, {}, {}, {0.0, 0.0}}},
    {UnitName::ICBM, {{}, {}, {}, {0.0, 0.0}}},
};
