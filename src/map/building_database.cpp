#include "building_database.h"

std::map<BuildingName, BuildingInfo> BuildingDatabase::_data = {
  {BuildingName::Airport, {{TechnologyName::Flight}, {250, 0, 0, 0, 0, 0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0.0}}}},
  {BuildingName::Aqueduct, {{TechnologyName::Mathematics, TechnologyName::Masonry}, {100, 0, 0, 0, 2, 0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0.0}}}},
  {BuildingName::Bank, {{TechnologyName::Banking}, {200, 0, 0, 0, 0, 0, {0, 0, 0.5, 0, 0, 0, 0, 0, 0, 0.0}}}},
  {BuildingName::Barracks, {{}, {50, 0, 0, 1, 0, 0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0.0}}}},
  {BuildingName::BombShelters, {{TechnologyName::Electricity}, {100, 0, 0, 0, 0, 0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0.0}}}},
  {BuildingName::BroadcastTower, {{TechnologyName::MassMedia}, {175, 0, 0, 0, 0, 0, {0, 0, 0, 0.5, 0, 0, 0, 0, 0, 0.0}}}},
  {BuildingName::BuddhistMonastery, {{TechnologyName::Meditation}, {60, 0, 2, 0, 0, 0, {0, 0, 0, 0, 0.1, 0, 0, 0, 0, 0.0}}}},
  {BuildingName::Bunker, {{TechnologyName::Electricity}, {100, 0, 0, 0, 0, 0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0.0}}}},
  {BuildingName::Castle, {{TechnologyName::Engineering}, {100, 0, 1, 0, 0, 0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0.0}}}},
  {BuildingName::ChristianCathedral, {{TechnologyName::Music}, {300, 0, 0, 0, 0, 0, {0, 0, 0, 0.5, 0, 0, 0, 0, 0, 0.0}}}},
  {BuildingName::ChristianMonastery, {{TechnologyName::Meditation}, {60, 0, 2, 0, 0, 0, {0, 0, 0, 0.1, 0, 0, 0, 0, 0, 0.0}}}},
  {BuildingName::ChristianTemple, {{TechnologyName::Priesthood}, {80, 0, 1, 1, 0, 0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0.0}}}},
  {BuildingName::Citadel, {{TechnologyName::Engineering}, {100, 0, 1, 0, 0, 0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0.0}}}},
  {BuildingName::CoalPlant, {{TechnologyName::AssemblyLine}, {150, 0, 0, 0, 0, 0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0.0}}}},
  {BuildingName::Colosseum, {{TechnologyName::Construction}, {80, 0, 0, 1, 0, 0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0.0}}}},
  {BuildingName::Drydock, {{TechnologyName::Steel}, {120, 0, 0, 0, 0, 1, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0.0}}}},
  {BuildingName::Factory, {{TechnologyName::AssemblyLine}, {250, 0, 0, 0, 0, 1, {0, 0.25, 0, 0, 0, 0, 0, 0, 0, 0.0}}}},
  {BuildingName::Forge, {{TechnologyName::MetalCasting}, {125, 0, 0, 0, 0, 1, {0, 0.25, 0, 0, 0, 0, 0, 0, 0, 0.0}}}},
  {BuildingName::Granary, {{TechnologyName::Pottery}, {60, 0, 0, 0, 0, 0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0.0}}}},
  {BuildingName::Grocer, {{TechnologyName::Guilds, TechnologyName::Currency}, {150, 0, 0, 0, 0, 0, {0, 0, 0.25, 0, 0, 0, 0, 0, 0, 0.0}}}},
  {BuildingName::Harbor, {{TechnologyName::Compass}, {80, 0, 0, 0, 0, 0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0.0}}}},
  {BuildingName::Hospital, {{TechnologyName::Medicine}, {200, 0, 0, 0, 3, 0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0.0}}}},
  {BuildingName::HydroPlant, {{TechnologyName::Plastics}, {200, 0, 0, 0, 0, 0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0.0}}}}, // TODO Amène l'éléctricité
  {BuildingName::IndustrialPark, {{TechnologyName::Industrialism}, {200, 0, 0, 0, 0, 2, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0.0}}}},
  {BuildingName::Jail, {{TechnologyName::Constitution}, {120, 0, 0, 0, 0, 0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0.0}}}},
  {BuildingName::Laboratory, {{TechnologyName::Superconductors}, {250, 0, 0, 0, 0, 1, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0.0}}}},
  {BuildingName::Levee, {{TechnologyName::Superconductors}, {180, 0, 0, 0, 0, 0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0.0}}}},
  {BuildingName::Library, {{TechnologyName::Writing}, {90, 0, 2, 0, 0, 0, {0, 0, 0, 0, 0.25, 0, 0, 0, 0, 0.0}}}},
  {BuildingName::Lighthouse, {{TechnologyName::Sailing}, {60, 1, 0, 0, 0, 0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0.0}}}},
  {BuildingName::Market, {{TechnologyName::Currency}, {150, 0, 0, 0, 0, 0, {0, 0.25, 0, 0, 0, 0, 0, 0, 0, 0.0}}}},
  {BuildingName::Monument, {{TechnologyName::Mysticism}, {30, 0, 1, 1, 0, 0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0.0}}}},
  {BuildingName::NuclearPlant, {{TechnologyName::Fission}, {250, 0, 0, 0, 0, 0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0.0}}}},
  {BuildingName::Observatory, {{TechnologyName::Astronomy}, {150, 0, 0, 0, 0, 0, {0, 0, 0, 0, 0.25, 0, 0, 0, 0, 0.0}}}},
  {BuildingName::PublicTransportation, {{TechnologyName::Combustion}, {150, 0, 0, 0, 1, 0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0.0}}}},
  {BuildingName::RecyclingCenter, {{TechnologyName::Ecology}, {300, 0, 0, 0, 0, 0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0.0}}}},
  {BuildingName::SecurityBureau, {{TechnologyName::Democracy}, {220, 0, 0, 0, 0, 0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0.0}}}},
  {BuildingName::Stable, {{TechnologyName::HorsebackRiding}, {60, 0, 0, 0, 0, 0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0.0}}}},
  {BuildingName::Supermarket, {{TechnologyName::Refrigeration}, {150, 0, 1, 0, 0, 0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0.0}}}},
  {BuildingName::Theatre, {{TechnologyName::Drama}, {50, 0, 3, 0, 0, 0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0.0}}}},
  {BuildingName::University, {{TechnologyName::Education}, {200, 0, 3, 0, 0, 0, {0, 0, 0, 0, 0.25, 0, 0, 0, 0, 0.0}}}},
  {BuildingName::Walls, {{TechnologyName::Masonry}, {50, 0, 0, 0, 0, 0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0.0}}}}
};

