#pragma once

#include "common_global.h"
#include "turiprogram.h"

enum FunctionName {
    /* usually uset to check connection */
    FN_NONE,

    /* parse program from code field in request */
    FN_PARSE_PROGRAM,

    /* create new carriage and save it on the server
       input word - code filed in request
       input program - program field in request
       return id of carriage to use it */
    FN_CARRIAGE_CREATE,

    /* return true if possible to make one more prev
       return special status if it is not possible
       return information about new carriage view */
    FN_CARRIAGE_PREV,

    /* return true if possible to make one more next
       return special status if it is not possible
       return information about new carriage view */
    FN_CARRIAGE_NEXT,

    /* move carriage view to left
       return information about new carriage view*/
    FN_CARRIAGE_LEFT,

    /* move carriage view to right
       return information about new carriage view */
    FN_CARRIAGE_RIGHT,

    /* return current word on carriage without whitespaces */
    FN_CARRIAGE_WORD,

    /* run all program to the end
       return information about new carriage view */
    FN_CARRIAGE_RUN,

    /* free memory of created carriage */
    FN_CARRIAGE_DELETE
};

class COMMONSHARED_EXPORT Request {
  public:
    FunctionName functionName = FN_NONE;
    QString code = "";
    TuriProgram * program = nullptr;
    int id = -1;

    Request() {}
    Request(FunctionName _functionName, int _id = -1, QString _code = "",
            TuriProgram * _program = nullptr);

    QString serialize();

    static Request deserialize(QString _source);
};
