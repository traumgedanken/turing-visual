#include "turicarriage.h"
#include <QDebug>
#include <QFile>
#include <QPainter>
#include <QPicture>
#include <QThread>
#include <exceptionmessage.h>

TuriCarriage::TuriCarriage(QLabel * _label) {
    currentCellNumber = cellNumber / 2;
    label = _label;
    for (int i = 0; i < 100; i++)
        word.append(' ');
    position = 50;
    draw();
}

TuriCarriage::TuriCarriage(QString _word, QLabel * _label,
                           QString & _currentState, TuriProgram * program) {
    currentCellNumber = (cellNumber - word.length()) / 2;
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
    int end = word.length() - 1;
    while (word.at(start) == ' ')
        start++;
    while (word.at(end) == ' ')
        end--;
    for (int i = start; i <= end; i++)
        result.append(word.at(i));
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

void TuriCarriage::draw() {
    QString currStateTmp = step == states.length() ? "!" : states.at(step);

    label->clear();
    QPicture pi;
    QPainter p(&pi);

    p.setRenderHint(QPainter::Antialiasing);

    p.setPen(Qt::NoPen);
    if (currStateTmp == "!") {
        p.setBrush(QColor(255, 150, 140));
    } else
        p.setBrush(QColor(120, 180, 150));
    p.drawRect((currentCellNumber - 1) * cellWidth, 0, cellWidth, cellHeigth);

    p.setPen(QPen(Qt::darkGray, 2));
    p.drawLine(0, 0, carriageWidth, 0);
    p.drawLine(0, cellHeigth, carriageWidth, cellHeigth);
    p.drawRect((currentCellNumber - 1) * cellWidth, cellHeigth, cellWidth, 25);
    for (int i = cellWidth; i < carriageWidth; i += cellWidth) {
        p.drawLine(i, 0, i, cellHeigth);
    }

    p.setPen(QPen(Qt::black));
    QFont font = p.font();
    font.setPointSize(25);
    p.setFont(font);
    QString currWord =
        step == words.length()
            ? word.mid(position - currentCellNumber + 1, cellNumber)
            : words.at(step).mid(positions.at(step) - currentCellNumber + 1,
                                 cellNumber);
    for (int i = 0; i < cellNumber; i++) {
        QString ch = currWord.at(i);
        p.drawText(i * cellWidth, 0, cellWidth, cellHeigth, Qt::AlignCenter,
                   ch);
    }
    font.setPointSize(10);
    p.setFont(font);
    p.drawText((currentCellNumber - 1) * cellWidth, cellHeigth, cellWidth, 25,
               Qt::AlignCenter, currStateTmp);
    p.end();
    label->setPicture(pi);

    // QFile file("yourFile.png");
    // file.open(QIODevice::WriteOnly);
    // if (label->pixmap() == nullptr) qDebug() << "NULL!!";
    // save("yourFile.png");
    // label->pixmap()->save(&file, "PNG");
    // file.close();
}

bool TuriCarriage::next() {
    step++;
    draw();
    return step != words.length();
}

bool TuriCarriage::prev() {
    step--;
    draw();
    return step != 0;
}

int TuriCarriage::getLine() { return lines.at(step); }

void TuriCarriage::moveView(DIRECTION direction) {
    if ((direction == LEFT && currentCellNumber == cellNumber) ||
        (direction == RIGHT && currentCellNumber == 1))
        return;

    if (direction == LEFT) {
        currentCellNumber++;
    } else if (direction == RIGHT) {
        currentCellNumber--;
    }
    draw();
}
