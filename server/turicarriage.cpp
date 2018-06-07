#include "turicarriage.h"

TuriCarriage::TuriCarriage() {
    for (int i = 0; i < 100; i++)
        word.append(' ');
    position = 50;
}

TuriCarriage::TuriCarriage(QString _word, TuriProgram * program) {
    for (int i = 0; i < 100; i++)
        word.append(' ');
    int wordLength = _word.length();
    position = (100 - wordLength) / 2;
    word.replace(position, wordLength, _word);
    currentState = program->getCommand(0)->getCurrentState();
    exec(program);
}

void TuriCarriage::exec(TuriProgram * program) {

    while (true) {
        positions.append(position);
        words.append(word);
        states.append(currentState);
        TuriCommand * currentCommand = nullptr;
        for (int i = 0; i < program->count(); i++) {
            TuriCommand * command = program->getCommand(i);
            if (command->getCurrentState() == currentState &&
                (command->getCurrentSymbol() == getSymbol() ||
                 command->getCurrentSymbol() == ANY_SYMBOL)) {
                currentCommand = command;
                lines.append(i);
                break;
            }
        }
        if (currentCommand == nullptr) {
            throw std::exception();
            break;
        }
        setSymbol(currentCommand->getNextSymbol());
        setState(currentCommand->getNextState());
        move(currentCommand->getDirection());
        if (currentState == "!") break;
    }
}

QString TuriCarriage::getResult() {
    QString result;
    int start = 0;
    QString source = step == words.length() ? word : words.at(step);
    int end = source.length() - 1;
    while (source.at(start) == ' ')
        start++;
    while (source.at(end) == ' ')
        end--;
    for (int i = start; i <= end; i++)
        result.append(source.at(i));
    return result;
}

void TuriCarriage::move(DIRECTION direction) {
    if (direction == LEFT) {
        position--;
    } else if (direction == RIGHT)
        position++;
}

QChar TuriCarriage::getSymbol() { return word.at(position); }

void TuriCarriage::setSymbol(QChar symbol) {
    word.replace(position, 1, symbol);
}

void TuriCarriage::setState(QString state) { currentState = state; }

bool TuriCarriage::next() {
    step++;
    return step != words.length();
}

bool TuriCarriage::prev() {
    step--;
    return step != 0;
}

void TuriCarriage::moveView(DIRECTION direction) {
    if ((direction == LEFT && currentCellNumber == cellNumber) ||
        (direction == RIGHT && currentCellNumber == 1))
        return;

    if (direction == LEFT) {
        currentCellNumber++;
    } else if (direction == RIGHT) {
        currentCellNumber--;
    }
}

QString TuriCarriage::getCurrentState() {
    return step == states.length() ? "!" : states.at(step);
}
QString TuriCarriage::getCurrentWord() {
    return step == words.length()
               ? word.mid(position - currentCellNumber + 1, cellNumber)
               : words.at(step).mid(positions.at(step) - currentCellNumber + 1,
                                    cellNumber);
}
int TuriCarriage::getCurrentLine() {
    return step == lines.length() ? -1 : lines.at(step);
}
int TuriCarriage::getPosition() { return currentCellNumber; }
