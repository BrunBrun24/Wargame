#include "Mainmenu.h"
#include <QImage>
#include <QDebug>
#include <cmath>
#include <QOpenGLTexture>
#include <QPainter>
#include <QFont>
#include <QStringList>

float int_color_into_float_color(int color){
    if (color < 256){
        return color / 255.0f;
    }
    return 1.0f;
}


Mainmenu::Mainmenu(QWidget* parent)
    : QOpenGLWidget(parent), texture(nullptr)
{
}

Mainmenu::~Mainmenu()
{
    delete texture;
}

void Mainmenu::initializeGL()
{
    initializeOpenGLFunctions();

    // Charge l'image depuis les ressources
    QImage img(":/images/civ6.png");
    if (img.isNull()) {
        qDebug() << "Impossible de charger l'image!";
        return;
    }

    // Supprime la texture existante si nécessaire
    delete texture;

    // Crée la texture OpenGL
    texture = new QOpenGLTexture(img.mirrored()); // mirrored pour corriger l'axe Y

    texture->setMinificationFilter(QOpenGLTexture::Linear);
    texture->setMagnificationFilter(QOpenGLTexture::Linear);

    texture->setWrapMode(QOpenGLTexture::ClampToEdge);

    glEnable(GL_TEXTURE_2D); // si tu utilises des fonctions legacy
}

void Mainmenu::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void Mainmenu::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);
    float gris = int_color_into_float_color(196);
    QStringList labels = { "Nouvelle Partie", "Continuer", "Options" };
    //faut garder toujours une des 
    //deux coordonnées la même pour un rectangle
    
    std::vector<std::vector<float>> grid = {
            { -0.6f, +0.8f, 0.0f,
              +0.6f, +0.8f, 0.0f,
              +0.6f, +0.4f, 0.0f,
              -0.6f, +0.4f, 0.0f
               },

            { -0.6f, +0.2f, 0.0f,
              +0.6f, +0.2f, 0.0f,
              +0.6f, -0.2f, 0.0f,
              -0.6f, -0.2f, 0.0f
               },

            { -0.6f, -0.4f, 0.0f,
              +0.6f, -0.4f, 0.0f,
              +0.6f, -0.8f, 0.0f,
              -0.6f, -0.8f, 0.0f
               },
        };
    if (texture) {
        texture->bind();

        // Quad couvrant tout l'écran
        glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 0.0f); glVertex2f(-1.0f, -1.0f);
            glTexCoord2f(1.0f, 0.0f); glVertex2f( 1.0f, -1.0f);
            glTexCoord2f(1.0f, 1.0f); glVertex2f( 1.0f,  1.0f);
            glTexCoord2f(0.0f, 1.0f); glVertex2f(-1.0f,  1.0f);
        glEnd();

        texture->release();
    }
    
    glColor3f(gris,gris,gris);
    for (size_t i = 0; i < grid.size(); ++i) {
        const std::vector<float>& rect = grid[i];
        glBegin(GL_QUADS);
        for (size_t j = 0; j < rect.size(); j += 3) {
            glVertex3f(rect[j], rect[j + 1], rect[j + 2]);
        }
        glEnd();
    }    
    QPainter painter(this);
    painter.setPen(Qt::black);
    painter.setFont(QFont("Arial", 18));

    for (size_t i = 0; i < grid.size() && i < labels.size(); ++i) {
        const std::vector<float>& rect = grid[i];

        // Calculer le centre du rectangle
        float centerX = (rect[0] + rect[3] + rect[6] + rect[9]) / 4.0f;
        float centerY = (rect[1] + rect[4] + rect[7] + rect[10]) / 4.0f;

        // Convertir les coordonnées OpenGL (-1,+1) en pixels Qt
        int px = (centerX + 1.0f) * width() / 2.0f;
        int py = (1.0f - centerY) * height() / 2.0f;

        // Dessiner le texte centré
        QRect textRect(px - 60, py - 15, 120, 30); // Ajuster si nécessaire
        painter.drawText(textRect, Qt::AlignCenter, labels[i]);
    }

    painter.end();
    
    
}

