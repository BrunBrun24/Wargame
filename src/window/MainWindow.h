#pragma once

#include <QMainWindow>
#include "Mainmenu.h"
#include "OptionMenu.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private:
    Mainmenu* mainMenu;       // Le menu principal
    OptionMenu* optionMenu;   // Le menu des options
};