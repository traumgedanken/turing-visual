#include "turicommand.h"

TuriCommand::TuriCommand(QString _currentState, QString _nextState,
                         QChar _currentSymbol, QChar _nextSymbol,
                         DIRECTION _direction)
{
    currentState = _currentState;
    nextState = _nextState;
    currentSymbol = _currentSymbol;
    nextSymbol = _nextSymbol;
    direction = _direction;
}

QString TuriCommand::getCurrentState() { return currentState; }
QString TuriCommand::getNextState() { return nextState; }
QChar TuriCommand::getCurrentSymbol() { return currentSymbol; }
QChar TuriCommand::getNextSymbol() { return nextSymbol; }
DIRECTION TuriCommand::getDirection() { return direction; }

void TuriCommand::setCurrentState(QString _currentState) {
    currentState = _currentState; }
void TuriCommand::setNextState(QString _nextState) {
    nextState = _nextState;
}
void TuriCommand::setCurrentSymbol(QChar _currentSymbol) {
    currentSymbol = _currentSymbol;
}
void TuriCommand::setNextSymbol(QChar _nextSymbol) {
    nextSymbol = _nextSymbol;
}
void TuriCommand::setDirection(DIRECTION _direction) {
    direction = _direction;
}
