#include "MainWindow.h"
#include "Mainmenu.h"
#include "OptionMenu.h"

#include <QStackedWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QStackedWidget *stackedWidget = new QStackedWidget(this);//système de pile.

    mainMenu = new Mainmenu(this);
    optionMenu = new OptionMenu(this);

    stackedWidget->addWidget(mainMenu);   // Index 0
    stackedWidget->addWidget(optionMenu); // Index 1


    setCentralWidget(stackedWidget);

    this->setMinimumSize(800, 600);
    // Connecte le signal
    connect(mainMenu, &Mainmenu::menuChanged, this, [=](int menu){
        if(menu == 3){
            stackedWidget->setCurrentWidget(optionMenu); // Change la "feuille" visible
        }
    });

    // --- Connexion pour revenir au menu ---
    connect(optionMenu, &OptionMenu::backToMain, this, [=](){
        stackedWidget->setCurrentWidget(mainMenu);
    });
}

MainWindow::~MainWindow()
{
}
