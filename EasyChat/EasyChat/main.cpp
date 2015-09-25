#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <ctype.h>
#include"ChatRoom.h"
#include"Message.h"
#include"User.h"
#include<signal.h>
using namespace std;

void intHandler(int a);

int main()
{
    cout << "Hello world!" << endl;
    signal(SIGINT, intHandler);
    signal(SIGSEGV, intHandler);
    ChatRoom r("main");

    return 0;
}
void intHandler(int a){

    exit(1);
}
