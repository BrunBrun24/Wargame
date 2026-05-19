#pragma once

#include <QMainWindow>
#include <QResizeEvent>
#include <QStackedWidget>

#include "Mainmenu.h"
#include "MyGLWidget.h"
#include "OptionMenu.h"
#include "ScreenProductions/CityProductionWidget.h"
#include "SetupNewGameWindow.h"

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget* parent = nullptr);
  ~MainWindow() = default;

  void resizeEvent(QResizeEvent* event) override;

 private:
  Mainmenu* mainMenu;             // Le menu principal
  OptionMenu* optionMenu;         // Le menu des options
  SetupNewGameWindow* setupmenu;  // Le menu du setup de jeux
  MyGLWidget* test;
  QStackedWidget* stackedWidget;
  CityProductionWidget* productionInterface;
  City* cityInEdition = nullptr;  // Pour savoir quelle ville on modifie
};