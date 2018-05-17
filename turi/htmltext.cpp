#include "htmltext.h"
#include <QDebug>

HtmlText::HtmlText(QString _text) { text = _text; }

void HtmlText::setColorAtLine(int lineNumber, QString color) {
    QString start = "<font style=\"background-color:" + color + "\">";
    int startIndex = getLineStartIndex(lineNumber);
    int endIndex = getLineStartIndex(lineNumber + 1) - 1;
    if (endIndex == text.length() - 1) {
        text.append("</font>");
    } else text.insert(endIndex, "</font>");
    text.insert(startIndex, start);
}

QString HtmlText::getText() { return text; }

int HtmlText::getLineStartIndex(int lineNumber) {
    int count = 0;
    int index = 0;
    while (count < lineNumber) {
        if (index == text.length()) return text.length();
        if (text.at(index) == '\n') count++;
        index++;
    }
    return index;
}