std::string BuildingDatabase::get_building_name_str(BuildingName name) {
  switch (name) {
    case BuildingName::Airport:
      return "Airport";
    case BuildingName::Aqueduct:
      return "Aqueduct";
    case BuildingName::Bank:
      return "Bank";
    case BuildingName::BombShelters:
      return "Bomb Shelters";
    case BuildingName::Barracks:
      return "Barracks";
    case BuildingName::BroadcastTower:
      return "Broadcast Tower";
    case BuildingName::BuddhistMonastery:
      return "Buddhist Monastery";
    case BuildingName::Bunker:
      return "Bunker";
    case BuildingName::Castle:
      return "Castle";
    case BuildingName::ChristianMonastery:
      return "Christian Monastery";
    case BuildingName::ChristianTemple:
      return "Christian Temple";
    case BuildingName::ChristianCathedral:
      return "Christian Cathedral";
    case BuildingName::Citadel:
      return "Citadel";
    case BuildingName::CoalPlant:
      return "Coal Plant";
    case BuildingName::Colosseum:
      return "Colosseum";
    case BuildingName::Courthouse:
      return "Courthouse";
    case BuildingName::Drydock:
      return "Drydock";
    case BuildingName::Factory:
      return "Factory";
    case BuildingName::Forge:
      return "Forge";
    case BuildingName::Granary:
      return "Granary";
    case BuildingName::Grocer:
      return "Grocer";
    case BuildingName::Harbor:
      return "Harbor";
    case BuildingName::Hospital:
      return "Hospital";
    case BuildingName::HydroPlant:
      return "Hydro Plant";
    case BuildingName::IndustrialPark:
      return "IndustrialPark";
    case BuildingName::Jail:
      return "Jail";
    case BuildingName::Laboratory:
      return "Laboratory";
    case BuildingName::Levee:
      return "Levee";
    case BuildingName::Library:
      return "Library";
    case BuildingName::Lighthouse:
      return "Lighthouse";
    case BuildingName::Market:
      return "Market";
    case BuildingName::Monument:
      return "Mosque";
    case BuildingName::NuclearPlant:
      return "Nuclear Plant";
    case BuildingName::Observatory:
      return "Observatory";
    case BuildingName::PublicTransportation:
      return "PublicTransportation";
    case BuildingName::RecyclingCenter:
      return "Recycling Center";
    case BuildingName::SecurityBureau:
      return "SecurityBureau";
    case BuildingName::Stable:
      return "Stable";
    case BuildingName::Supermarket:
      return "Supermarket";
    case BuildingName::Theatre:
      return "Theatre";
    case BuildingName::University:
      return "University";
    case BuildingName::Walls:
      return "Walls";
    default:
      return "Unknown";
  }
}
