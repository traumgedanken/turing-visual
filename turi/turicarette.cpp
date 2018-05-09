#include "turicarette.h"
#include <QDebug>
#include <exceptionmessage.h>

TuriCarette::TuriCarette(QString _word) {
    for (int i = 0; i < 100; i++)
        word.append(' ');
    int wordLength = _word.length();
    position = (100 - wordLength) - 2;
    word.replace(position, wordLength, _word);
}

int TuriCarette::exec(TuriProgram * program) {
    currentState = program->getCommand(0)->getCurrentState();
    while (true) {
        TuriCommand * currentCommand = nullptr;
        for (int i = 0; i < program->count(); i++) {
            TuriCommand * command = program->getCommand(i);
            if (command->getCurrentState() == currentState &&
                (command->getCurrentSymbol() == getSymbol() ||
                  command->getCurrentSymbol() == ANY_SYMBOL)) {
                currentCommand = command;
                break;
            }
        }
        if (currentCommand == nullptr) return 1;
        setSymbol(currentCommand->getNextSymbol());
        setState(currentCommand->getNextState());
        move(currentCommand->getDirection());
        if (currentState == "!") break;
    }
    return 0;
}

QString TuriCarette::getResult() {
    QString result;
    int start = 0;
    int end = word.length() - 1;
    while (word.at(start) == ' ')
        start++;
    while (word.at(end) == ' ')
        end--;
    for (int i = start; i <= end; i++)
        result.append(word.at(i));
    return result;
}

void TuriCarette::move(DIRECTION direction) {
    if (direction == LEFT) {
        position--;
    } else if (direction == RIGHT)
        position++;
}

QChar TuriCarette::getSymbol() { return word.at(position); }

void TuriCarette::setSymbol(QChar symbol) { word.replace(position, 1, symbol); }

void TuriCarette::setState(QString state) { currentState = state; }
