#include <QApplication>
//#include "MyGLWidget.h"
#include "Mainmenu.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    Mainmenu widget;
    widget.resize(800, 600);
    widget.show();

    return app.exec();
}
