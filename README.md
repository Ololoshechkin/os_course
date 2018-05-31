# os_course

## HW7
### IPC (inter process communication)

Key-value password microservice working on Unix domain socket.

Features:
  1) Server sends pipes (file descriptors) to client process
  2) Zygote mechanism (https://medium.com/masters-on-mobile/the-zygote-process-a5d4fc3503db) :
    * Main server starts up and forks to Zygote service
    * Zygote server initializes some state
    * Client connects to the Main server
    * Main server sends a request to Zygote
    * Zygote forks and starts it's copy with the same initial state
    * Copy of Zygote initializes pipes to communicate with the Client
    * Zygote replies to the Main server with the new copy's pipes
    * Main server sends the pipes to the Client via IPC mechanism
    * Client receives the pipes and interacts directly with an independent Zygote's fork
  3) By using Zygote mechanism the following advantages were achieved:
    * Clients never see each other's key-value changes and never corrupt each other's data. That can be usefull when using the service for keeping passwords of different clients. 
    When using Zygote trick it becomes more difficult to abuse any valnurabilities to see another client's personal data
    * When Unix uses copy-on-write technique new Zygote forks start up immediately with ready-to-use initial state

## HW6
### Non-blocking networking and multiplexing.

Simple hello-server asynchronously listening for new connections, reading client requests and replying to them.
Features:
  1) multiplatform multiplexing implementations : epoll solution for linux, kqueue solution for mac os
  2) async client (multiplexing keyboard IO and network communication with server)

## HW5
### Simple terminal chat.
usage :
  a) chat server <port> <host???>
  b) chat client <port> <host???>
If not specified host equals 127.0.0.1
  
client comands
   * users --- get list of available users to start chat with
   * start_chat --- enter "Start Chat" mode in order to choose a partner to start chat with
      Within this mode user is asked to type a name of suggested partner. And after receiving of an acknowledgement (that        claims that client exists and is weeling to start chat with the client) moves to "In Chat" mode or to default mode.
   NOTE: if the chosen partner exists and connected to the server, it is asked for acknowledgement. All available (not in-chat) clients are listening for acknowledgement requests simultaniously with the main execution with a rate of 500ms.
   * exit_chat -- exists chat if the client's mode is "In Chat" and notifies the partner
   * message --- if the client is in chat with someone, enters "Send Message" mode.
      Within this mode client types a message (continous string with no whitespace characters) and sends to the partner
   * update_messages -- DEPRECATED COMAND!!! --- manually updates new messages from current partner if there is some.
   NOTE: update_messages is simmultaniously called by default each 500ms and all messages are being updated automatically. So, I don't recommend to use this comand. It was added just in case.
   * disconnect --- disconnects current client. If one is in chat --- exit_chat is called. After an execution of the comand all existing sockets are closed on both server and client side and server state contains no info of the client anymore.
   
NOTE-1: server is tolerant to ocasional client disconnections or corrupted messages. A client is tolerant to ocasional partner disconnection (after detecting one it moves to default state and notifies the user about chat was closed), but client is not tolerant to server failures or corrupted messages received from the server.
NOTE-2: there still remains a way to send arbitrary sequence of bytes to server wich represents very long numbers (message lengths). It can be fixed after a while.

TODO-plans: add cryptography in order to encrypt each message content. 

## HW4
### Linkage
3 types of libraries implemented:
 * static library
 * dynamic library
 * runtime-linked library

## HW3
### Jit

supports:
  * Cycle unrolling.
  
supporting cycles :
  * for (any varname = 0; any varname < any const; ++ any varname) {
  
     // any body
     
  }
  
Jit maps code in runtime, finds All cycles in the given code and replaces them with their body repeated N times. Supports N in [0;222].

## HW2
### grep utility

## HW1
### custom shell
