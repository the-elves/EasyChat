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
    protected:
    private:
        string name,username,ipAddress;
        int socket,port;
};

#endif // USER_H
