#ifndef USER_H
#define USER_H
#include<string>
using namespace std;
class User
{
    public:
        User(string, string,int, int);
        int getSocket(){return socket;}
        string getName(){return name;}
        void setFallBack(string ip, int port);
    protected:
    private:
        bool leader;
        string fallBackIp;
        string fallBackPort;
        string name,username,ipAddress;
        int socket,port;
};

#endif // USER_H
