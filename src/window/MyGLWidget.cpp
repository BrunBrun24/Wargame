#include "MyGLWidget.h"
#include <QOpenGLFunctions>

MyGLWidget::MyGLWidget(QWidget* parent) : QOpenGLWidget(parent) {}

/*float int_color_into_float_color(int color){
    if (color < 256){
        return color / 255.0f;
    }
    return 1.0f;
}

float* int_array_color_into_float_array_color(const int* couleurs){
    float* transfert = new float[3];
    for (int i = 0; i < 3; ++i){
        transfert[i] = int_color_into_float_color(couleurs[i]);
    }
    return transfert;
}
*/
void MyGLWidget::initializeGL() {
    //initialisation de GL
    initializeOpenGLFunctions();
    //Juste pour du choix basique
    int red[3] = {255,0,0};
    int green[3] = {0,255,0};
    int blue[3] = {0,0,255};
    int choix1[3] = {139, 247, 246};
    int choix2[3] = {189, 17, 172};

    //float* choix_choix = int_array_color_into_float_array_color(choix1);
    //glClearColor(choix_choix[0],choix_choix[1],choix_choix[2],1.0f);
    glClearColor(1.0f,1.0f,1.0f,1.0f);
    //delete[] choix_choix;
}

void MyGLWidget::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
}

void MyGLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT);

    float radius = 0.05f;                // longueur d'un côté
    float dx = radius * sqrt(3.0f);      // distance horizontale entre centres
    float dy = 1.5f * radius;            // distance verticale entre centres

    glColor3f(0.0f, 0.0f, 0.0f);
    glLineWidth(2.0f);

    // Calcul automatique du nombre de colonnes et lignes selon la fenêtre (-1 à 1)
    int cols = static_cast<int>((2.0f) / dx) + 2; // +2 pour marge
    int rows = static_cast<int>((2.0f) / dy) + 2;

    for (int row = 0; row < rows; ++row) {
        float cy = 1.0f - radius - row * dy; // coordonnée Y

        for (int col = 0; col < cols; ++col) {
            float cx = -1.0f + radius + col * dx;
            if (row % 2 == 1) cx += dx / 2; // décalage des lignes impaires

            glBegin(GL_LINE_LOOP);
            for (int j = 0; j < 6; ++j) {
                float angle = M_PI / 2 - j * M_PI / 3; // pointe vers le haut
                float x = cx + radius * cos(angle);
                float y = cy + radius * sin(angle);
                glVertex2f(x, y);
            }
            glEnd();
        }
    }
}



/*
int hexCount = 30;        // nombre d'hexagones
    float radius = 0.05f;     // longueur de chaque côté
    float spacing = radius * sqrt(3.0f); // distance horizontale entre centres

    glColor3f(0.0f, 0.0f, 0.0f);
    glLineWidth(2.0f);

    float startX = -1.0f + radius;    // bord gauche
    float cy = 1.0f - radius;         // bord supérieur

    for (int i = 0; i < hexCount; ++i) {
        float cx = startX + i * spacing;

        glBegin(GL_LINE_LOOP);
        for (int j = 0; j < 6; ++j) {
            float angle = M_PI / 2 - j * M_PI / 3; // pointe vers le haut
            float x = cx + radius * cos(angle);
            float y = cy + radius * sin(angle);
            glVertex2f(x, y);
        }
        glEnd();
    }
Dessiner 30 hexagone cote a cote.         
*/