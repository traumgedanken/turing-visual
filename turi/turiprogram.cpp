#include "turiprogram.h"

TuriProgram::TuriProgram()
{

}

TuriProgram::~TuriProgram() {
    for (auto & command : program) {
        delete command;
    }
}

void TuriProgram::addCommand(TuriCommand * newCommand) {
    program.push_back(newCommand);
}

TuriCommand * TuriProgram::getCommand(int index) {
    return program.at(index);
}

int TuriProgram::numberOfCommand() {
    return program.length();
}
