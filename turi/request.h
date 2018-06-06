#ifndef REQUEST_H
#define REQUEST_H

#include "turiprogram.h"

#include <QString>

enum FunctionName {
    FN_NONE,
    // supported remote functions
    FN_PARSE_PROGRAM,
    FN_CARRIAGE_CREATE,
    FN_CARRIAGE_PREV,
    FN_CARRIAGE_NEXT,
    FN_CARRIAGE_LEFT,
    FN_CARRIAGE_RIGHT,
    FN_CARRIAGE_WORD
};

class Request {
  public:
    FunctionName functionName = FN_NONE;
    QString code = "";
    TuriProgram * program = nullptr;
    int id = -1;

    Request() {}
    Request(FunctionName _functionName, int _id, QString _code, TuriProgram * _program = nullptr);
    QString serialize();
    static Request deserialize(QString _source);
};

#endif // REQUEST_H
