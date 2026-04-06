#include "resource_database.h"

std::map<ResourceName, Bonus> ResourceDatabase::_data = {
    // --- Strategic ---
    {ResourceName::Aluminum,
     {{ResourceType::Strategic, UnitAction::BuildMine, {0, 1, 0, 0, 0, 0, 0}},
      {ImprovementName::Mine, {0, 1, 1, 0, 0, 0, 0}}}},

    {ResourceName::Coal,
     {{ResourceType::Strategic, UnitAction::BuildMine, {0, 2, 0, 0, 0, 0, 0}},
      {ImprovementName::Mine, {0, 1, 0, 0, 0, 0, 0}}}},

    {ResourceName::Copper,
     {{ResourceType::Strategic, UnitAction::BuildMine, {0, 1, 0, 0, 0, 0, 0}},
      {ImprovementName::Mine, {0, 1, 0, 0, 0, 0, 0}}}},

    {ResourceName::Horse,
     {{ResourceType::Strategic,
       UnitAction::BuildPasture,
       {0, 1, 0, 0, 0, 0, 0}},
      {ImprovementName::Pasture, {0, 2, 1, 0, 0, 0, 0}}}},

    {ResourceName::Iron,
     {{ResourceType::Strategic, UnitAction::BuildMine, {0, 1, 0, 0, 0, 0, 0}},
      {ImprovementName::Mine, {0, 1, 0, 0, 0, 0, 0}}}},

    {ResourceName::Ivory,
     {{ResourceType::Production, UnitAction::BuildCamp, {0, 1, 0, 0, 0, 0, 0}},
      {ImprovementName::Camp, {0, 1, 1, 0, 0, 0, 0}}}},

    {ResourceName::Oil,
     {{ResourceType::Strategic, UnitAction::BuildWell, {0, 1, 0, 0, 0, 0, 0}},
      {ImprovementName::Well, {0, 2, 1, 0, 0, 0, 0}}}},

    {ResourceName::Uranium,
     {{ResourceType::Strategic, UnitAction::BuildMine, {0, 0, 0, 0, 0, 0, 0}},
      {ImprovementName::Mine, {0, 0, 3, 0, 0, 0, 0}}}},

    // --- Production ---
    {ResourceName::Marble,
     {{ResourceType::Production,
       UnitAction::BuildQuarry,
       {0, 0, 0, 0, 0, 0, 0}},
      {ImprovementName::Quarry, {0, 1, 2, 0, 0, 0, 0}}}},

    {ResourceName::Stone,
     {{ResourceType::Production,
       UnitAction::BuildQuarry,
       {0, 0, 0, 0, 0, 0, 0}},
      {ImprovementName::Quarry, {0, 2, 0, 0, 0, 0, 0}}}},

    // --- Food ---
    {ResourceName::Banana,
     {{ResourceType::Food, UnitAction::BuildPlantation, {1, 0, 0, 0, 0, 0, 0}},
      {ImprovementName::Plantation, {2, 0, 0, 0, 0, 0, 0}}}},

    {ResourceName::Clam,
     {{ResourceType::Food,
       UnitAction::BuildFishingBoats,
       {1, 0, 0, 0, 0, 0, 0}},
      {ImprovementName::FishingBoats, {2, 0, 0, 0, 0, 0, 0}}}},

    {ResourceName::Corn,
     {{ResourceType::Food, UnitAction::BuildFarm, {1, 0, 0, 0, 0, 0, 0}},
      {ImprovementName::Farm, {2, 0, 0, 0, 0, 0, 0}}}},

    {ResourceName::Cow,
     {{ResourceType::Food, UnitAction::BuildPasture, {1, 0, 0, 0, 0, 0, 0}},
      {ImprovementName::Pasture, {1, 2, 0, 0, 0, 0, 0}}}},

    {ResourceName::Crab,
     {{ResourceType::Food,
       UnitAction::BuildFishingBoats,
       {1, 0, 0, 0, 0, 0, 0}},
      {ImprovementName::FishingBoats, {2, 0, 0, 0, 0, 0, 0}}}},

    {ResourceName::Deer,
     {{ResourceType::Food, UnitAction::BuildCamp, {1, 0, 0, 0, 0, 0, 0}},
      {ImprovementName::Camp, {2, 0, 0, 0, 0, 0, 0}}}},

    {ResourceName::Fish,
     {{ResourceType::Food,
       UnitAction::BuildFishingBoats,
       {1, 0, 0, 0, 0, 0, 0}},
      {ImprovementName::FishingBoats, {3, 0, 0, 0, 0, 0, 0}}}},

    {ResourceName::Pig,
     {{ResourceType::Food, UnitAction::BuildPasture, {1, 0, 0, 0, 0, 0, 0}},
      {ImprovementName::Pasture, {3, 0, 0, 0, 0, 0, 0}}}},

    {ResourceName::Rice,
     {{ResourceType::Food, UnitAction::BuildFarm, {1, 0, 0, 0, 0, 0, 0}},
      {ImprovementName::Farm, {1, 0, 0, 0, 0, 0, 0}}}},

    {ResourceName::Sheep,
     {{ResourceType::Food, UnitAction::BuildPasture, {1, 0, 0, 0, 0, 0, 0}},
      {ImprovementName::Pasture, {2, 0, 1, 0, 0, 0, 0}}}},

    {ResourceName::Wheat,
     {{ResourceType::Food, UnitAction::BuildFarm, {1, 0, 0, 0, 0, 0, 0}},
      {ImprovementName::Farm, {1, 0, 0, 0, 0, 0, 0}}}},

    {ResourceName::Sugar,
     {{ResourceType::Food, UnitAction::BuildPlantation, {0, 0, 1, 0, 0, 0, 0}},
      {ImprovementName::Plantation, {1, 0, 1, 0, 0, 0, 0}}}},

    {ResourceName::Whale,
     {{ResourceType::Food,
       UnitAction::BuildWhalingBoats,
       {0, 0, 1, 0, 0, 0, 0}},
      {ImprovementName::WhalingBoats, {0, 1, 2, 0, 0, 0, 0}}}},

    // --- Commerce ---
    {ResourceName::Dye,
     {{ResourceType::Commerce,
       UnitAction::BuildPlantation,
       {0, 0, 1, 0, 0, 0, 0}},
      {ImprovementName::Plantation, {0, 0, 4, 0, 0, 0, 0}}}},

    {ResourceName::Fur,
     {{ResourceType::Commerce, UnitAction::BuildCamp, {0, 0, 1, 0, 0, 0, 0}},
      {ImprovementName::Camp, {0, 0, 3, 0, 0, 0, 0}}}},

    {ResourceName::Gems,
     {{ResourceType::Commerce, UnitAction::BuildMine, {0, 0, 1, 0, 0, 0, 0}},
      {ImprovementName::Mine, {0, 1, 5, 0, 0, 0, 0}}}},

    {ResourceName::Gold,
     {{ResourceType::Commerce, UnitAction::BuildMine, {0, 0, 1, 0, 0, 0, 0}},
      {ImprovementName::Mine, {0, 1, 6, 0, 0, 0, 0}}}},

    {ResourceName::Incense,
     {{ResourceType::Commerce,
       UnitAction::BuildPlantation,
       {0, 0, 1, 0, 0, 0, 0}},
      {ImprovementName::Plantation, {0, 0, 5, 0, 0, 0, 0}}}},

    {ResourceName::Silk,
     {{ResourceType::Commerce,
       UnitAction::BuildPlantation,
       {0, 0, 1, 0, 0, 0, 0}},
      {ImprovementName::Plantation, {0, 0, 3, 0, 0, 0, 0}}}},

    {ResourceName::Silver,
     {{ResourceType::Commerce, UnitAction::BuildMine, {0, 0, 1, 0, 0, 0, 0}},
      {ImprovementName::Mine, {0, 1, 4, 0, 0, 0, 0}}}},

    {ResourceName::Spices,
     {{ResourceType::Commerce,
       UnitAction::BuildPlantation,
       {0, 0, 1, 0, 0, 0, 0}},
      {ImprovementName::Plantation, {1, 0, 2, 0, 0, 0, 0}}}},

    {ResourceName::Wine,
     {{ResourceType::Commerce, UnitAction::BuildWinery, {0, 0, 1, 0, 0, 0, 0}},
      {ImprovementName::Winery, {1, 0, 2, 0, 0, 0, 0}}}},
};
