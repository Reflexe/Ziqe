# Network Redesign:

## Required capabilities:
    * (almost) identical API for both TCP and UDP.
    * Change the current Stream interface to support server-client connections.

# 0.1

# Defects:
    * No way to detect if a packet is a part of a stream or not with UDP.
      for example, we can't receive the next message from an `accept`ed client because:
        1. The socket server would receive it.
        2. It may be from another connection (the old connection already has been closed).

      Therefore, we should design a way to detect a splitted packet.

# New Design (not this old song by TFK):
```
interface Server {
    Server(Address, Port);

    Stream acceptClient ();
}

interface Stream {
    Connect(Address,Port);

    sendData ();
    Data recevieData();
}
```

## Description
    Server::Server:
        TCP: Create a new server and listen.
        UDP: Create a new server (port - connection to anyone).
    Server::acceptClient():
        TCP: Accept a new client.
        UDP: Wait for new package, and create a stream from it.


    Stream::Connect:
        TCP: Connect to a TCP server.
        UDP: Start a client that will be able to receive packets from this
             info only and save this info.

    Stream::Create:
        TCP: Nothing (everything is done by the OS, there is no need for Address and Port).
        UDP: The same as Connect.

    Stream::sendData:
        TCP: Send data to this connection.
        UDP: Send data to the info we saved before (on Connect)

    Stream::receiveData:
        TCP: Receive data from this connection.
        UDP: Get the data the server already received on acceptClient (if it is a server).
             Or, receive new data (if it is a client).


# 0.2
UDP: Write a complete protocol stack for our new protocol:
    A lookup table of <remote address, remote port> -> data queue.
    when someone want to read, it read a calls socket.receive(), then,
    it look up the required queue, insert it and trigger its condition variable.

Currenly, I'm not devepoing it, we'll do that on the optimization stage, I'll be using
TCP.
