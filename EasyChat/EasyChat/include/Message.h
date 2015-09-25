#ifndef MESSAGE_H
#define MESSAGE_H
#include<string.h>
#include<string>
using namespace std;

enum MessageType {
            Close, Connect, Msg
};

class Message
{
    public:

        Message(char *, string, string, string, MessageType);
        Message();
        string getTo();
        string getFrom();
        char * getText();
        MessageType getType();
    protected:
    private:
        char text[300];
        string time, to, from;
        MessageType type;

};

#endif // MESSAGE_H
