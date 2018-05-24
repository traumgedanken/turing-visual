#include "turicarette.h"
#include <QDebug>
#include <QPainter>
#include <QPicture>
#include <exceptionmessage.h>
#include <QThread>

TuriCarette::TuriCarette(QString _word, QLabel * _label, QString & _currentState, TuriProgram * program) {
    label = _label;
    for (int i = 0; i < 100; i++)
        word.append(' ');
    int wordLength = _word.length();
    position = (100 - wordLength) / 2;
    word.replace(position, wordLength, _word);
    currentState = _currentState;
    exec(program);
    draw();
}

void TuriCarette::exec(TuriProgram * program) {

    while (true) {
        QString caretteWord = word.mid(position - currentCellNumber + 1, cellNumber);
        words.append(caretteWord);
        states.append(currentState);
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
        if (currentCommand == nullptr) break;
        setSymbol(currentCommand->getNextSymbol());
        setState(currentCommand->getNextState());
        move(currentCommand->getDirection());
        if (currentState == "!") break;
    }
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

void TuriCarette::draw() {
    QString currStateTmp = step == states.length() ? "!" : states.at(step);

    label->clear();
    QPicture pi;
    QPainter p(&pi);

    p.setRenderHint(QPainter::Antialiasing);

    p.setPen(Qt::NoPen);
    if (currStateTmp == "!") {
        p.setBrush(QColor(255, 150, 140));
    } else p.setBrush(QColor(120, 180, 150));
    p.drawRect((currentCellNumber - 1) * cellWidth, 0, cellWidth, cellHeigth);

    p.setPen(QPen(Qt::darkGray, 2));
    p.drawLine(0, 0, caretteWidth, 0);
    p.drawLine(0, cellHeigth, caretteWidth, cellHeigth);
    p.drawRect((currentCellNumber - 1) * cellWidth, cellHeigth, cellWidth, 25);
    for (int i = cellWidth; i < caretteWidth; i += cellWidth) {
        p.drawLine(i, 0, i, cellHeigth);
    }

    p.setPen(QPen(Qt::black));
    QFont font = p.font() ;
    font.setPointSize(25);
    p.setFont(font);
    QString currWord = step == words.length() ? word.mid(position - currentCellNumber + 1, cellNumber) : words.at(step);
    for (int i = 0; i < cellNumber; i++) {
        QString ch = currWord.at(i);
        qDebug() << currWord.length();
        p.drawText(i * cellWidth, 0, cellWidth, cellHeigth, Qt::AlignCenter, ch);
    }
    font.setPointSize(10);
    p.setFont(font);
    p.drawText((currentCellNumber - 1) * cellWidth, cellHeigth, cellWidth, 25, Qt::AlignCenter, currStateTmp);
    p.end();
    label->setPicture(pi);
    //QObject().thread()->usleep(1000*1000*2);
}

bool TuriCarette::next() {
    step++;
    draw();
    return step != words.length();
}

bool TuriCarette::prev() {
    step--;
    draw();
    return step != 0;
}
