#pragma once
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include "ScreenUnitStatsActions/UnitControlPanel.h"

#include "map.h"
#include "game.h"

using RGB = std::array<float, 3>;
extern const std::map<Country, RGB> COULEURS_PAYS_OPENGL;
class UnitControlPanel;

class MyGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    MyGLWidget(QWidget* parent = nullptr);

    void dessinerDemiCerclePlein(float cx, float cy, float radius, float aspect, float directionRad);
    void dessinerArmored(float cx, float cy, float radius, float aspect);
    void dessinerTriangleAir(float cx, float cy, float radius, float aspect);
    void dessinerLosangeGunpowder(float cx, float cy, float radius, float aspect);
    void dessinerHelicopter(float cx, float cy, float radius, float aspect);
    void dessinerSiege(float cx, float cy, float radius, float aspect);
    void dessinerTriangleMelee(float cx, float cy, float radius, float aspect);
    void dessinerMounted(float cx, float cy, float radius, float aspect);
    void dessinerMissile(float cx, float cy, float radius, float aspect);
    void dessinerRecon(float cx, float cy, float radius, float aspect); 
    
    QString unitTypeToString(UnitType name);

    void setMapPtr(Map* map) {
        qDebug() << "MyGLWidget : setMapPtr appelé avec l'adresse :" << map;
        _mapData = map; 
        update(); // Force OpenGL à redessiner avec les nouvelles données
    }
protected:
    void renderGame(float aspect);
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
    void keyPressEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    QString countryToString(Country c);
    
    private:
    UnitControlPanel* _unitControl = nullptr;
    int Nombre_Hexagone_Hauteur;
    int Nombre_Hexagone_Largeur;
    Map* _mapData = nullptr; // Pointeur vers la logique de jeu
    bool modeTestCouleurs = false;
    void genererMapDeTest();
    UnitControlPanel* _controlPanel;
};
