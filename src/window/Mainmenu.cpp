#include "Mainmenu.h"

#include <QImage>
#include <QDebug>
#include <cmath>
#include <QOpenGLTexture>
#include <QPainter>
#include <QFont>
#include <QStringList>
#include <QMediaPlayer>
#include <QAudioOutput>

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
    clickPlayer = new QMediaPlayer(this);
    audioOutput = new QAudioOutput(this);

    clickPlayer->setAudioOutput(audioOutput);
    clickPlayer->setSource(QUrl("qrc:/sounds/chat.mp3"));
    audioOutput->setVolume(0.2); // Volume à 20%
    // Charge l'image depuis les ressources
    QImage img(":/images/civ6.png");
    if (img.isNull()) {
        qDebug() << "Impossible de charger l'image!";
        return;
    }

    // Supprime la texture existante si nécessaire
    delete texture;

    // Crée la texture OpenGL
    texture = new QOpenGLTexture(img.flipped(Qt::Vertical)); // mirrored pour corriger l'axe Y

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
   // --- Partie Texte ---
    QPainter painter(this);
    painter.setPen(Qt::black);

    // 1. Calcul de la taille de police dynamique
    // On définit une taille de base (ex: 20) pour une largeur de référence (ex: 1280)
    int baseWidth = 1280;
    int fontSize = qMax(10, (this->width() * 24) / baseWidth); 
    // qMax(10, ...) permet d'éviter que le texte devienne illisible sur les petites fenêtres

    QFont dynamicFont("Arial", fontSize);
    dynamicFont.setBold(true); // Optionnel : rend le menu plus lisible
    painter.setFont(dynamicFont);

    for (size_t i = 0; i < grid.size() && i < labels.size(); ++i) {
        const std::vector<float>& rect = grid[i];

        // Calculer le centre du rectangle
        float centerX = (rect[0] + rect[3] + rect[6] + rect[9]) / 4.0f;
        float centerY = (rect[1] + rect[4] + rect[7] + rect[10]) / 4.0f;

        // Convertir les coordonnées OpenGL (-1,+1) en pixels Qt
        int px = (centerX + 1.0f) * width() / 2.0f;
        int py = (1.0f - centerY) * height() / 2.0f;

        // 2. Calculer une zone de texte proportionnelle au rectangle OpenGL
        // Ici, tes rectangles font 1.2 de large (0.6 - (-0.6)) en coordonnées OpenGL
        int rectWidthPx = 1.2f * (width() / 2.0f);
        int rectHeightPx = 0.4f * (height() / 2.0f);

        // On crée un rectangle de texte centré sur le point calculé
        QRect textRect(px - rectWidthPx/2, py - rectHeightPx/2, rectWidthPx, rectHeightPx);

        // Dessiner le texte parfaitement centré dans sa zone
        painter.drawText(textRect, Qt::AlignCenter, labels[i]);
    }

    painter.end();
    
    
}

void Mainmenu::mousePressEvent(QMouseEvent *event){
    int x = event->position().x();
    int y = event->position().y();
    //traduction en OPENGL float
    int h = height();
    int w = width();

    float xGL = (2.0f * x / w) - 1.0f; // on fais 2 * x, puis divisé par w. on enlève 1 car sinon on est en dehors.
    float yGL = 1.0f - (2.0f * y / h);
    qDebug() << xGL << " , " << yGL << "\n";
    //Borders width of the rectangles (static borders, always the same)
    if((xGL <= 0.6f) && (xGL >= -0.6f)){
        if((yGL <= 0.8f) && (yGL >= 0.4f)){
            qDebug() << "Nouvelle Partie" << "\n";
            emit menuChanged(1); //1 sera Nouvelle partie
            
        } 
        else{
            if((yGL <= 0.2f) && (yGL >= -0.2f)){
                qDebug() << "Continuer Partie" << "\n";
                emit menuChanged(2); //2 sera Continuer une Partie  
                if (clickPlayer->playbackState() == QMediaPlayer::PlayingState) {
                    clickPlayer->stop();
                }
                clickPlayer->play();   
            }
            else{
                if((yGL <= -0.4f) && (yGL >= -0.8f)){
                    qDebug() << "Options" << "\n";
                    emit menuChanged(3); //3 sera Options.                    
                }
            }
        }
    }
}