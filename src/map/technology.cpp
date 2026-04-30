#include "technology.h"

std::map<TechnologyName, TechnologyInfo> Technology::_data {
  {TechnologyName::AdvancedFlight, {5850, {TechnologyName::Satellites, TechnologyName::Flight}}},
  {TechnologyName::Aesthetics, {351, {TechnologyName::Writing}}},
  {TechnologyName::Agriculture, {70, {}}},
  {TechnologyName::Alphabet, {351, {TechnologyName::Writing}}},
  {TechnologyName::AnimalHusbandry, {117, {TechnologyName::Hunting}}},
  {TechnologyName::Archery, {70, {TechnologyName::Hunting}}},
  {TechnologyName::Artillery, {4680, {TechnologyName::Physics, TechnologyName::Steel, TechnologyName::Rifling}}},
  {TechnologyName::AssemblyLine, {5850, {TechnologyName::Corporation, TechnologyName::SteamPower}}},
  {TechnologyName::Astronomy, {2340, {TechnologyName::Calendar, TechnologyName::Optics}}},
  {TechnologyName::Banking, {819, {TechnologyName::Currency, TechnologyName::Guilds}}},
  {TechnologyName::Biology, {4212, {TechnologyName::Chemistry, TechnologyName::ScientificMethod}}},
  {TechnologyName::BronzeWorking, {140, {TechnologyName::Mining}}},
  {TechnologyName::Calendar, {409, {TechnologyName::Sailing, TechnologyName::Mathematics}}},
  {TechnologyName::Chemistry, {2106, {TechnologyName::Engineering, TechnologyName::Gunpowder}}},
  {TechnologyName::CivilService, {936, {TechnologyName::Mathematics, TechnologyName::Feudalism}}},
  {TechnologyName::CodeOfLaws, {409, {TechnologyName::Writing, TechnologyName::Priesthood}}},
  {TechnologyName::Combustion, {4212, {TechnologyName::Railroad}}},
  {TechnologyName::Communism, {3276, {TechnologyName::Liberalism, TechnologyName::ScientificMethod}}},
  {TechnologyName::Compass, {468, {TechnologyName::Sailing, TechnologyName::IronWorking}}},
  {TechnologyName::Composites, {8775, {TechnologyName::Satellites, TechnologyName::Plastics}}},
  {TechnologyName::Computers, {7605, {TechnologyName::Plastics, TechnologyName::Radio}}},
  {TechnologyName::Constitution, {2340, {TechnologyName::CodeOfLaws, TechnologyName::Nationalism}}},
  {TechnologyName::Construction, {409, {TechnologyName::Masonry, TechnologyName::Mathematics}}},
  {TechnologyName::Corporation, {1872, {TechnologyName::Constitution, TechnologyName::Economics}}},
  {TechnologyName::Currency, {468, {TechnologyName::Mathematics, TechnologyName::Alphabet}}},
  {TechnologyName::Democracy, {3276, {TechnologyName::PrintingPress, TechnologyName::Constitution}}},
  {TechnologyName::DivineRight, {1404, {TechnologyName::Theology, TechnologyName::Monarchy}}},
  {TechnologyName::Drama, {351, {TechnologyName::Aesthetics}}},
  {TechnologyName::Ecology, {645, {TechnologyName::Biology, TechnologyName::Fission}}},
  {TechnologyName::Economics, {1638, {TechnologyName::Banking, TechnologyName::Education}}},
  {TechnologyName::Education, {2106, {TechnologyName::Paper}}},
  {TechnologyName::Electricity, {5265, {TechnologyName::Physics}}},
  {TechnologyName::Engineering, {1170, {TechnologyName::Machinery, TechnologyName::Construction}}},
  {TechnologyName::Fascism, {2808, {TechnologyName::Nationalism, TechnologyName::AssemblyLine}}},
  {TechnologyName::Feudalism, {819, {TechnologyName::Writing, TechnologyName::Monarchy}}},
  {TechnologyName::FiberOptics, {8775, {TechnologyName::Computers, TechnologyName::Laser}}},
  {TechnologyName::Fishing, {46, {}}},
  {TechnologyName::Fission, {6435, {TechnologyName::Electricity}}},
  {TechnologyName::Flight, {5850, {TechnologyName::Physics, TechnologyName::Combustion}}},
  {TechnologyName::Fusion, {9360, {TechnologyName::Fission, TechnologyName::FiberOptics}}},
  {TechnologyName::FutureTech, {11700, {TechnologyName::Stealth, TechnologyName::Genetics}}},
  {TechnologyName::Genetics, {8190, {TechnologyName::Medicine, TechnologyName::Superconductors}}},
  {TechnologyName::Guilds, {1170, {TechnologyName::Feudalism, TechnologyName::Machinery}}},
  {TechnologyName::Gunpowder, {1404, {TechnologyName::Guilds}}},
  {TechnologyName::HorsebackRiding, {292, {TechnologyName::AnimalHusbandry}}},
  {TechnologyName::Hunting, {46, {}}},
  {TechnologyName::Industrialism, {7605, {TechnologyName::AssemblyLine}}},
  {TechnologyName::IronWorking, {234, {TechnologyName::BronzeWorking}}},
  {TechnologyName::Laser, {8190, {TechnologyName::Plastics, TechnologyName::Satellites}}},
  {TechnologyName::Liberalism, {1638, {TechnologyName::Philosophy, TechnologyName::Education}}},
  {TechnologyName::Literature, {234, {TechnologyName::Polytheism, TechnologyName::Aesthetics}}},
  {TechnologyName::Machinery, {819, {TechnologyName::MetalCasting}}},
  {TechnologyName::Masonry, {93, {TechnologyName::Mining}}},
  {TechnologyName::MassMedia, {4212, {TechnologyName::Radio}}},
  {TechnologyName::Mathematics, {292, {TechnologyName::Writing}}},
  {TechnologyName::Medicine, {5265, {TechnologyName::Optics, TechnologyName::Biology}}},
  {TechnologyName::Meditation, {93, {TechnologyName::Mysticism}}},
  {TechnologyName::MetalCasting, {526, {TechnologyName::Pottery, TechnologyName::BronzeWorking}}},
  {TechnologyName::MilitaryScience, {2340, {TechnologyName::Chemistry}}},
  {TechnologyName::MilitaryTradition, {2340, {TechnologyName::Music, TechnologyName::Nationalism}}},
  {TechnologyName::Mining, {58, {}}},
  {TechnologyName::Monarchy, {351, {TechnologyName::Priesthood}}},
  {TechnologyName::Monotheism, {140, {TechnologyName::Masonry, TechnologyName::Polytheism}}},
  {TechnologyName::Music, {702, {TechnologyName::Mathematics, TechnologyName::Literature}}},
  {TechnologyName::Mysticism, {58, {}}},
  {TechnologyName::Nationalism, {2106, {TechnologyName::CivilService, TechnologyName::DivineRight}}},
  {TechnologyName::Optics, {702, {TechnologyName::Machinery, TechnologyName::Compass}}},
  {TechnologyName::Paper, {702, {TechnologyName::Theology, TechnologyName::CivilService}}},
  {TechnologyName::Philosophy, {936, {TechnologyName::Meditation, TechnologyName::Drama}}},
  {TechnologyName::Physics, {4680, {TechnologyName::Astronomy, TechnologyName::ScientificMethod}}},
  {TechnologyName::Plastics, {8190, {TechnologyName::Combustion, TechnologyName::Industrialism}}},
  {TechnologyName::Polytheism, {117, {TechnologyName::Mysticism}}},
  {TechnologyName::Pottery, {93, {TechnologyName::TheWheel}}},
  {TechnologyName::Priesthood, {70, {TechnologyName::Meditation}}},
  {TechnologyName::PrintingPress, {1872, {TechnologyName::Machinery, TechnologyName::Alphabet, TechnologyName::Paper}}},
  {TechnologyName::Radio, {7020, {TechnologyName::Electricity}}},
  {TechnologyName::Railroad, {5265, {TechnologyName::SteamPower, TechnologyName::Steel}}},
  {TechnologyName::Refrigeration, {4680, {TechnologyName::Biology, TechnologyName::Electricity}}},
  {TechnologyName::ReplaceableParts, {2106, {TechnologyName::Banking, TechnologyName::PrintingPress}}},
  {TechnologyName::Rifling, {2808, {TechnologyName::Gunpowder, TechnologyName::ReplaceableParts}}},
  {TechnologyName::Robotics, {9360, {TechnologyName::Computers}}},
  {TechnologyName::Rocketry, {5850, {TechnologyName::Rifling, TechnologyName::Flight}}},
  {TechnologyName::Sailing, {117, {TechnologyName::Fishing}}},
  {TechnologyName::Satellites, {7020, {TechnologyName::Radio, TechnologyName::Rocketry}}},
  {TechnologyName::ScientificMethod, {2808, {TechnologyName::PrintingPress, TechnologyName::Chemistry}}},
  {TechnologyName::Stealth, {9360, {TechnologyName::Composites, TechnologyName::AdvancedFlight}}},
  {TechnologyName::SteamPower, {3744, {TechnologyName::Chemistry, TechnologyName::ReplaceableParts}}},
  {TechnologyName::Steel, {3276, {TechnologyName::IronWorking, TechnologyName::Chemistry}}},
  {TechnologyName::Superconductors, {7605, {TechnologyName::Refrigeration, TechnologyName::Computers}}},
  {TechnologyName::TheWheel, {70, {}}},
  {TechnologyName::Theology, {585, {TechnologyName::Writing, TechnologyName::Monotheism}}},
  {TechnologyName::Writing, {140, {TechnologyName::Priesthood, TechnologyName::AnimalHusbandry}}}
};

