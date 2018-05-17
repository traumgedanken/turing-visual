#ifndef HTMLTEXT_H
#define HTMLTEXT_H

#include <QString>



class HtmlText
{
    QString text;
public:
    HtmlText(QString _text);
    void setColorAtLine(int lineNumber, QString color);
    QString getText();
private:
    int getLineStartIndex(int lineNumber);
};

#endif // HTMLTEXT_H
