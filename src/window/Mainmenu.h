#pragma once
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLTexture>
#include <QMouseEvent>


class Mainmenu : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
    signals:
    
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

};
