#include <iostream>
#include <cstring>
#include <cstdio>

#include <netdb.h>
//#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <netinet/in.h>

#include <sys/select.h>
#include <fcntl.h>

#include <algorithm>
#include <set>
#include <vector>

#include <time.h>
#include <sys/stat.h>

using namespace std;

class SocketAddress
{
    struct sockaddr_in address;
public:
    SocketAddress(const char *hostname, short port);
    struct sockaddr_in *getAddress();
};

class IOSocket_select;
//!--------------------------------------------------------------------------------------
class CGIHandler
{
public:
    int pid;
    /* need to add this */
    void run_cgi(char *file_name, char *args);
    void make_response(IOSocket_select *pSocket);
};

// Socket classes
//!--------------------------------------------------------------------------------------

// base class
class Socket
{
protected:
    int _sd;
public:
    Socket();
    ~Socket();
    int get_sd() const;
};

// IOSocket for select
class IOSocket_select : public Socket
{
    char *buffer;
    char *curr_buffer;
public:
    long long body_size;
    int bytes_to_send;
    int file_descriptor;
    CGIHandler *cgihandler;
    
    IOSocket_select();
    IOSocket_select(const IOSocket_select &socket);
    explicit IOSocket_select(int new_sd);
    
    char *get_buffer();
    bool receive_part(); // TRUE if received all message
    bool send_part();
    void send_response(char *buffer);
    ~IOSocket_select();
};

// IO + connect for client
class ConnectionSocket : public Socket
{
public:
    ConnectionSocket();
    ConnectionSocket(int sd);
    int send(void *buffer, size_t n);
    int receive(void *buffer, size_t n);
    void connect(SocketAddress *pAddress);
};

//server
class ServerSocket: public Socket
{
public:
    void bind(SocketAddress *pAddress);
    void listen();
    virtual int accept();
protected:
    virtual int on_accept(IOSocket_select *pSocket) = 0;
};

class MyServerSocket_select: public ServerSocket
{
    int get_content_type(char *filename);// 1 text, 2 html, 3 jpg 0 other
public:
    virtual int accept();
    
    //returns type of request () 1 get, 2 head, 3 cgi, 0 other
    int on_accept(IOSocket_select *pSocket);
    virtual void run();
};

//! create string with response, depends on response type (200, 400, 403, 404, 501)
//!--------------------------------------------------------------------------------------
class Response
{
    char *buffer;
    int response_type;// 200 400 403 404 501
    char *content_length;
    int content_type;// 1 text, 2 html, 3 jpg 0 other
    time_t *last_modified;
    bool is_get;
public:
    Response(int resp_type, char *cont_length = NULL, int cont_type = 2, bool is_get1 = true, time_t *last_modif = NULL);
    char *get_buffer();
    ~Response();
    
    void add_Date();
    void add_Server();
    void add_Content_type();
    void add_Content_length();
    void add_Allow();
    void add_Last_modified();
    void add_end();
};
