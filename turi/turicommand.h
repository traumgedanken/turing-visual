#ifndef TURICOMMAND_H
#define TURICOMMAND_H

#include <QString>
#include <defines.h>


class TuriCommand
{
    QString currentState;
    QString nextState;
    QChar currentSymbol;
    QChar nextSymbol;
    DIRECTION direction;
public:
    TuriCommand(QString _currentState, QString _nextState,
                QChar _currentSymbol, QChar _nextSymbol,
                DIRECTION _direction);

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
};

#endif // TURICOMMAND_H
