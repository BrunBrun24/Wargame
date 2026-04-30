#include "MyGLWidget.h"
#include <QOpenGLFunctions>
#include <QKeyEvent>
#include <map>
#include <array>
#include "../type.h"
#include "../units/units.h"
#include "DebugGfx.h"
// On définit un alias pour plus de clarté
using RGB = std::array<float, 3>;

const std::map<Country, RGB> COULEURS_PAYS_OPENGL = {
    {Country::Neutral,       {0.0f, 0.0f, 0.0f}}, 
    {Country::France,        {0.01953125f, 0.22265625f, 0.26953125f}}, 
    {Country::Germany,       {0.3671875f, 0.23828125f, 0.046875f}}, 
    {Country::UnitedKingdom, {0.578125f, 0.54296875f, 0.54296875f}},
    {Country::Russia,        {1.0f, 1.0f, 1.0f}},
    {Country::Egypt,         {0.85f, 0.65f, 0.13f}}, 
    {Country::Switzerland,   {1.0f, 0.0f, 0.0f}},    
    {Country::Japan,         {1.0f, 0.71f, 0.76f}}, 
    {Country::Spain,         {0.87109375f, 0.2734375f, 0.90625f}},    
    {Country::Italy,         {0.12890625f, 0.30078125f, 0.02734375f}}, 
    {Country::UnitedStates,  {0.42578125f, 0.1015625f, 0.85546875f}}     
};
const std::vector<UnitType> listeTypes = {
        
    };



MyGLWidget::MyGLWidget(QWidget* parent) : QOpenGLWidget(parent) {
    setFocusPolicy(Qt::StrongFocus); // Pour le clavier
    setMouseTracking(true);
    
    this->setLayout(new QGridLayout(this));
}

void MyGLWidget::initializeGL() {
    //initialisation de GL
    initializeOpenGLFunctions();
    //Juste pour du choix basique
    int red[3] = {255,0,0};
    int green[3] = {0,255,0};
    int blue[3] = {0,0,255};
    int choix1[3] = {139, 247, 246};
    int choix2[3] = {189, 17, 172};

    glClearColor(1.0f,1.0f,1.0f,1.0f);

}

void MyGLWidget::resizeGL(int w, int h) {
    glViewport(0, 0, w-100, h-100);
}

void MyGLWidget::paintGL() {
    // 1. Nettoyage de l'écran (Commun à tous les modes)
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    if (_mapData == nullptr) return;

    // 2. Calcul du ratio d'aspect
    float aspect = (float)width() / (float)height();

    // 3. Choix du mode de rendu
    if (this->modeTestCouleurs) { // Si tu as appuyé sur 'T'
        // On utilise le fichier de debug pour afficher la matrice Pays/Unités
        DebugGfx::afficherMatriceTest(this, aspect);
    } else {
        // On utilise le rendu normal du jeu
        renderGame(aspect);
    }
}

