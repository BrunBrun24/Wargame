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
    /*faut garder toujours une des 
      deux coordonnées la même pour un rectangle
        par exemple (A B 0) => (C B 0) => (C D 0) => (A D 0)
      */
    
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

        // Quad couvrant tout l'écran (qui vas afficher toute l'image)
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
    glColor3f(0.0f,0.0f,0.0f);
    glLineWidth(2.0f);
    for (size_t i = 0; i < grid.size(); ++i) {
        const std::vector<float>& rect = grid[i];
        glBegin(GL_LINE_LOOP);
            for (size_t j = 0; j < rect.size(); j += 3) {
                glVertex3f(rect[j], rect[j + 1], rect[j + 2]);
            }
        glEnd();
    }
    // partie de texte, faut que je comprenne comment cela fonctionne.
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

void Mainmenu::mousePressEvent(QMouseEvent *event){
    int x = event->position().x();
    int y = event->position().y();
    QString xStr = QString::number(x);
    QString yStr = QString::number(y);
    qDebug() << xStr << " , " << yStr << "\n";
    //traduction en OPENGL float

    int h = height();
    int w = width();

    float xGL = (2.0f * x / w) - 1.0f; // on fais 2 * x, puis divisé par w. on enlève 1 car sinon on est en dehors.
    float yGL = 1.0f - (2.0f * y / h);
    qDebug() << xGL << " , " << yGL << "\n";
    //Borders width of the rectangles (static borders, always the same)
    if((xGL <= 0.6f) && (xGL >= -0.6f)){
        if((yGL <= 0.8f) && (yGL >= 0.4f))
            qDebug() << "Nouvelle Partie" << "\n";
        else{
            if((yGL <= 0.2f) && (yGL >= -0.2f))
                qDebug() << "Continuer Partie" << "\n";
            else{
                if((yGL <= -0.4f) && (yGL >= -0.8f))
                    qDebug() << "Options" << "\n";
            }
        }
    }
}