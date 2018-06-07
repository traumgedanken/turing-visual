#include "turicommand.h"

TuriCommand::TuriCommand(QString _currentState, QString _nextState,
                         QChar _currentSymbol, QChar _nextSymbol,
                         DIRECTION _direction) {
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

QString TuriCommand::toArrowDescription() {
    QString result;
    result.append("'");
    result.append(currentSymbol);
    result.append("'/'");
    result.append(nextSymbol);
    result.append("',");
    result.append(directionToQString(direction));
    return result;
}

void TuriCommand::setCurrentState(QString _currentState) {
    currentState = _currentState;
}
void TuriCommand::setNextState(QString _nextState) { nextState = _nextState; }
void TuriCommand::setCurrentSymbol(QChar _currentSymbol) {
    currentSymbol = _currentSymbol;
}
void TuriCommand::setNextSymbol(QChar _nextSymbol) { nextSymbol = _nextSymbol; }
void TuriCommand::setDirection(DIRECTION _direction) { direction = _direction; }

QString TuriCommand::directionToQString(DIRECTION _direction) {
    switch (_direction) {
    case LEFT: return "L";
    case RIGHT: return "R";
    default: return "N";
    }
}

DIRECTION TuriCommand::QStringToDirection(QString str) {
    if (str == "L") return LEFT;
    if (str == "R") return RIGHT;
    return NONE;
}
