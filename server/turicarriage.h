#pragma once

#include <turiprogram.h>

class TuriCarriage {
    // to describe carriage state at exec
    QString word;
    int position;
    QString currentState;
    int step = 0;

    // history of steps
    QVector<QString> words;
    QVector<QString> states;
    QVector<int> lines;
    QVector<int> positions;

    // to get displayed word
    static const int cellNumber = 20;
    int currentCellNumber = 5;

  public:
    TuriCarriage(QString _word, TuriProgram * program);

    // get word on carriage withou whitespaces
    QString getResult();

    /* change current step,
       return true if possible to do it one more time */
    bool next();
    bool prev();

    void moveView(DIRECTION direction);

    // to get displayed word
    QString getCurrentState();
    QString getCurrentWord();
    int getCurrentLine();
    int getPosition();

  private:
    void move(DIRECTION direction);

    QChar getSymbol();

    void setSymbol(QChar symbol);

    void setState(QString state);

    void exec(TuriProgram * program);
};