void MyGLWidget::renderGame(float aspect) {
    bool contour = true;
    bool pays = false;
    float r,g,b;
    if (_mapData == nullptr) {
        return; // On sort de la fonction si la map n'est pas encore là
    }
    static bool firstDraw = true;
    if (firstDraw) {
        qDebug() << "MyGLWidget : Premier rendu paintGL commencé avec succès.";
        firstDraw = false;
    }
   // Paramètres de ta grille
    Nombre_Hexagone_Largeur = 44; // Nombre d'hexagones en largeur
    Nombre_Hexagone_Hauteur = 28;  // Nombre d'hexagones en hauteur
    glClear(GL_COLOR_BUFFER_BIT);

    // 1. Gérer le ratio de la fenêtre pour éviter l'écrasement

    // 2. Calculer le rayon pour que ça remplisse l'écran
    // On calcule l'espace total nécessaire en unités "logiques"
    float totalLogicalW = sqrt(3.0f) * (Nombre_Hexagone_Largeur + 0.5f);
    float totalLogicalH = 1.5f * Nombre_Hexagone_Hauteur + 0.5f;

    // Le rayon est adapté à la dimension la plus restrictive
    float radiusX = 2.0f * aspect / totalLogicalW;
    float radiusY = 2.0f / totalLogicalH;
    float radius = std::min(radiusX, radiusY);

    float dx = radius * sqrt(3.0f);
    float dy = 1.5f * radius;

    // 3. Centrer la grille
    float gridWidth = dx * (Nombre_Hexagone_Largeur - 0.5f);
    float gridHeight = dy * (Nombre_Hexagone_Hauteur - 1) + 2 * radius;
    
    // On commence en haut à gauche, ajusté par l'aspect ratio
    float startX = (-gridWidth / 2.0f); 
    float startY = (gridHeight / 2.0f) - radius;

    glLineWidth(2.0f);

    for (int row = 0; row < Nombre_Hexagone_Hauteur; ++row) {
    
    float cy = startY - row * dy;
    const std::vector<Case>& ligne = _mapData->get_cases().at(row);

    for (int col = 0; col < Nombre_Hexagone_Largeur; ++col) {
        const Case& case_actuelle = ligne.at(col);
        float cx = startX + col * dx;
        pays = false;
        
        if (row % 2 == 1) cx += dx / 2.0f;
        // --- 1. DÉTERMINER LA COULEUR DE REMPLISSAGE ---
        Country pays_proprietaire = case_actuelle.get_country();
        // /!/!/surement a changer par un switch car après c'est juste la même chose
        if (COULEURS_PAYS_OPENGL.count(pays_proprietaire) && pays_proprietaire != Country::Neutral) {
            // Si la case appartient à un pays (et n'est pas neutre), on prend sa couleur
            const RGB& rgb = COULEURS_PAYS_OPENGL.at(pays_proprietaire);
            pays = true;
            // OPTIONNEL : On peut éclaircir un peu la couleur pour le remplissage 
            // pour que ce soit moins agressif que le contour
            glColor3f(rgb[0], rgb[1], rgb[2]); 
        } 
        else if(case_actuelle.get_terrain().type == TerrainsType::Coast){
            glColor3f(0.01953125f, 0.9765625f, 0.75390625f);
        }
        else if(case_actuelle.get_terrain().type == TerrainsType::Desert){
            glColor3f(0.99609375f, 0.92578125f, 0.30078125f);
        }
        else if(case_actuelle.get_terrain().type == TerrainsType::Grassland){
            
            r = 0.140625f;
            g = 0.828125f;
            b = 0.125f;
            if (case_actuelle.get_terrain().feature == TerrainFeature::Forest){
                r = r * 1.2f;
                g = g * 1.2f;
                b = b * 1.2f;
            }
            else if(case_actuelle.get_terrain().feature == TerrainFeature::Jungle){
                r = r * 0.8f;
                g = g * 0.8f;
                b = b * 0.8f;               
            }
            glColor3f(r, g, b);

        }
        else if(case_actuelle.get_terrain().type == TerrainsType::Ice){
            glColor3f(0.6796875f, 0.95703125f, 0.921875f);
        }
        else if (case_actuelle.get_terrain().type == TerrainsType::Ocean) {
            // Si c'est l'océan, on garde le bleu
            glColor3f(0.15f, 0.45f, 0.85f); 
        } 
        else if(case_actuelle.get_terrain().type == TerrainsType::Plains){
            r = 0.63671875f;
            g = 0.99609375f;
            b = 0.48828125f;
            if(case_actuelle.get_terrain().feature == TerrainFeature::Forest){
                r = r * 1.2f;
                g = g * 1.2f;
                b = b * 1.2f;
            }
            else if(case_actuelle.get_terrain().feature == TerrainFeature::Jungle){
                r = r * 0.8f;
                g = g * 0.8f;
                b = b * 0.8f;                
            }
            glColor3f(r, g, b);
        }
        else if(case_actuelle.get_terrain().type == TerrainsType::Tundra){
            glColor3f(0.9453125f, 0.68359375f, 0.21875f);
        }
        else {
            // Case de terre neutre
            glColor3f(0.9f, 0.85f, 0.75f); 
        }

        // --- 2. DESSIN DU CORPS DE L'HEXAGONE ---
        glBegin(GL_POLYGON);
        for (int j = 0; j < 6; ++j) {
            float angle = M_PI / 2.0f - j * M_PI / 3.0f;
            glVertex2f((cx + radius * cos(angle)) / aspect, cy + radius * sin(angle));
        }
        glEnd();

        // /!\ Dessin des troupes : On prend que la première troupe
        if (!case_actuelle.get_units().empty()){
            Country pays_proprietaire_troupe = case_actuelle.get_units().at(0)->get_player()->get_country();
            const RGB& couleurs_troupe = COULEURS_PAYS_OPENGL.at(pays_proprietaire_troupe);
            glColor3f(couleurs_troupe[0], couleurs_troupe[1], couleurs_troupe[2]); 

            switch(UNIT_TYPE.at(case_actuelle.get_units().at(0)->get_name())){
                case UnitType::Air:
                    dessinerTriangleAir(cx, cy, radius, aspect);
                    break;

                case UnitType::Archery:
                    dessinerDemiCerclePlein(cx, cy, radius, aspect, 0.0f);
                    break;

                case UnitType::Armored:
                    dessinerArmored(cx, cy, radius, aspect);
                    break;

                case UnitType::Gunpowder:
                    dessinerLosangeGunpowder(cx, cy, radius, aspect);
                    break;

                case UnitType::Helicopter:
                    dessinerHelicopter(cx,cy,radius,aspect);
                    break;

                case UnitType::Melee:
                    dessinerTriangleMelee(cx, cy, radius, aspect);
                    break;

                case UnitType::Mounted:
                    dessinerMounted(cx, cy, radius, aspect);
                    break;

                case UnitType::Naval:
                    dessinerDemiCerclePlein(cx, cy, radius, aspect, -M_PI / 2.0f);
                    break;

                case UnitType::Recon:
                    dessinerRecon(cx, cy, radius, aspect);
                    break;

                case UnitType::Siege:
                    dessinerSiege(cx, cy, radius, aspect);
                    break;

                case UnitType::Missile:
                    dessinerMissile(cx, cy, radius, aspect);
                    break;

                case UnitType::Civil:
                    dessinerDemiCerclePlein(cx, cy, radius, aspect, M_PI / 2.0f);
                    break;

                default:
                    // Optionnel : un dessin par défaut si le type est inconnu
                    qDebug() << "Type d'unité inconnu détecté";
                    break;
            }
        }
        
        // --- 3. DESSIN DU CONTOUR (GRILLE) ---
        // On dessine toujours un petit contour noir ou gris foncé 
        // pour bien séparer les cases les unes des autres
        if ((contour) || (pays)){
            glColor3f(0.0f, 0.0f, 0.0f); 
            glLineWidth(1.0f); 
            if (pays){
                glLineWidth(2.0f); 
            }
            glBegin(GL_LINE_LOOP);
            for (int j = 0; j < 6; ++j) {
                float angle = M_PI / 2.0f - j * M_PI / 3.0f;
                // On réduit très légèrement le radius (0.99) pour le contour
                // Cela crée une grille fine et évite les problèmes de recouvrement
                glVertex2f((cx + (radius * 0.99f) * cos(angle)) / aspect, cy + (radius * 0.99f) * sin(angle));
            }
            glEnd();
        }
        }
    }
}



