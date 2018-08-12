#ifndef DATABUFFER_H
#define DATABUFFER_H

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

using std::stack;
using std::vector;
using std::unique_ptr;
using std::runtime_error;
using std::string;

class DataBuffer {

public:
    DataBuffer();
    inline void pushInt(int value)          { ints.push_back(value); }
    inline void pushFloat(float value)      { floats.push_back(value); }
    inline void pushString(QString value)   { strings.push_back(value); }

    int     popInt();
    float   popFloat();
    QString popString();

    int     peekInt();
    float   peekFloat();
    QString peekString();

    inline void resetIntBufferPos(){ intCount = 0; }
    inline void resetFloatBufferPos(){ floatCount = 0; }
    inline void resetStringBufferPos(){ stringCount = 0; }
    inline void resetAllBufferPos(){ resetIntBufferPos(); resetFloatBufferPos(); resetStringBufferPos(); }

    inline size_t getIntBufferSize(){ return ints.size(); }
    inline size_t getFloatBufferSize(){ return floats.size(); }
    inline size_t getStringBufferSize(){ return strings.size(); }

    inline int getIntBufferPos(){ return intCount; }
    inline int getFloatBufferPos(){ return floatCount; }
    inline int getStringBufferPos(){ return stringCount; }

private:
  int intCount, floatCount, stringCount;
  vector<int> ints;
  vector<float> floats;
  vector<QString> strings;
};
#endif // DATABUFFER_H
