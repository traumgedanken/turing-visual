#include "turicarette.h"
#include <QDebug>
#include <QPainter>
#include <QPicture>
#include <exceptionmessage.h>
#include <QThread>

TuriCarette::TuriCarette(QString _word, QLabel * _label) {
    label = _label;
    for (int i = 0; i < 100; i++)
        word.append(' ');
    int wordLength = _word.length();
    position = (100 - wordLength) / 2;
    word.replace(position, wordLength, _word);
}

int TuriCarette::exec(TuriProgram * program) {
    currentState = program->getCommand(0)->getCurrentState();
    while (true) {
        drawWord();
        //QObject().thread()->usleep(1000*1000*2);
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
    qDebug() << getResult();
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

void TuriCarette::drawTable() {
    QPicture pi;
    QPainter p(&pi);

    p.setRenderHint(QPainter::Antialiasing);

    p.setPen(Qt::NoPen);
    p.setBrush(QColor(120, 180, 150));
    p.drawRect(300, 0, 100, 100);

    p.setPen(QPen(Qt::darkGray, 2));
    p.drawLine(0, 0, 800, 0);
    p.drawLine(0, 100, 800, 100);
    p.drawLine(300, 100, 300, 150);
    p.drawLine(400, 100, 400, 150);
    p.drawLine(300, 150, 400, 150);
    for (int i = 100; i < 800; i += 100) {
        p.drawLine(i, 0, i, 100);
    }
    p.end();
    label->setPicture(pi);
}

void TuriCarette::drawWord() {
    label->clear();
    QPicture pi;
    QPainter p(&pi);

    p.setRenderHint(QPainter::Antialiasing);

    p.setPen(Qt::NoPen);
    p.setBrush(QColor(120, 180, 150));
    p.drawRect(300, 0, 100, 100);

    p.setPen(QPen(Qt::darkGray, 2));
    p.drawLine(0, 0, 800, 0);
    p.drawLine(0, 100, 800, 100);
    p.drawLine(300, 100, 300, 150);
    p.drawLine(400, 100, 400, 150);
    p.drawLine(300, 150, 400, 150);
    for (int i = 100; i < 800; i += 100) {
        p.drawLine(i, 0, i, 100);
    }

    p.setPen(QPen(Qt::black));
    QFont font = p.font() ;
    font.setPointSize(50);
    p.setFont(font);
    for (int i = 0; i < 8; i++) {
        QString ch = word.at(position + i - 3);
        qDebug() << ch;

        p.drawText(i * 100, 0, 100, 100, Qt::AlignCenter, ch);
    }
    qDebug() << "end";
    p.end();
    label->setPicture(pi);
}
