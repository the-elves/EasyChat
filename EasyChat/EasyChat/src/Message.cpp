#include "Message.h"

Message::Message(char *text, string time, string to, string from, MessageType type)
{
    strcpy(this->text, text);
    this->time = time;
    this->to = to;
    this->from = from;
    this->type = type;
}

Message::Message(){

}

string Message::getTo(){
    return to;
}

string Message::getFrom(){
    return from;
}

char * Message::getText(){
    return text;
}

MessageType Message::getType(){
    return type;
}


