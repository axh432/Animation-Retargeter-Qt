#include "databuffer.h"

DataBuffer::DataBuffer(): intCount(0), floatCount(0), stringCount(0){ /*empty*/ }

int DataBuffer::popInt(){

    if(intCount >= ints.size()){
        throw runtime_error("At the end of int buffer");
    }

    int count = intCount;

    ++intCount;

    return ints[count];
}

float DataBuffer::popFloat(){

    if(floatCount >= floats.size()){
        throw runtime_error("At the end of float buffer");
    }

    int count = floatCount;

    ++floatCount;

    return floats[count];
}

QString DataBuffer::popString(){

    if(stringCount >= strings.size()){
        throw runtime_error("At the end of string buffer");
    }

    int count = stringCount;

    ++stringCount;

    return strings[count];
}

int DataBuffer::peekInt(){

    if(intCount >= ints.size()){
        throw runtime_error("At the end of int buffer");
    }

    return ints[intCount];
}

float DataBuffer::peekFloat(){

    if(floatCount >= floats.size()){
        throw runtime_error("At the end of float buffer");
    }

    return floats[floatCount];
}

QString DataBuffer::peekString(){

    if(stringCount >= strings.size()){
        throw runtime_error("At the end of string buffer");
    }

    return strings[stringCount];
}
