#ifndef REQUEST_H
#define REQUEST_H

#include "turiprogram.h"

#include <QString>

enum FunctionName {
    FN_NONE,
    // supported remote functions
    FN_PARSE_PROGRAM
};

class Request {
  public:
    FunctionName functionName = FN_NONE;
    QString code = "";
    TuriProgram * program = nullptr;

    Request() {}
    Request(FunctionName _functionName, QString _code, TuriProgram * _program = nullptr);
    QString serialize();
    static Request deserialize(QString _source);
};

#endif // REQUEST_H
