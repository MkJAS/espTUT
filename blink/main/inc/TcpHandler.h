#pragma once

#include <string>

class TcpHandler
{

public:

    TcpHandler();

    void createSocket();

    void acceptClient();

    void write(char buff, int buffsize);

    std::string readSocket();


private:

    int m_listenSocket;
    int m_clientSocket;




};