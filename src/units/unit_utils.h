#pragma once
#include <map>
#include <string>

#include "units.h"

namespace UnitParser {

UnitName string_to_unit_name(const std::string& name);
StrengthWeaknessMatrix load_strength_weakness_matrix();

}  // namespace UnitParser