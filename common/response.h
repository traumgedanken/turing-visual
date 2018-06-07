#pragma once

#include "common_global.h"
#include "turiprogram.h"

class COMMONSHARED_EXPORT Response {
  public:
    int status = -1;
    TuriProgram * program = nullptr;
    QString word = "";
    QString state = "";
    int id = -1;
    int line = -1;

    Response() {}
    Response(int _status, QString _word = "", QString _state = "", int _id = -1,
             int _line = -1, TuriProgram * _program = nullptr);
    QString serialize();
    static Response deserialize(QString source);
};
