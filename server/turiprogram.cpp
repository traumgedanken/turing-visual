#include "turiprogram.h"
#include <QByteArray>
#include <QFile>
#include <QIODevice>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTextCodec>
#include <QTextStream>

TuriProgram::TuriProgram() {}

TuriProgram::~TuriProgram() {
    for (auto & command : program) {
        delete command;
    }
}

void TuriProgram::addCommand(TuriCommand * newCommand) {
    program.push_back(newCommand);
}

TuriCommand * TuriProgram::getCommand(int index) { return program.at(index); }

int TuriProgram::count() { return program.length(); }

QString TuriProgram::toJson() {
    int programLength = program.length();
    QJsonDocument doc;
    QJsonObject commandsObj;
    commandsObj.insert("length", programLength);

    QJsonArray commandsArray;
    for (int i = 0; i < programLength; i++) {
        auto & currCommand = program.at(i);
        QJsonObject command;
        command.insert("number", i);
        command.insert("currentState", currCommand->getCurrentState());
        command.insert("nextState", currCommand->getNextState());
        QString currentSymbol;
        currentSymbol.append(currCommand->getCurrentSymbol());
        command.insert("currentSymbol", currentSymbol);
        QString nextSymbol;
        currentSymbol.append(currCommand->getNextSymbol());
        command.insert("nextSymbol", nextSymbol);
        QString direction =
            TuriCommand::directionToQString(currCommand->getDirection());
        command.insert("direction", direction);
        commandsArray.append(command);
    }
    commandsObj.insert("commands", commandsArray);
    doc.setObject(commandsObj);
    return doc.toJson();
}

TuriCommand * TuriProgram::getCommandWithCurrentState(QString _currentState) {
    for (auto command : program) {
        if (command->getCurrentState() == _currentState) return command;
    }
    return nullptr;
}

void TuriProgram::setErrorsList(QVector<TuriParserError *> &_errors) {
    errors = _errors;
}

QVector<TuriParserError *> TuriProgram::getErrors() { return errors; }

