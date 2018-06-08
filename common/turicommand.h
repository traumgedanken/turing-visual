#pragma once

#include "common_global.h"
#include "defines.h"
#include <QString>

class COMMONSHARED_EXPORT TuriCommand {
    QString currentState;
    QString nextState;
    QChar currentSymbol;
    QChar nextSymbol;
    DIRECTION direction;

  public:
    TuriCommand(QString _currentState, QString _nextState, QChar _currentSymbol,
                QChar _nextSymbol, DIRECTION _direction);

    // to descript lines on the graph
    QString toArrowDescription();

    QString getCurrentState();

    QString getNextState();

    QChar getCurrentSymbol();

    QChar getNextSymbol();

    DIRECTION getDirection();

    void setCurrentState(QString _currentState);

    void setNextState(QString _nextState);

    void setCurrentSymbol(QChar _currentSymbol);

    void setNextSymbol(QChar _nextSymbol);

    void setDirection(DIRECTION _direction);

    static QString directionToQString(DIRECTION _direction);

    static DIRECTION QStringToDirection(QString str);
};
