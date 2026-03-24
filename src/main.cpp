#include <iostream>
#include "terrestrial.h"

int main() {
    Terrestrial Ter = Terrestrial(UnitName::Destroyer,Country::France, 2, 2, 2, 2, 2);
    std::cout << "Good Execution !";
    return 0;
}