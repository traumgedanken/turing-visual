#include "turiprogram.h"

TuriProgram::TuriProgram() {}

TuriProgram::~TuriProgram() {
    for (auto & command : program)
        delete command;
}

void TuriProgram::addCommand(TuriCommand * newCommand) {
    program.push_back(newCommand);
}

TuriCommand * TuriProgram::getCommand(int index) { return program.at(index); }

int TuriProgram::count() { return program.length(); }

TuriCommand * TuriProgram::getCommandWithCurrentState(QString _currentState) {
    for (auto command : program) {
        if (command->getCurrentState() == _currentState) return command;
    }
    return nullptr;
}

void TuriProgram::setErrorsList(QVector<TuriParserError *> & _errors) {
    errors = _errors;
}

QVector<TuriParserError *> TuriProgram::getErrors() { return errors; }

void TuriProgram::removeCommands() {
    for (auto & com : program)
        delete com;
    program.clear();
}
