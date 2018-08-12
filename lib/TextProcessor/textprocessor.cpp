#include "textprocessor.h"

TextProcessor::TextProcessor(unique_ptr<QTextStream> newIn, unique_ptr<QTextStream> newSchema, DataBuffer* newBuffer):
in(std::move(newIn)), schema(std::move(newSchema)), buffer(newBuffer){
    /*empty*/
}


void TextProcessor::parse(){

    //qDebug("\n");
    //qDebug("parse()");

    while(!in->atEnd()){
        QString schemaToken;
        *schema 	>> schemaToken;
        processSchema(schemaToken);
    }

}

void TextProcessor::processSchema(QString schemaToken){

    //qDebug("\n");
    //qDebug("processSchemaCommands()");
    //qDebug() << "schemaToken: " << schemaToken;

    //definitions
    if(schemaToken.compare("define_ignore_lines") == 0){
        defineIgnoreLines();
    }

    else if(schemaToken.compare("repeat_section") == 0){
        //qDebug("'repeat_section' found. added pos to stack");
        repeats.push(schema->pos());
    }

    else if(schemaToken.compare("end_section") == 0){
        endSection();
    }

    //values
    else if(schemaToken.compare("int") == 0){
        parseIntValue();
    }

    else if(schemaToken.compare("float") == 0){
        parseFloatValue();
    }

    else if(schemaToken.compare("quoted_string") == 0){
        parseQuotedStringValue();
    }

    //match text
    else{
        matchTokens(schemaToken);
    }

}

void TextProcessor::defineIgnoreLines(){

    //qDebug("\n");
    //qDebug("defineIgnoreLines()");

    while(!in->atEnd()){
       QString ignoreToken;

        *schema >> ignoreToken;

        if(ignoreToken.compare("end_ignore_lines") == 0){
            return;
        }

        //qDebug() << "pushing " << ignoreToken << " to ignoreTokens";
        ignoreLines.insert(ignoreToken);
    }

    QString msg = "Expected 'end_ignore_lines' but found 'End of File' instead";
    throw runtime_error(msg.toStdString());
}

void TextProcessor::matchTokens(QString schemaToken){

    //qDebug("\n");
    //qDebug("matchTokens()");

    QString inToken;
    *in 	>> inToken;

    //ignore_lines
    while(isIgnoreLine(&inToken)){
        in->readLine(); //move passed the line.
        *in >> inToken;
        //qDebug() << "new inToken: " << inToken;
    }

    if(!schemaToken.compare(inToken) == 0){
        QString msg = "Expected '" + schemaToken + "' but found '" + inToken + "'";
        throw runtime_error(msg.toStdString());
    }else{
        //qDebug() << "matched: " << schemaToken << " with: " << inToken;
    }

}


QString TextProcessor::peekForward(int numTokens, QTextStream* stream){

    QString token;
    qint64 prevPos = stream->pos();

    for(int i = 0; i < numTokens; i++){
        *stream >> token;
    }

    stream->seek(prevPos);

    return token;

}

bool TextProcessor::isIgnoreLine(QString* inToken){
    //qDebug("\n");
    //qDebug("isIgnoreLine()");

    return ignoreLines.contains(*inToken);
}



void TextProcessor::endSection(){

    //qDebug("\n");
    //qDebug("endSection()");

    QString schemaPeekToken;
    QString inPeekToken;

    schemaPeekToken = peekForward(1, schema.get()); //the token after section_end so }
    inPeekToken     = peekForward(1, in.get());     //token after ) so either // or "body" or }

    //qDebug() << "peeked into the future - schemaPeekToken: " << schemaPeekToken;
    //qDebug() << "peeked into the future - inPeekToken: " << inPeekToken;

    //move past the ignore potentially multiple ignore lines.
    while(isIgnoreLine(&inPeekToken)){
        in->readLine(); //skip the whole line
        inPeekToken = peekForward(1, in.get()); //peek the next token
    }

    //} == }
    //finish.
    if(schemaPeekToken.compare(inPeekToken) == 0){
        //qDebug() << schemaPeekToken << " and " << inPeekToken << " are the same! Repeat no more!";
        QString trash;

        *schema >> trash; //}
        *in     >> trash; //}

        repeats.pop(); //delete most recent repeat position.
    }
    //"body" != }
    //repeat
    else {
        //qDebug("continue repeating");
        schema->seek(repeats.top());
    }

}


void TextProcessor::parseIntValue(){

    //qDebug("\n");
    //qDebug("parseIntValue(){");

    bool ok = false;
    QString inToken;
    *in 	>> inToken;

    //ignore_lines
    while(isIgnoreLine(&inToken)){
        in->readLine(); //move passed the line.
        *in >> inToken;
        //qDebug() << "new inToken: " << inToken;
    }

    //qDebug() << "attempting to parse int value from: " << inToken;

    int value = inToken.toInt(&ok);

    if(ok){
        buffer->pushInt(value);
    }else{
        QString msg = "Expected an int value but got:" + inToken;
        throw runtime_error(msg.toStdString());
    }

}

void TextProcessor::parseFloatValue(){

    //qDebug("\n");
    //qDebug("parseFloatValue()");

    bool ok = false;
    QString inToken;
    *in 	>> inToken;

    //ignore_lines
    while(isIgnoreLine(&inToken)){
        in->readLine(); //move passed the line.
        *in >> inToken;
        //qDebug() << "new inToken: " << inToken;
    }

    //qDebug() << "attempting to parse float value from: " << inToken;

    float value = inToken.toFloat(&ok);

    if(ok){
        buffer->pushFloat(value);
    }else{
        QString msg = "Expected an float value but got:" + inToken;
        throw runtime_error(msg.toStdString());
    }

}

void TextProcessor::parseQuotedStringValue(){

    //qDebug("\n");
    //qDebug("parseQuotedStringValue()");

    QString inToken;
    *in 	>> inToken;

    //ignore_lines
    while(isIgnoreLine(&inToken)){
        in->readLine(); //move passed the line.
        *in >> inToken;
        //qDebug() << "new inToken: " << inToken;
    }

    //qDebug() << "attempting to parse quoted string value from: " << inToken;
    int end = inToken.length() -1;

    if(inToken.at(0) == '"' && inToken.at(end) == '"'){
        //qDebug("both ends of this string have quotes whoopeee!");

        QString pushVal = inToken.mid(1, end-1);

        //qDebug() << "pushing back: " << pushVal;

        buffer->pushString(pushVal);
    }

}

