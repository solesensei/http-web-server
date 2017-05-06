# http-web-server
#### by sole and lazy

---

## JavaScript Interpreter
### First stage :: tokenizing
token analyzer

    tmain.cpp
    tokenizing.cpp
    Tokenlib.h

    :make token

### Second stage :: syntax
syntax analyzer

     smain.cpp
     syntax.cpp
     Syntaxlib.h

     :make smain

### Third stage :: semantics
semantic analyzer
    
    smain.cpp
    syntax.cpp
    semantic.cpp
    interpretlib.h

    :make smain

### Forth stage :: Poliz

---

## HTTP web server

     web_server.cpp
