#pragma once

#include <QLabel>
#include <QString>
#include <turiprogram.h>

class TuriCarette {
    QString word;
    int position;
    QString currentState;
    QLabel * label = nullptr;
    QVector<QString> words;
    QVector<QString> states;
    int step = 0;
    static const int caretteWidth = 800;
    static const int cellNumber = 20;
    static const int currentCellNumber = cellNumber / 2 - 2;
    static const int cellWidth = caretteWidth / cellNumber;
    static const int cellHeigth = 50;

  public:
    TuriCarette() {}
    TuriCarette(QString _word, QLabel * label, QString & _currentState,
                TuriProgram * program);

    QString getResult();

    void draw();

    bool next();

    bool prev();

  private:
    void move(DIRECTION direction);

    QChar getSymbol();

    void setSymbol(QChar symbol);

    void setState(QString state);

    void exec(TuriProgram * program);
};
