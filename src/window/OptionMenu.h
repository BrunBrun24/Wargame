#pragma once
#include <QWidget>
#include <QPushButton>

class OptionMenu : public QWidget
{
    Q_OBJECT
public:
    OptionMenu(QWidget* parent = nullptr);

signals:
    void backToMain(); // Si tu veux revenir au menu principal
};
