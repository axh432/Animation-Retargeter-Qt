#ifndef TEXTPROCESSOR_H
#define TEXTPROCESSOR_H

#include <QTextStream>
#include <QString>
#include <QFile>
#include <stack>
#include <vector>
#include <memory>
#include <iostream>
#include <stdexcept>
#include <string>
#include <QDebug>
#include <QSet>

#include "databuffer.h"

using std::stack;
using std::vector;
using std::unique_ptr;
using std::runtime_error;
using std::string;

class TextProcessor {

public:
    TextProcessor(unique_ptr<QTextStream> newIn, unique_ptr<QTextStream> newSchema, DataBuffer* newBuffer);
    void parse();
    inline DataBuffer* getBuffer(){ return buffer; }
    inline void setBuffer(DataBuffer* newBuffer){ this->buffer = newBuffer; }

private:
    DataBuffer* buffer;
    QSet<QString> ignoreLines;
    stack<qint64> repeats;
    unique_ptr<QTextStream> in;
    unique_ptr<QTextStream> schema;
    bool debugging = false;

    void processSchema(QString schemaToken);
    void defineIgnoreLines();
    void endSection();
    void parseIntValue();
    void parseFloatValue();
    void parseQuotedStringValue();
    void matchTokens(QString schemaToken);
    bool isIgnoreLine(QString* inToken);
    QString peekForward(int numTokens, QTextStream* stream);
    void debug(QString line);

};

#endif // TEXTPROCESSOR_H
