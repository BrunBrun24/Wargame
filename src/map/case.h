#include vector
#include terrains
#include Unit

class Case {
    private:
        vector<Case> cases; // Cases adjacentes
        vector<Unit> units; // Unité sur la case
        Terrains terrains;
};