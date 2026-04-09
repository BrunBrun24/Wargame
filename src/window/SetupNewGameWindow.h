#pragma once
#include <QWidget>
#include <QPushButton>

class SetupNewGameWindow : public QWidget
{
    Q_OBJECT
public:
    SetupNewGameWindow(QWidget* parent = nullptr);

signals:
    void backToMain(); // Si tu veux revenir au menu principal
};
