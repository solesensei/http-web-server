#include <iostream>
#include "server_socket.h"

using namespace std;

//! Functions.
//!======================================================================================
void run_server();

//! Main.
//!======================================================================================
int main()
{
    run_server();
    
    return 0;
}

//!======================================================================================
void run_server()
{
    MyServerSocket_select socket;
    SocketAddress address("localhost", 8000);
    socket.bind(&address);
    socket.listen();
    socket.run();
}
