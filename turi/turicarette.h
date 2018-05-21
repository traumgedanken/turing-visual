#pragma once

#include <QString>
#include <turiprogram.h>
#include <QLabel>

class TuriCarette {
    QString word;
    int position;
    QString currentState;
    QLabel * label = nullptr;

  public:
    TuriCarette() {}
    TuriCarette(QString _word, QLabel * label, QString & _currentState);

    int exec(TuriProgram * program);

    QString getResult();

    void drawTable();

    void drawWord();

  private:
    void move(DIRECTION direction);

    QChar getSymbol();

    void setSymbol(QChar symbol);

    void setState(QString state);
};
