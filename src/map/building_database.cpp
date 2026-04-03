#include "building_database.h"

/**
 * @brief Rendements et bonus apportés par chaque aménagement de terrain.
 * Les valeurs sont cumulées au rendement naturel de la case (Plaine, Colline,
 * etc.).
 */
const std::map<BuildingName, Yields> BuildingDatabase::_data = {
    {BuildingName::None, Yields{0, 0, 0, 0}},
    {BuildingName::Workshop, Yields{-1, 1, 0, 0}},  // -1 Food, +1 Prod (Base)
    {BuildingName::WhalingBoats, Yields{0, 1, 1, 0}},  // Sur Baleine
    {BuildingName::FishingBoats, Yields{1, 0, 0, 0}},  // Sur Poisson/Coquillage
    {BuildingName::Camp,
     Yields{0, 0, 0, 0}},  // Active les ressources (Ivoire/Cerf)
    {BuildingName::Quarry, Yields{0, 1, 0, 0}},     // +1 Prod (Marbre/Pierre)
    {BuildingName::Cottage, Yields{0, 0, 1, 0}},    // +1 Commerce
    {BuildingName::Winery, Yields{0, 0, 1, 0}},     // +1 Commerce (Sur Vin)
    {BuildingName::Farm, Yields{1, 0, 0, 0}},       // +1 Food
    {BuildingName::Fort, Yields{0, 0, 0, 0.25}},    // +25% Défense
    {BuildingName::Hamlet, Yields{0, 0, 2, 0}},     // +2 Commerce
    {BuildingName::Mine, Yields{0, 2, 0, 0}},       // +2 Production
    {BuildingName::Watermill, Yields{0, 1, 0, 0}},  // +1 Prod
    {BuildingName::Windmill, Yields{1, 0, 1, 0}},   // +1 Food, +1 Commerce
    {BuildingName::Plantation,
     Yields{0, 0, 0, 0}},  // Active les ressources (Soie/Epices)
    {BuildingName::OffshorePlatform,
     Yields{0, 1, 0, 0}},                      // +1 Prod (Sur Pétrole marin)
    {BuildingName::Well, Yields{0, 0, 0, 0}},  // Active le pétrole
    {BuildingName::Pasture,
     Yields{0, 0, 0, 0}},  // Active les ressources (Vache/Mouton)
    {BuildingName::Ruins, Yields{0, 0, 0, 0}},
    {BuildingName::ForestPreserve, Yields{0, 0, 1, 0}},  // +1 Commerce
    {BuildingName::LumberMill, Yields{0, 1, 0, 0}},      // +1 Prod
    {BuildingName::Village, Yields{0, 0, 3, 0}},         // +3 Commerce
    {BuildingName::TribalVillage, Yields{0, 0, 0, 0}}};
