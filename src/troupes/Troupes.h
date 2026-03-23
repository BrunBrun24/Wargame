#include <iostream>

enum Equipe {
    Default,
    Rouge,
    Bleu
};

class Troupes{
    public:
        Troupes(int pv, int vitesse, int force, int def):_pv(pv),_vitesse(vitesse),_force(force),_def(def),_portee(0),_ekip(Equipe(Default)){};
        ~Troupes(){}
        void attaque(Troupes *ennemy){
            if (ennemy->get_ekip() != _ekip){
                ennemy->change_pv(_force);
                this->change_pv(ennemy->get_force());
                if (ennemy->get_pv() <= 0){
                    delete ennemy;
                }
                if (_pv <= 0){
                    delete this;
                }
            }
        }

        Equipe get_ekip() {return _ekip;}
        int get_pv(){return _pv;}
        int get_force(){return _force;}
        void change_pv(int atq) {_pv -= atq;}

    private:
        Equipe _ekip;
        int _pv; 
        int _vitesse; //Hexagone/tour => debuff par terrain
        int _force; 
        int _def; 
        int _portee ; //Portée d'attaque de Troupes
    
};

class Terrestre : public Troupes{
    public:
        Terrestre(int pv, int vitesse, int force, int def):Troupes(pv,vitesse,force,def){};
    private:

};


class Maritime : public Troupes{
    public:
        Maritime(int pv, int vitesse, int force, int def):Troupes(pv,vitesse,force,def){};
    private:
};

class Aerienne : public Troupes{
    public:
        Aerienne(int pv, int vitesse, int force, int def):Troupes(pv,vitesse,force,def){};
    private:
};