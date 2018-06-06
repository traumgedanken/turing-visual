#pragma once

#include <turiprogram.h>

class TuriCarriage {
    QString word;
    int position;
    QString currentState;
    QVector<QString> words;
    QVector<QString> states;
    QVector<int> lines;
    QVector<int> positions;
    int step = 0;
    static const int carriageWidth = 800;
    static const int cellNumber = 20;
    int currentCellNumber = 5;
    static const int cellWidth = carriageWidth / cellNumber;
    static const int cellHeigth = 50;

  public:
    TuriCarriage();
    TuriCarriage(QString _word, TuriProgram * program);

    QString getResult();

    bool next();

    bool prev();

    void moveView(DIRECTION direction);

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
