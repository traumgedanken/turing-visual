#ifndef HTMLTEXT_H
#define HTMLTEXT_H

#include <QString>
#include <QVector>
#include <turiparsererror.h>

class HtmlText
{
    QString text;
public:
    HtmlText(QString _text);
    void setColorAtLine(int lineNumber, QString color);
    QString getText();

    void markErrors(QVector<TuriParserError *> & errors, QString color);
private:
    int getLineStartIndex(int lineNumber);
};

#endif // HTMLTEXT_H
