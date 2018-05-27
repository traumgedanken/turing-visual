#pragma once

#include <QLabel>
#include <QString>
#include <turiprogram.h>

class TuriCarriage {
    QString word;
    int position;
    QString currentState;
    QLabel * label = nullptr;
    QVector<QString> words;
    QVector<QString> states;
    QVector<int> lines;
    QVector<int> positions;
    int step = 0;
    static const int carriageWidth = 800;
    static const int cellNumber = 20;
    int currentCellNumber = 3;
    static const int cellWidth = carriageWidth / cellNumber;
    static const int cellHeigth = 50;

  public:
    TuriCarriage() {}
    TuriCarriage(QLabel * _label);
    TuriCarriage(QString _word, QLabel * label, QString & _currentState,
                 TuriProgram * program);

    QString getResult();

    void draw();

    bool next();

    bool prev();

    int getLine();

    void moveView(DIRECTION direction);

  private:
    void move(DIRECTION direction);

    QChar getSymbol();

    void setSymbol(QChar symbol);

    void setState(QString state);

    void exec(TuriProgram * program);
};
