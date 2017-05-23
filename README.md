# http-web-server
#### by sole and lazy

---

## JavaScript Interpreter [project](https://github.com/SoleSensei/http-web-server/projects/2)
### First stage :: tokenizing
token analyzer

    tmain.cpp
    tokenizing.cpp
    Tokenlib.h

    :make token
    ./token

### Second stage :: syntax
syntax analyzer

     syntax.cpp
     Syntaxlib.h
     
### Third stage :: semantics
semantic analyzer
    
    syntax.cpp
    semantic.cpp
    interpretlib.h


### Forth stage :: Poliz

    syntax.cpp
    polizmain.cpp
    execute.cpp
    
    :make main
    ./js
---

## HTTP web server

     tcp-ip-server.cpp
     on_accept.cpp
     server_socket.cpp
     server_socket.h
     server.cpp
     
     :make smain
     ./server
