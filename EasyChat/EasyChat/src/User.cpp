#include "User.h"

User::User(string name, string ipAddress, int sock, int port)
{
    this->name = name;
    this->ipAddress = ipAddress;
    this->socket = sock;
    this->port = port;
}