void MyGLWidget::dessinerRecon(float cx, float cy, float radius, float aspect) {
    // 1. Paramètres (deux rectangles côte à côte)
    float futW = radius * 0.25f;  // Largeur d'un fût
    float futH = radius * 0.6f;   // Hauteur d'un fût
    float ecart = radius * 0.1f;  // Espace entre les deux

    // Positions X des deux fûts
    float xGauche = cx - (futW + ecart/2.0f);
    float xDroit = cx + (ecart/2.0f);
    float yBas = cy - (futH / 2.0f);

    // --- A. DESSIN DES DEUX FÛTS (Plein) ---
    glBegin(GL_QUADS);
        // Fût gauche
        glVertex2f(xGauche / aspect, yBas);
        glVertex2f((xGauche + futW) / aspect, yBas);
        glVertex2f((xGauche + futW) / aspect, yBas + futH);
        glVertex2f(xGauche / aspect, yBas + futH);

        // Fût droit
        glVertex2f(xDroit / aspect, yBas);
        glVertex2f((xDroit + futW) / aspect, yBas);
        glVertex2f((xDroit + futW) / aspect, yBas + futH);
        glVertex2f(xDroit / aspect, yBas + futH);
    glEnd();

    // --- B. LA BARRE DE LIAISON ---
    glLineWidth(3.0f);
    glBegin(GL_LINES);
        glVertex2f((xGauche + futW) / aspect, cy);
        glVertex2f(xDroit / aspect, cy);
    glEnd();

    // --- C. CONTOURS NOIRS ---
    glColor3f(0.0f, 0.0f, 0.0f);
    glLineWidth(1.5f);
    // Contour gauche
    glBegin(GL_LINE_LOOP);
        glVertex2f(xGauche / aspect, yBas);
        glVertex2f((xGauche + futW) / aspect, yBas);
        glVertex2f((xGauche + futW) / aspect, yBas + futH);
        glVertex2f(xGauche / aspect, yBas + futH);
    glEnd();
    // Contour droit
    glBegin(GL_LINE_LOOP);
        glVertex2f(xDroit / aspect, yBas);
        glVertex2f((xDroit + futW) / aspect, yBas);
        glVertex2f((xDroit + futW) / aspect, yBas + futH);
        glVertex2f(xDroit / aspect, yBas + futH);
    glEnd();
}
void MyGLWidget::dessinerMissile(float cx, float cy, float radius, float aspect) {
    // 1. Dimensions
    float corpsW = radius * 0.25f;  // Très étroit
    float corpsH = radius * 0.5f;   // Corps principal
    float pointeH = radius * 0.4f;  // Longue pointe
    float aileronS = radius * 0.2f; // Taille des ailerons à la base

    // Coordonnées de base
    float yBas = cy - (corpsH / 2.0f);
    float yHautCorps = yBas + corpsH;
    float yPointe = yHautCorps + pointeH;

    // --- A. CORPS ET POINTE (Plein) ---
    glBegin(GL_POLYGON);
        // Base gauche
        glVertex2f((cx - corpsW/2.0f) / aspect, yBas);
        // Base droite
        glVertex2f((cx + corpsW/2.0f) / aspect, yBas);
        // Haut corps droit
        glVertex2f((cx + corpsW/2.0f) / aspect, yHautCorps);
        // LA POINTE
        glVertex2f(cx / aspect, yPointe);
        // Haut corps gauche
        glVertex2f((cx - corpsW/2.0f) / aspect, yHautCorps);
    glEnd();

    // --- B. AILERONS (Lignes épaisses à la base) ---
    glLineWidth(3.0f);
    glBegin(GL_LINES);
        // Aileron gauche
        glVertex2f((cx - corpsW/2.0f) / aspect, yBas);
        glVertex2f((cx - corpsW/2.0f - aileronS) / aspect, yBas - aileronS/2.0f);
        // Aileron droit
        glVertex2f((cx + corpsW/2.0f) / aspect, yBas);
        glVertex2f((cx + corpsW/2.0f + aileronS) / aspect, yBas - aileronS/2.0f);
    glEnd();

    // --- C. CONTOUR NOIR ---
    glColor3f(0.0f, 0.0f, 0.0f);
    glLineWidth(1.5f);
    glBegin(GL_LINE_LOOP);
        glVertex2f((cx - corpsW/2.0f) / aspect, yBas);
        glVertex2f((cx + corpsW/2.0f) / aspect, yBas);
        glVertex2f((cx + corpsW/2.0f) / aspect, yHautCorps);
        glVertex2f(cx / aspect, yPointe);
        glVertex2f((cx - corpsW/2.0f) / aspect, yHautCorps);
    glEnd();
}
void MyGLWidget::dessinerMounted(float cx, float cy, float radius, float aspect) {
    float w = radius * 0.6f; // Largeur
    float h = radius * 0.4f; // Hauteur
    float inclinaison = radius * 0.2f; // Le décalage pour l'effet penché

    // 1. Calcul des 4 points (Parallélogramme penché vers la droite)
    float x1 = cx - w/2.0f;                float y1 = cy - h/2.0f; // Bas Gauche
    float x2 = cx + w/2.0f;                float y2 = cy - h/2.0f; // Bas Droite
    float x3 = cx + w/2.0f + inclinaison;  float y3 = cy + h/2.0f; // Haut Droite (décalé)
    float x4 = cx - w/2.0f + inclinaison;  float y4 = cy + h/2.0f; // Haut Gauche (décalé)

    // 2. Dessin du plein
    glBegin(GL_QUADS);
        glVertex2f(x1 / aspect, y1);
        glVertex2f(x2 / aspect, y2);
        glVertex2f(x3 / aspect, y3);
        glVertex2f(x4 / aspect, y4);
    glEnd();

    // 3. Contour noir
    glColor3f(0.0f, 0.0f, 0.0f);
    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);
        glVertex2f(x1 / aspect, y1);
        glVertex2f(x2 / aspect, y2);
        glVertex2f(x3 / aspect, y3);
        glVertex2f(x4 / aspect, y4);
    glEnd();

    // 4. Petit détail : Une "lance" inclinée pour confirmer le type
    glBegin(GL_LINES);
        glVertex2f(x3 / aspect, y3); 
        glVertex2f((x3 + radius * 0.3f) / aspect, y3 + radius * 0.2f);
    glEnd();
}
void MyGLWidget::dessinerTriangleMelee(float cx, float cy, float radius, float aspect) {
    float size = radius * 0.5f; // Un peu plus grand pour l'infanterie

    // 1. Calcul des coordonnées (Pointe vers le HAUT)
    // A : Bas Gauche, B : Bas Droite, C : Pointe Haute
    float xa = cx - (size * 0.7f); 
    float ya = cy - (size * 0.6f);
    
    float xb = cx + (size * 0.7f); 
    float yb = cy - (size * 0.6f);
    
    float xc = cx;                
    float yc = cy + (size * 0.8f);

    // 2. Dessin du triangle PLEIN
    glBegin(GL_TRIANGLES);
        glVertex2f(xa / aspect, ya);
        glVertex2f(xb / aspect, yb);
        glVertex2f(xc / aspect, yc);
    glEnd();

    // 3. Dessin du CONTOUR noir pour la visibilité
    glColor3f(0.0f, 0.0f, 0.0f);
    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);
        glVertex2f(xa / aspect, ya);
        glVertex2f(xb / aspect, yb);
        glVertex2f(xc / aspect, yc);
    glEnd();
}
void MyGLWidget::dessinerSiege(float cx, float cy, float radius, float aspect) {
    // 1. Paramètres de proportions
    float baseW = radius * 0.6f;     // Base un peu plus étroite que le tank
    float baseH = radius * 0.25f;    // Base assez plate
    float domeR = radius * 0.35f;    // Dôme plus imposant
    float canonL = radius * 0.8f;    // Canon très long
    float yBase = cy - (baseH / 2.0f);
    int segments = 20;

    // --- A. DESSIN DE LA BASE (Rectangle plein) ---
    glBegin(GL_QUADS);
        glVertex2f((cx - baseW/2.0f) / aspect, yBase);
        glVertex2f((cx + baseW/2.0f) / aspect, yBase);
        glVertex2f((cx + baseW/2.0f) / aspect, yBase + baseH);
        glVertex2f((cx - baseW/2.0f) / aspect, yBase + baseH);
    glEnd();

    // --- B. DESSIN DU CORPS CIRCULAIRE (Dôme) ---
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(cx / aspect, yBase + baseH); 
        for (int i = 0; i <= segments; ++i) {
            float angle = (float)i / (float)segments * M_PI;
            glVertex2f((cx + domeR * cos(angle)) / aspect, 
                       (yBase + baseH) + domeR * sin(angle));
        }
    glEnd();

    // --- C. LE LONG CANON (Incliné vers le haut) ---
    glLineWidth(4.0f); // Canon lourd
    glBegin(GL_LINES);
        glVertex2f(cx / aspect, (yBase + baseH) + domeR/2.0f);
        // On tire vers le haut à droite (angle de ~30 degrés)
        glVertex2f((cx + canonL * 0.86f) / aspect, (yBase + baseH + domeR/2.0f) + canonL * 0.5f);
    glEnd();

    // --- D. FINITIONS NOIRES (Contours) ---
    glColor3f(0.0f, 0.0f, 0.0f);
    glLineWidth(1.5f);
    
    // Contour Base
    glBegin(GL_LINE_LOOP);
        glVertex2f((cx - baseW/2.0f) / aspect, yBase);
        glVertex2f((cx + baseW/2.0f) / aspect, yBase);
        glVertex2f((cx + baseW/2.0f) / aspect, yBase + baseH);
        glVertex2f((cx - baseW/2.0f) / aspect, yBase + baseH);
    glEnd();
    
    // Contour Dôme
    glBegin(GL_LINE_STRIP);
        for (int i = 0; i <= segments; ++i) {
            float angle = (float)i / (float)segments * M_PI;
            glVertex2f((cx + domeR * cos(angle)) / aspect, 
                       (yBase + baseH) + domeR * sin(angle));
        }
    glEnd();
}
void MyGLWidget::dessinerHelicopter(float cx, float cy, float radius, float aspect) {
    float size = radius * 0.5f;

    // --- A. CONTOUR NOIR (Épaisseur plus grande) ---
    // On dessine d'abord le "squelette" en noir pour la lisibilité
    glColor3f(0.0f, 0.0f, 0.0f);
    glLineWidth(5.0f); // Plus large que la couleur finale
    glBegin(GL_LINES);
        // Pales
        glVertex2f((cx - size * 0.7f) / aspect, cy + size * 0.7f);
        glVertex2f((cx + size * 0.7f) / aspect, cy - size * 0.7f);
        glVertex2f((cx + size * 0.7f) / aspect, cy + size * 0.7f);
        glVertex2f((cx - size * 0.7f) / aspect, cy - size * 0.7f);
        // Queue
        glVertex2f(cx / aspect, cy + size * 0.2f);
        glVertex2f(cx / aspect, cy - size * 0.9f);
    glEnd();

    // --- B. COULEUR DE L'UNITÉ ---
    // On récupère la couleur définie avant l'appel (ou on la gère ici)
    // Pour cet exemple, on suppose que glColor3f a été appelé juste avant la méthode
    // Si tu veux forcer la couleur ici, il faudrait passer le Country en paramètre.
    
    glLineWidth(2.5f); // Un peu plus fin pour laisser déborder le noir
    glBegin(GL_LINES);
        glVertex2f((cx - size * 0.7f) / aspect, cy + size * 0.7f);
        glVertex2f((cx + size * 0.7f) / aspect, cy - size * 0.7f);
        glVertex2f((cx + size * 0.7f) / aspect, cy + size * 0.7f);
        glVertex2f((cx - size * 0.7f) / aspect, cy - size * 0.7f);
        glVertex2f(cx / aspect, cy + size * 0.2f);
        glVertex2f(cx / aspect, cy - size * 0.9f);
    glEnd();

    // --- C. LE ROTOR (Point central) ---
    glColor3f(0.0f, 0.0f, 0.0f);
    glPointSize(7.0f);
    glBegin(GL_POINTS);
        glVertex2f(cx / aspect, cy);
    glEnd();
}
void MyGLWidget::dessinerLosangeGunpowder(float cx, float cy, float radius, float aspect) {
    // 1. Définition des dimensions (étiré verticalement)
    float width = radius * 0.4f;   
    float height = radius * 0.7f;  

    // 2. Points du losange
    float xH = cx;               float yH = cy + height; // Haut
    float xB = cx;               float yB = cy - height; // Bas
    float xG = cx - width;        float yG = cy;          // Gauche
    float xD = cx + width;        float yD = cy;          // Droite

    // 3. Dessin du plein (Couleur définie à l'appel)
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(xH / aspect, yH);
        glVertex2f(xD / aspect, yD);
        glVertex2f(xB / aspect, yB);
        glVertex2f(xG / aspect, yG);
    glEnd();

    // 4. Contour noir
    glColor3f(0.0f, 0.0f, 0.0f);
    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);
        glVertex2f(xH / aspect, yH);
        glVertex2f(xD / aspect, yD);
        glVertex2f(xB / aspect, yB);
        glVertex2f(xG / aspect, yG);
    glEnd();
}
void MyGLWidget::dessinerTriangleAir(float cx, float cy, float radius, float aspect) {
    float size = radius * 0.4f;

    // 1. Calcul des coordonnées (Pointe vers le bas)
    float xa = cx - (size * 0.8f); 
    float ya = cy + (size * 0.5f);
    
    float xb = cx + (size * 0.8f); 
    float yb = cy + (size * 0.5f);
    
    float xc = cx;                
    float yc = cy - (size * 0.8f);

    // 2. Dessin du triangle PLEIN 
    // (La couleur doit être définie par glColor avant d'appeler la méthode)
    glBegin(GL_TRIANGLES);
        glVertex2f(xa / aspect, ya);
        glVertex2f(xb / aspect, yb);
        glVertex2f(xc / aspect, yc);
    glEnd();

    // 3. Dessin du CONTOUR noir
    glColor3f(0.0f, 0.0f, 0.0f);
    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);
        glVertex2f(xa / aspect, ya);
        glVertex2f(xb / aspect, yb);
        glVertex2f(xc / aspect, yc);
    glEnd();
}
void MyGLWidget::dessinerArmored(float cx, float cy, float radius, float aspect) {
    // 1. Paramètres de proportions
    float chassisW = radius * 0.7f;   // Largeur du rectangle
    float chassisH = radius * 0.35f;  // Hauteur du rectangle
    float tourelleR = radius * 0.22f; // Rayon du dôme
    float yBase = cy - (chassisH / 2.0f); // Décalage pour centrer l'icône
    int segments = 15;

    // --- A. DESSIN DU CHÂSSIS (Rectangle) ---
    glBegin(GL_QUADS);
        glVertex2f((cx - chassisW/2.0f) / aspect, yBase);
        glVertex2f((cx + chassisW/2.0f) / aspect, yBase);
        glVertex2f((cx + chassisW/2.0f) / aspect, yBase + chassisH);
        glVertex2f((cx - chassisW/2.0f) / aspect, yBase + chassisH);
    glEnd();

    // --- B. DESSIN DE LA TOURELLE (Demi-cercle) ---
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(cx / aspect, yBase + chassisH); // Centre sur le toit
        for (int i = 0; i <= segments; ++i) {
            float angle = (float)i / (float)segments * M_PI;
            glVertex2f((cx + tourelleR * cos(angle)) / aspect, 
                       (yBase + chassisH) + tourelleR * sin(angle));
        }
    glEnd();

    // --- C. DESSIN DU CANON (Une ligne épaisse) ---
    glLineWidth(3.0f);
    glBegin(GL_LINES);
        glVertex2f(cx / aspect, (yBase + chassisH) + tourelleR/2.0f);
        glVertex2f((cx + chassisW * 0.6f) / aspect, (yBase + chassisH) + tourelleR/2.0f);
    glEnd();

    // --- D. CONTOUR NOIR (Finitions) ---
    glColor3f(0.0f, 0.0f, 0.0f);
    glLineWidth(1.5f);
    
    // Contour Châssis
    glBegin(GL_LINE_LOOP);
        glVertex2f((cx - chassisW/2.0f) / aspect, yBase);
        glVertex2f((cx + chassisW/2.0f) / aspect, yBase);
        glVertex2f((cx + chassisW/2.0f) / aspect, yBase + chassisH);
        glVertex2f((cx - chassisW/2.0f) / aspect, yBase + chassisH);
    glEnd();
    
    // Contour Tourelle
    glBegin(GL_LINE_STRIP);
        for (int i = 0; i <= segments; ++i) {
            float angle = (float)i / (float)segments * M_PI;
            glVertex2f((cx + tourelleR * cos(angle)) / aspect, 
                       (yBase + chassisH) + tourelleR * sin(angle));
        }
    glEnd();
}
void MyGLWidget::dessinerDemiCerclePlein(float cx, float cy, float radius, float aspect, float directionRad) {
    int nbSegments = 30; // Nombre de triangles pour lisser la courbe
    
    // Pour un demi-cercle orienté, on commence à (Direction - 90°)
    float startAngle = directionRad - (M_PI / 2.0f);
    float ouverture = M_PI; // 180 degrés

    // 1. Définir la couleur avant d'appeler la fonction (ex: glColor3f)

    glBegin(GL_TRIANGLE_FAN);
        // --- LE POINT CENTRAL ---
        // Tous les triangles se rejoignent ici pour remplir la forme
        glVertex2f(cx / aspect, cy); 

        // --- LES POINTS DE L'ARC ---
        for (int i = 0; i <= nbSegments; ++i) {
            // Calcul de l'angle pour ce point précis de l'arc
            float angle = startAngle + (float)i / (float)nbSegments * ouverture;
            
            // Calcul des coordonnées X et Y
            // ATTENTION : n'oublie pas le /aspect uniquement sur le X !
            float x_point = (cx + radius * cos(angle)) / aspect;
            float y_point = cy + radius * sin(angle);
            
            glVertex2f(x_point, y_point);
        }
    glEnd();
}





