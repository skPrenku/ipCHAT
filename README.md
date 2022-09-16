The Client
    The client has the user interface, and accepts and receives text.
    It also sends commands
    Command -list lists all connected users.
    The sent message format is: <user> <message>
   *What do we show when we receive a message, do we show the sender too?
   *How does the client know its user's name?

The Server:
    The server provides an network endpoint for client connections
    The server maintains a list of connected users in realtime
    When a client requests a message to be sent to user, it finds that user and send the message
   *What does the server do if the user is not present?
    The server processes client commands.  At present there is only one command, -list.

So down to the part you're interested in, server design.

The flow might be something like:
    initialize server tcp endpoint
    use a game loop to:
        poll for client connections
        dispatch to a new thread to handle the new connection
       *each connection must identify itself to the server, we need to know its unique name
    each thread will:
        listen for messages from its client
        decide if the message is text to be sent to another client or a command
        handle the message

CLIENT:
Q1. What do we show when we receive a message, do we show the sender too?
A1. Yes, we show the senders name, in All-Chat, and also Private-Chat

Q2. How does the client know its user's name?
A2. The client will be prompd to type his username, ip and port when starting the client-chat application

SERVER:
Q1.What does the server do if the user is not present?
A1. The server finds the thread, and also the users DATA inside the map container "std::map<SOCKET,struct Client> userList" and delets it and updates the totalUsers counter

