#include <iostream>
#include "map.h"

int main() {
    Map map;
    map.create_map();
    map.render_debug();

    std::cout << "Good Execution !";
    return 0;
}