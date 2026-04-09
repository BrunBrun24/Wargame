#pragma once
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLTexture>


class Mainmenu : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

    public:
        Mainmenu(QWidget* parent = nullptr);
        ~Mainmenu();
    protected:
        void initializeGL() override;
        void resizeGL(int w, int h) override;
        void paintGL() override;
    
    private:
        QOpenGLTexture* texture;
};
