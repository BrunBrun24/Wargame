#include <QApplication>
#include <QIcon>
#include "MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    app.setWindowIcon(QIcon(":/images/test_civ6.jpg")); // ou chemin classique

    MainWindow widget;
    widget.resize(800, 600);
    widget.show();

    return app.exec();
}
