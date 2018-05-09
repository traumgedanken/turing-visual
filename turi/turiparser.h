#pragma once

#include <QVector>
#include <turiprogram.h>
#include <turiparsererror.h>

class TuriParser
{
    QString code;
    QVector<TuriParserError *> errors;
public:
    TuriParser(QString & _code);
    ~TuriParser();
    TuriProgram * parseTuriProgram();
    QVector<TuriParserError *> getErrors();
private:
    QVector<QVector<QString> > getCsvTable();
    void clearErrors();

    bool isValidState(QString & state);
    bool isValidSymbol(const QChar symbol);
    void validateSymbols(QString & symbols);
    bool isValidDirection(QString & direction);

    void processErrorsAtLine(QVector<QString> & line, int lineIndex);

    static void getSymbols(QString source, QChar & current, QChar & next);
};