std::string Technology::get_technology_name_str(TechnologyName name) {
  switch (name) {
    case TechnologyName::AdvancedFlight:    return "Advanced Flight";
    case TechnologyName::Aesthetics:         return "Aesthetics";
    case TechnologyName::Agriculture:        return "Agriculture";
    case TechnologyName::Alphabet:           return "Alphabet";
    case TechnologyName::AnimalHusbandry:    return "Animal Husbandry";
    case TechnologyName::Archery:            return "Archery";
    case TechnologyName::Artillery:          return "Artillery";
    case TechnologyName::AssemblyLine:       return "Assembly Line";
    case TechnologyName::Astronomy:          return "Astronomy";
    case TechnologyName::Banking:            return "Banking";
    case TechnologyName::Biology:            return "Biology";
    case TechnologyName::BronzeWorking:      return "Bronze Working";
    case TechnologyName::Calendar:           return "Calendar";
    case TechnologyName::Chemistry:          return "Chemistry";
    case TechnologyName::CivilService:       return "Civil Service";
    case TechnologyName::CodeOfLaws:         return "Code of Laws";
    case TechnologyName::Combustion:         return "Combustion";
    case TechnologyName::Communism:          return "Communism";
    case TechnologyName::Compass:            return "Compass";
    case TechnologyName::Composites:         return "Composites";
    case TechnologyName::Computers:          return "Computers";
    case TechnologyName::Constitution:       return "Constitution";
    case TechnologyName::Construction:       return "Construction";
    case TechnologyName::Corporation:        return "Corporation";
    case TechnologyName::Currency:           return "Currency";
    case TechnologyName::Democracy:          return "Democracy";
    case TechnologyName::DivineRight:        return "Divine Right";
    case TechnologyName::Drama:              return "Drama";
    case TechnologyName::Ecology:            return "Ecology";
    case TechnologyName::Economics:          return "Economics";
    case TechnologyName::Education:          return "Education";
    case TechnologyName::Electricity:        return "Electricity";
    case TechnologyName::Engineering:        return "Engineering";
    case TechnologyName::Fascism:            return "Fascism";
    case TechnologyName::Feudalism:          return "Feudalism";
    case TechnologyName::FiberOptics:        return "Fiber Optics";
    case TechnologyName::Fishing:            return "Fishing";
    case TechnologyName::Fission:            return "Fission";
    case TechnologyName::Flight:             return "Flight";
    case TechnologyName::Fusion:             return "Fusion";
    case TechnologyName::FutureTech:         return "Future Tech";
    case TechnologyName::Genetics:           return "Genetics";
    case TechnologyName::Guilds:             return "Guilds";
    case TechnologyName::Gunpowder:          return "Gunpowder";
    case TechnologyName::HorsebackRiding:    return "Horseback Riding";
    case TechnologyName::Hunting:            return "Hunting";
    case TechnologyName::Industrialism:      return "Industrialism";
    case TechnologyName::IronWorking:        return "Iron Working";
    case TechnologyName::Laser:              return "Laser";
    case TechnologyName::Liberalism:         return "Liberalism";
    case TechnologyName::Literature:         return "Literature";
    case TechnologyName::Machinery:          return "Machinery";
    case TechnologyName::Masonry:            return "Masonry";
    case TechnologyName::MassMedia:          return "Mass Media";
    case TechnologyName::Mathematics:        return "Mathematics";
    case TechnologyName::Medicine:           return "Medicine";
    case TechnologyName::Meditation:         return "Meditation";
    case TechnologyName::MetalCasting:       return "Metal Casting";
    case TechnologyName::MilitaryScience:    return "Military Science";
    case TechnologyName::MilitaryTradition:  return "Military Tradition";
    case TechnologyName::Mining:             return "Mining";
    case TechnologyName::Monarchy:           return "Monarchy";
    case TechnologyName::Monotheism:         return "Monotheism";
    case TechnologyName::Music:              return "Music";
    case TechnologyName::Mysticism:          return "Mysticism";
    case TechnologyName::Nationalism:        return "Nationalism";
    case TechnologyName::Optics:             return "Optics";
    case TechnologyName::Paper:              return "Paper";
    case TechnologyName::Philosophy:         return "Philosophy";
    case TechnologyName::Physics:            return "Physics";
    case TechnologyName::Plastics:           return "Plastics";
    case TechnologyName::Polytheism:         return "Polytheism";
    case TechnologyName::Pottery:            return "Pottery";
    case TechnologyName::Priesthood:         return "Priesthood";
    case TechnologyName::PrintingPress:      return "Printing Press";
    case TechnologyName::Radio:              return "Radio";
    case TechnologyName::Railroad:           return "Railroad";
    case TechnologyName::Refrigeration:      return "Refrigeration";
    case TechnologyName::ReplaceableParts:   return "Replaceable Parts";
    case TechnologyName::Rifling:            return "Rifling";
    case TechnologyName::Robotics:           return "Robotics";
    case TechnologyName::Rocketry:           return "Rocketry";
    case TechnologyName::Sailing:            return "Sailing";
    case TechnologyName::Satellites:         return "Satellites";
    case TechnologyName::ScientificMethod:   return "Scientific Method";
    case TechnologyName::Stealth:            return "Stealth";
    case TechnologyName::SteamPower:         return "Steam Power";
    case TechnologyName::Steel:              return "Steel";
    case TechnologyName::Superconductors:    return "Superconductors";
    case TechnologyName::TheWheel:           return "The Wheel";
    case TechnologyName::Theology:           return "Theology";
    case TechnologyName::Writing:            return "Writing";
    default:                                 return "Unknown Technology";
  }
}
