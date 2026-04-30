#include "MainWindow.h"
#include "Mainmenu.h"
#include "OptionMenu.h"
#include "SetupNewGameWindow.h"
#include "MyGLWidget.h"

#include <QStackedWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QStackedWidget *stackedWidget = new QStackedWidget(this);//système de pile.
    //On créer les widgets.
    mainMenu = new Mainmenu(this);
    optionMenu = new OptionMenu(this);
    setupmenu = new SetupNewGameWindow(this);
    test = new MyGLWidget(this);


    //Liste de widget (pages différentes qu'on peut avoir)
    stackedWidget->addWidget(mainMenu);   // Index 0
    stackedWidget->addWidget(optionMenu); // Index 1
    stackedWidget->addWidget(setupmenu);
    stackedWidget->addWidget(test);
    //premier widget sera celui affiché.
    setCentralWidget(stackedWidget);

    this->setMinimumSize(800, 600);
    // Connecte les signal
    //On connecte mainMenu, avec le signal menuChanged 
    //sur soi même, on fais la fonction lambda qui suis
    //elle change le widget actuel. 
    connect(mainMenu, &Mainmenu::menuChanged, this, [=](int menu){
        if(menu == 3){
            stackedWidget->setCurrentWidget(optionMenu); // Change la "feuille" visible
        }
        else if(menu == 1){
            stackedWidget->setCurrentWidget(setupmenu);
        }
    });

    // Connection pour revenir au menu principal
    //elle font la même chose, juste pas du même endroit.
    connect(optionMenu, &OptionMenu::backToMain, this, [=](){
        stackedWidget->setCurrentWidget(mainMenu);
    });
    connect(setupmenu, &SetupNewGameWindow::backToMain, this, [=](){
        stackedWidget->setCurrentWidget(mainMenu);
    });
    connect(setupmenu, &SetupNewGameWindow::gameCreated, this, [=](Game* game) {
    // 1. On donne la map au widget OpenGL
        if (!test) {
            qCritical() << "MainWindow : Le pointeur 'test' (MyGLWidget) est nul !";
            return;
        }
        test->setMapPtr(game->getMap());
        qDebug() << "MainWindow : Pointeur de map transmis au widget OpenGL.";
        stackedWidget->setCurrentWidget(test);
        qDebug() << "transition";
    });
}

MainWindow::~MainWindow()
{
}
