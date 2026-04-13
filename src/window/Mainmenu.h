#pragma once
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLTexture>
#include <QMouseEvent>
#include <QMediaPlayer>
#include <QAudioOutput>

class Mainmenu : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

    signals:
        void menuChanged(int newMenu); // pour changer le menu

    public:
        Mainmenu(QWidget* parent = nullptr);
        ~Mainmenu();
        
    protected:
        void initializeGL() override;
        void resizeGL(int w, int h) override; //On aura la même taille de window que de window OPENGL
        void paintGL() override;
        void mousePressEvent(QMouseEvent *event) override;
    private:
        QOpenGLTexture* texture;
        QMediaPlayer *clickPlayer;
        QAudioOutput *audioOutput;
};
