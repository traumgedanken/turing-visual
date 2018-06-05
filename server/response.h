#ifndef RESPONSE_H
#define RESPONSE_H

#include "turiprogram.h"
#include <QVector>
#include "turicarriage.h"

class Response {
  public:
    int status = -1;
    TuriProgram * program = nullptr;
    TuriCarriage * carriage;
    Response() {}
    Response(int _status, TuriProgram * _program, TuriCarriage * _carriage = nullptr);
    QString serialize();
    static Response deserialize(QString source);
    void clean();
};

#endif // RESPONSE_H
