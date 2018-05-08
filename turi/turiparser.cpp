#include "turiparser.h"
#include <iostream>

TuriParser::TuriParser(QString & _code) {
    code = _code;
}

TuriParser::~TuriParser() {
    for (auto error : errors) {
        delete error;
    }
}

TuriProgram * TuriParser::parseTuriProgram() {
    QVector<QVector<QString>> table = getCsvTable();
    TuriProgram * program = new TuriProgram();
    for (int lineIndex = 0; lineIndex < table.length(); lineIndex++) {
        auto & line = table[lineIndex];
        if (line.length() == 0) {
            continue;
        }
        if (line.length() < 4) {
            errors.push_back(new TuriParserError(lineIndex + 1, "Incomplite line"));
            continue;
        }
        if (!isValidState(line[0])) {
            errors.push_back(new TuriParserError(lineIndex + 1, "Invalid current state"));
        }
        if (!isValidSymbols(line[1]) ) {
            errors.push_back(new TuriParserError(lineIndex + 1, "Error at symbol block"));
            continue;
        }
        if (!isValidDirection(line[2]) ) {
            errors.push_back(new TuriParserError(lineIndex + 1, "Invalid direction"));
        }
        if (!isValidState(line[3])) {
            errors.push_back(new TuriParserError(lineIndex + 1, "Invalid next state"));
        }
    }
}

QVector<QVector<QString>> TuriParser::getCsvTable(){
    QVector<QVector<QString>> table;
    QVector<QString> newLine;
    table.append(newLine);
    int index = 0;
    int line = 0;
    int lineIndex = 0;
    while (true) {
        QString elem;
        while (index < code.length() &&
               code.at(index) != ',' &&
               code.at(index) != '\n') {
            elem.append(code.at(index));
            index++;
        }
        if (index == code.length()) break;
        table[line].append("");
        table[line][lineIndex] = elem;
        lineIndex++;
        if (code.at(index) == '\n') {
            table.append(newLine);
            line++;
            lineIndex = 0;
        }
        index++;

    }
    return table;
}
bool TuriParser::isValidState(QString & state) {
    if (state.isEmpty() || state == "!") return true;
    for (int index = 0; index < state.length(); index++) {
        if(!state.at(index).isLetterOrNumber()) {
            return false;
        }
    }
    return true;
}

bool TuriParser::isValidSymbol(const QChar symbol) {
    return symbol.isLetterOrNumber() ||
            symbol == " ";
}

bool TuriParser::isValidSymbols(QString & symbols) {
    if (symbols == "->") return true;
    if (symbols.length() > 4 || symbols.length() < 2) {
        return false;
    }
    if (symbols.length() == 4){
        return isValidSymbol(symbols.at(0)) &&
               symbols.at(1) == '-' &&
               symbols.at(2) == '>' &&
               isValidSymbol(symbols.at(3));
    }
    if (!((symbols.at(0) == '-' && symbols.at(1) == '>') ||
          (symbols.at(1) == '-' && symbols.at(2) == '>'))) {
        return false;
    }
    if (symbols.at(0) == '-') return isValidSymbol(symbols.at(2));
    return isValidSymbol(symbols.at(0));
}
bool TuriParser::isValidDirection(QString & direction) {
    return direction.isEmpty() ||
            direction == "L" ||
            direction == "N" ||
            direction == "R";
}

QVector<TuriParserError *> TuriParser::getErrors() {
    return errors;
}

