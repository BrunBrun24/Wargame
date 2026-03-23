#include <iostream>
#include <vector>


enum Equipe {
    Default,
    Rouge,
    Bleu
};

enum Allowed_terrain{//A changer pour quand j'aurai Enum class venant de terrain
    Water,
    Mountain,
    Desert,
    Snow,
    Coast,
    Grassland
};

class Troupes{
    public:
        Troupes(int pv, int vitesse, int force, int def):_pv(pv),_vitesse(vitesse),_force(force),_def(def),_portee(0),_ekip(Default){};
        ~Troupes(){}
        void attaque(Troupes *ennemy){}

        Equipe get_ekip() {return _ekip;}
        int get_pv(){return _pv;}
        int get_force(){return _force;}
        void change_pv(int atq) {_pv -= atq;}
        int get_def(){return _def;}
    private:
        Equipe _ekip;
        int _pv; 
        int _vitesse; //Hexagone/tour => debuff par terrain
        int _force; 
        int _def; 
        int _portee ; //Portée d'attaque de Troupes
    protected:
        std::vector<Allowed_terrain> Allow_terrain; // 
        void set_allow_terrain(std::vector<Allowed_terrain> terrains) 
        {Allow_terrain = terrains;};
};

class Terrestre : public Troupes{
    public:
        Terrestre(int pv, int vitesse, int force, int def):Troupes(pv,vitesse,force,def)
        {set_allow_terrain({Desert,Snow,Coast,Grassland});};
    private:
};


class Maritime : public Troupes{
    public:
        Maritime(int pv, int vitesse, int force, int def):Troupes(pv,vitesse,force,def)
        {set_allow_terrain({Water});};
    private:
};

class Aerienne : public Troupes{
    public:
        Aerienne(int pv, int vitesse, int force, int def):Troupes(pv,vitesse,force,def)
        {set_allow_terrain({Desert,Snow,Coast,Grassland,Water});};
    private:
};