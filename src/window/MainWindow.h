#pragma once

#include <QMainWindow>
#include "Mainmenu.h"
#include "OptionMenu.h"
#include "SetupNewGameWindow.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private:
    Mainmenu* mainMenu;       // Le menu principal
    OptionMenu* optionMenu;   // Le menu des options
    SetupNewGameWindow* setupmenu; // Le menu du setup de jeux
};