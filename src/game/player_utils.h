#pragma once

#include <vector>

#include "player.h"

namespace PlayerParser {

Country choice_country(const std::vector<Country>& excluded_countries);

}