void MyGLWidget::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_T) {
        qDebug() << "Destruction de la map pour test de couleurs...";
        modeTestCouleurs = true;
        genererMapDeTest();
    }
}

QString MyGLWidget::countryToString(Country c) {
    switch(c) {
        case Country::France:         return "France";
        case Country::Germany:        return "Germany";
        case Country::UnitedKingdom:  return "United Kingdom";
        case Country::Russia:         return "Russia";
        case Country::Egypt:          return "Egypt";
        case Country::Switzerland:    return "Switzerland";
        case Country::Japan:          return "Japan";
        case Country::Spain:          return "Spain";
        case Country::Italy:          return "Italy";
        case Country::UnitedStates:   return "United States";
        default:                      return "Neutral";
    }
}

void MyGLWidget::mousePressEvent(QMouseEvent *event) {
    qDebug() << "Click détécté";
    if (this->modeTestCouleurs) {
        float x = (2.0f * event->pos().x()) / width() - 1.0f;
        float y = 1.0f - (2.0f * event->pos().y()) / height();
        float aspect = (float)width() / height();


        auto clickedType = DebugGfx::getTypeAt(x, y, aspect);

        if (clickedType.has_value()) {
            qDebug() << ">>> CLIC DEBUG <<<";
            qDebug() << DebugGfx::unitTypeToString(clickedType.value());
            qDebug() << "------------------";
        }
    } else {
        qDebug() << "Click détécté hors modeTestCouleurs";
       if (_mapData == nullptr) return;

        float w = (float)width();
        float h = (float)height();
        float aspect = w / h;

        // --- 1. RECALCUL DES PARAMÈTRES DE DESSIN (DOIT ÊTRE IDENTIQUE À RENDERGAME) ---
        float totalLogicalW = sqrt(3.0f) * (Nombre_Hexagone_Largeur + 0.5f);
        float totalLogicalH = 1.5f * Nombre_Hexagone_Hauteur + 0.5f;
        float radiusX = 2.0f * aspect / totalLogicalW;
        float radiusY = 2.0f / totalLogicalH;
        float radius = std::min(radiusX, radiusY);

        float dx = radius * sqrt(3.0f);
        float dy = 1.5f * radius;

        float gridWidth = dx * (Nombre_Hexagone_Largeur - 0.5f);
        float gridHeight = dy * (Nombre_Hexagone_Hauteur - 1) + 2 * radius;
        
        // Coordonnées du centre de la grille (0,0 en OpenGL)
        float startX = (-gridWidth / 2.0f); 
        float startY = (gridHeight / 2.0f) - radius;

        // --- 2. POSITION DE LA SOURIS EN COORDONNÉES OPENGL ---
        float mouseX = ((2.0f * (float)event->x() / w) - 1.0f) * aspect;
        float mouseY = 1.0f - (2.0f * (float)event->y() / h);

        // --- 3. TRANSFORMATION POUR ALIGNER SUR L'INDEX [0][0] ---
        // On soustrait startX et startY pour que le clic soit relatif au début de la grille
        float localX = mouseX - startX;
        float localY = mouseY - startY;

        // --- 4. CONVERSION EN COORDONNÉES DE GRILLE ---
        // On utilise la formule inverse de la disposition "Pointy-topped"
        float r_frac = -localY / dy;
        
        // Ajustement pour le décalage horizontal des lignes impaires (row % 2 == 1)
        int row_check = std::round(r_frac);
        float offset = (row_check % 2 != 0) ? (dx / 2.0f) : 0.0f;
        float q_frac = (localX - offset) / dx;

        // --- 5. ARRONDIS ET RÉCUPÉRATION ---
        int finalR = std::round(r_frac);
        int finalQ = std::round(q_frac);

        // Debug pour vérifier les index
        qDebug() << "Index calculé : [" << finalR << "][" << finalQ << "]";

        auto& allCases = _mapData->get_cases();
        if (finalR >= 0 && finalR < (int)allCases.size()) {
            if (finalQ >= 0 && finalQ < (int)allCases[finalR].size()) {
                Case* clickedCase = const_cast<Case*>(&(allCases.at(finalR).at(finalQ)));

                if (_unitControl) { delete _unitControl; _unitControl = nullptr; }

                if (clickedCase && !clickedCase->get_units().empty()) {
                    const Unit* targetUnit = clickedCase->get_units()[0];
                    _unitControl = new UnitControlPanel(this, targetUnit, clickedCase);
                }
            }
        }
        update();
    }

}
void MyGLWidget::genererMapDeTest() {
    if (!_mapData) return;

    // 1. On récupère un joueur valide pour éviter le crash (Segmentation Fault)
    // On suppose que ta structure _mapData contient une liste de joueurs
    if (_mapData->get_players().empty()) {
        std::cout << "Erreur : Aucun joueur n'est défini dans la map pour le debug." << std::endl;
        return;
    }
    Player* debugPlayer = _mapData->get_players().at(0);

    // 2. Liste des pays pour les lignes
    std::vector<Country> listePays = {
        Country::France, Country::Germany, Country::UnitedKingdom, 
        Country::Russia, Country::Egypt, Country::Switzerland, 
        Country::Japan, Country::Spain, Country::Italy, Country::UnitedStates
    };

    // 3. Liste des noms d'unités pour les colonnes (UnitName car requis par le constructeur)
    std::vector<UnitName> listeNames = {
        UnitName::Settler, UnitName::Warrior, UnitName::Archer, UnitName::Scout,
        UnitName::Chariot, UnitName::Catapult, UnitName::Tank, UnitName::Fighter
    };

    for (int row = 0; row < Nombre_Hexagone_Hauteur; ++row) {
        for (int col = 0; col < Nombre_Hexagone_Largeur; ++col) {
            
            // Récupération de la case
            Case& c = const_cast<Case&>(_mapData->get_cases().at(row).at(col));

            // --- NETTOYAGE DES UNITÉS EXISTANTES ---
            // On récupère une copie de la liste pour itérer sans problème
            std::vector<Unit*> currentUnits = c.get_units();
            for (Unit* u : currentUnits) {
                c.remove_unit(u);
                // Note: ne fais 'delete u' que si ton remove_unit ne le fait pas déjà
                // delete u; 
            }

            // --- CONFIGURATION DU TERRAIN ---
            c.get_terrain().type = TerrainsType::Plains;
            c.get_terrain().feature = TerrainFeature::None;
            c.get_terrain().elevation = TerrainElevation::Flat;

            // --- REMPLISSAGE DE LA MATRICE DE DEBUG ---
            if (row < (int)listePays.size() && col < (int)listeNames.size()) {
                
                Country paysLigne = listePays.at(row);
                UnitName nomCol = listeNames.at(col);

                // On définit le pays de la case (pour le fond coloré de l'hexagone)
                c.set_country(paysLigne);

                // --- CRÉATION DE L'UNITÉ AVEC PARAMÈTRES RÉELS ---
                // UnitName name, Player* player, Case* case_unit, std::vector<TerrainsType> allow_terrain
                Unit* newUnit = new Unit(nomCol, debugPlayer, &c, {});
                
                // On ajoute l'unité à la case
                c.add_unit(newUnit);
                
            } else {
                // Reste de la carte en neutre et vide
                c.set_country(Country::Neutral);
            }
        }
    }
    
    // On force la mise à jour de l'affichage
    update();
}


