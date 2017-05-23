#include "server_socket.h"

using namespace std;

// class SocketAddress
//!======================================================================================
SocketAddress::SocketAddress(const char *hostname, short port)
{
    struct hostent *hostent_struct;
    hostent_struct = ::gethostbyname(hostname);
    
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    memcpy(&address.sin_addr, hostent_struct->h_addr, hostent_struct->h_length);
}

struct sockaddr_in *SocketAddress::getAddress()
{
    return &address;
}

//class Socket
//!======================================================================================
Socket::Socket()
{
    if ((_sd = ::socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Socket wasn't created");
        exit(1);
    }
}

Socket::~Socket()
{
    if (_sd >= 0)
    {
        if (::shutdown(_sd, 1) < 0)
            if (::shutdown(_sd, 2) < 0)
                perror("Final shutdown error");
        close(_sd);
    }
}

int Socket::get_sd() const
{
    return _sd;
}

// class ConnectionSocket
//!======================================================================================
ConnectionSocket::ConnectionSocket()
{
    // for new connection socket
}

ConnectionSocket::ConnectionSocket(int sd)
{
    close(_sd); // for accept socket, that already exist
    _sd = sd;
}

int ConnectionSocket::send(void *buffer, size_t n)
{
    unsigned int len;
    len = ::send(_sd, buffer, n, 0);
    if (len != n)
    {
        cerr << "Send failed" << endl;
    }
    return len;
}

int ConnectionSocket::receive(void *buffer, size_t n)
{
    int len;
    len = ::recv(_sd, buffer, n, 0);
    if (len == -1)
    {
        cerr << "Receive failed" << endl;
    }
    return len;
}

void ConnectionSocket::connect(SocketAddress *pAddress)
{
    if (::connect(_sd, (struct sockaddr*) pAddress -> getAddress(), sizeof(struct sockaddr_in)) < 0)
    {
        perror("Connection failed");
        exit(2);
    }
}

// IOSocket with select
//!======================================================================================
IOSocket_select::IOSocket_select()
{
    //use sd, that already exist (in Socket constructor)
    buffer = new char [10240];
    strcpy(buffer, "");
    curr_buffer = buffer;
    bytes_to_send = 0;
    file_descriptor = 0;
    body_size = 0;
    cgihandler = NULL;
}

IOSocket_select::IOSocket_select(const IOSocket_select &socket)
{
    _sd = socket.get_sd();
    buffer = new char [10240];
    strcpy(buffer, socket.buffer);
    curr_buffer = socket.curr_buffer;
    file_descriptor = socket.file_descriptor;
    bytes_to_send = socket.bytes_to_send;
    body_size = socket.body_size;
    cgihandler = new CGIHandler;
    *cgihandler = *(socket.cgihandler);
}

IOSocket_select::IOSocket_select(int new_sd)
{
    close(_sd); // for accept socket, that already exist
    _sd = new_sd;
    buffer = new char [10240];
    strcpy(buffer, "");
    curr_buffer = buffer;
    file_descriptor = 0;
    bytes_to_send = 0;
    body_size = 0;
    cgihandler = NULL;
}

bool IOSocket_select::receive_part()
{
    const int BUFFER_SIZE = 10240;
    int len = ::recv(_sd, curr_buffer, BUFFER_SIZE, 0);
    if (len == -1)
    {
        perror("Receive failed");
    }
    curr_buffer += len;
    if (strstr(buffer, "\r\n\r\n"))
    {
        *curr_buffer = '\0';
        curr_buffer = buffer;
        return true;
    }
    else
        return false;
}

bool IOSocket_select::send_part()
{
    const int BUFFER_SIZE = 10240;
    if (bytes_to_send == 0)
    {
        bytes_to_send = ::read(file_descriptor, buffer, BUFFER_SIZE);
        if (bytes_to_send < 0)
        {
            perror("Send, read from file error");
            exit(20);
        }
        curr_buffer = buffer;
    }
    int len = ::send(_sd, curr_buffer, bytes_to_send, 0);
    if (len < 0)
        perror("Send body error");

    bytes_to_send -= len;
    curr_buffer += len;
    cerr << body_size << " " << len << endl;
    body_size -= len;

    if (body_size == 0)
    {
        curr_buffer = buffer;
        close(file_descriptor);
        return true;
    }
    return false;
}

void IOSocket_select::send_response(char *response_buffer)
{
    cerr << "Response" << endl;
    cerr << response_buffer;
    int send_len, len = strlen(response_buffer);
    send_len = ::send(_sd, response_buffer, len, 0);
    if (send < 0)
        perror("Send header failed");
    if (len != send_len)
    {
        cerr << "Send header failed";
    }
}

char *IOSocket_select::get_buffer()
{
    return buffer;
}

IOSocket_select::~IOSocket_select()
{
    if (file_descriptor > 0)
        ::close(file_descriptor);
    if (cgihandler != NULL)
        delete cgihandler;
    delete [] buffer;
}

// class ServerSocket
//!======================================================================================
void ServerSocket::bind(SocketAddress *pAddress)
{
    int opt = 1;
    setsockopt(_sd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    if (::bind(_sd, (struct sockaddr*) pAddress -> getAddress(), sizeof(struct sockaddr_in)) < 0)
    {
        perror("Bind failed");
        exit(3);
    }
}

void ServerSocket::listen()
{
    const int backlog = 5; //number of requests
    if (::listen(_sd, backlog) < 0)
    {
        perror("Listen failed");
        exit(4);
    }
}

int ServerSocket::accept()
{
    int new_socket_sd;
    if ((new_socket_sd = ::accept(_sd, NULL, NULL)) < 0)
    {
        perror("Accept failed");
        exit(5);
    }
    IOSocket_select *new_socket = new IOSocket_select(new_socket_sd);
    on_accept(new_socket);
    return new_socket_sd;
}

// Final classes
// class ServerSocket (first test)
//!======================================================================================
/*void MyServerSocket::on_accept(IOSocket *pSocket)
{
    char buffer[10240];
    int n = pSocket -> receive(buffer, sizeof(buffer)-1);
    buffer[n]=0;
    //analizis of request
    char *response = (char *)"501 Not Implemented HTTP/1.1\r\n\r\n";
    pSocket -> send(response, strlen(response)-1);
    delete pSocket;
}*/

// class Server_socket on select
//!======================================================================================
void MyServerSocket_select::run()
{
    int new_socket_sd;
    fcntl(_sd, F_SETFL, O_NONBLOCK | FD_CLOEXEC);
    
    set<int> clients; //to receive
    set<int> clients_to_send;
    clients.clear();
    clients_to_send.clear();
    IOSocket_select *clients_sockets[512];
    int num_clients = 0;
    
    cout << "Server is running..." << endl;  	
    for (;;)
    {
        // fill set of sockets
        fd_set readset;
        FD_ZERO(&readset);
        FD_SET(_sd, &readset);
        
        fd_set writeset;
        FD_ZERO(&writeset);
        
        for(set<int>::iterator it = clients.begin(); it != clients.end(); it++)
            FD_SET(*it, &readset);
        for(set<int>::iterator it = clients_to_send.begin(); it != clients_to_send.end(); it++)
            FD_SET(*it, &writeset);

        // set timeout
        timeval timeout;
        timeout.tv_sec = 1;
        timeout.tv_usec = 0;
        
        // wait for event in sockets
        int max_sockets1, max_sockets2;
        if (clients.empty())
            max_sockets1 = 0;
        else
            max_sockets1 = *max_element(clients.begin(), clients.end());
        if (clients_to_send.empty())
            max_sockets2 = 0;
        else
            max_sockets2 = *max_element(clients_to_send.begin(), clients_to_send.end());
        max_sockets1 = max(max_sockets1, max_sockets2);
        max_sockets1 = max(_sd, max_sockets1);
        
        if(::select(max_sockets1 + 1, &readset, &writeset, NULL, &timeout) < 0)
        {
            perror("select");
            continue;
        }

        // determine type of event
        if(FD_ISSET(_sd, &readset))
        {
            // new connect
            new_socket_sd = accept();
            clients_sockets[num_clients++] = new IOSocket_select(new_socket_sd);
            clients.insert(new_socket_sd);
        }
        for(int i = 0; i < num_clients; i++)
        {
            if (FD_ISSET(clients_sockets[i] -> get_sd(), &readset))
            {
                //read from socket
                if (clients_sockets[i] -> receive_part())
                {
                    clients.erase(clients_sockets[i] -> get_sd());
                    if (::shutdown(clients_sockets[i] -> get_sd(), 0) < 0)// stop read
                        perror("Shutdown");
                    
                    int request_type = on_accept(clients_sockets[i]);
                    if (request_type == 2 || request_type == 0)
                    {
                        //response sent, no files to send - delete connection
                        delete clients_sockets[i];
                        for (int j = i; j < num_clients - 1; j++)
                        {
                            clients_sockets[j] = clients_sockets[j+1];
                        }
                        num_clients--;
                    }
                    else if (request_type == 1)
                    {
                        clients_to_send.insert(clients_sockets[i] -> get_sd());
                        //FD_SET(clients_sockets[i] -> get_sd(), &writeset);// crutch
                    }
                    else if (request_type == 3)// cgi
                    {
                        ;// nothing
                    }
                }
            }
            if (FD_ISSET(clients_sockets[i] -> get_sd(), &writeset))
            {
                //write to socket
                if (clients_sockets[i] -> send_part())
                {
                    //end of write
                    if (clients_sockets[i] -> cgihandler != NULL)
                    {
                        //delete temporary file
                        char buf[20];
                        sprintf(buf, "%d", clients_sockets[i] -> cgihandler -> pid);
                        char buf2[20];
                        strcpy(buf2, "temp");
                        strcat(buf2, buf);
                        cerr << buf2;
                        unlink(buf2);
                    }
                    clients_to_send.erase(clients_sockets[i] -> get_sd());
                    delete clients_sockets[i];
                    for (int j = i; j < num_clients - 1; j++)
                    {
                        clients_sockets[j] = clients_sockets[j+1];
                    }
                    num_clients--;
                }
            }
        }
        
        //wait for cgi processes
        int pid1, status;
        while ((pid1 = waitpid(-1, &status, WNOHANG)) > 0)
        {
            for (int i = 0; i < num_clients; ++i)
            {
                if (clients_sockets[i] -> cgihandler -> pid == pid1)
                {
                    if (WIFEXITED(status) && (WEXITSTATUS(status) == 0))
                    {
                        int redir_fd;
                        char redir_file_name[20];
                        strcpy(redir_file_name, "temp");
                        char temp_str[20];
                        sprintf(temp_str, "%d", pid1);
                        strcat(redir_file_name, temp_str);
                        redir_fd = open(redir_file_name, O_RDONLY);
                        if (redir_fd < 0)
                        {
                            perror("error with openig result file");
                            exit(21);
                        }
                        clients_sockets[i] -> file_descriptor = redir_fd;
                        /*need to add make_respinse */ 
                        //clients_sockets[i] -> cgihandler -> make_response(clients_sockets[i]);
                        clients_to_send.insert(clients_sockets[i] -> get_sd());// add to send list
                    }
                    else
                    {
                        char file_size[20];
                        struct stat file_info;
                        int file_descriptor = ::open("./program_error.html", O_RDONLY);
                        fstat(file_descriptor, &file_info);
                        sprintf(file_size, "%ld", file_info.st_size);
                        clients_sockets[i] -> body_size = file_info.st_size;

                        Response answer(400, file_size, 2);
                        clients_sockets[i] -> send_response(answer.get_buffer());
                        clients_to_send.insert(clients_sockets[i] -> get_sd());
                        clients_sockets[i] -> file_descriptor = file_descriptor;
                    }
                                        break;
                }
            }
        }
    }
}

int MyServerSocket_select::accept()
{
    int new_socket_sd;
    if ((new_socket_sd = ::accept(_sd, NULL, NULL)) < 0)
    {
        perror("Accept failed");
        exit(5);
    }
    fcntl(new_socket_sd, F_SETFL, O_NONBLOCK | FD_CLOEXEC);
    //IOSocket *new_socket = new IOSocket(new_socket_sd);
    return new_socket_sd;
}
///-------------------------------------fix this
 /*void RunCgi (Response * resp, char * filename, int * fd, char * args, BaseSocket * comSock) {
        int pid;

        if ( (pid = fork() ) == 0) {  //сын
            int tempFd = ::open("temp.txt", O_CREAT|O_WRONLY|O_TRUNC , 00777);
            dup2(tempFd, 1); 
            strcpy (filename, "TransO"); //Home directory
            if ( execl(filename, filename, NULL) == -1 ) printf("Oh dear, something went wrong! %s\n", strerror(errno));
            exit(1);
            //Закрыть файл и еще что-то?!
        } 
        else { //отец
            int status;
            char file_size[20];
            struct stat file_info;

            waitpid (pid, &status, 0);

            if(WIFEXITED(status) ){//Хорошо закончили
                *fd = ::open ("temp.txt", O_RDONLY); 
                if (*fd == -1) printf("Oh dear, something went wrong! %s\n", strerror(errno));
                fstat(*fd, &file_info);
                sprintf(file_size, "%lld", (long long int)file_info.st_size);
                int content_type = 2;

                resp = new Response(200, file_size, content_type, true, NULL);
            }
            else{
                *fd = ::open ("./data/forbidden.html", O_RDONLY);
                fstat(*fd, &file_info);
                sprintf(file_size, "%lld", (long long int)file_info.st_size);
                resp = new Response(403, file_size, 2, false, NULL);
            }
            comSock->Write ( resp->GetBuf(), strlen(resp->GetBuf()) );
            sleep(1);
            comSock->SendFile (*fd, file_info.st_size);
        }
    }
*/
