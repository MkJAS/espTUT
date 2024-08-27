#include "TcpHandler.h"

#include "esp_netif.h"
#include "lwip/sockets.h"
#include "lwip/netdb.h"

#define LOG_LEVEL_LOCAL ESP_LOG_VERBOSE
#include "esp_log.h"
#define TAG "TCP"

#define PORT "8080"

TcpHandler::TcpHandler()
{


}



void TcpHandler::createSocket()
{
    struct addrinfo *result = NULL;
    struct addrinfo destAddr;


    
    destAddr.ai_family = AF_INET;
    destAddr.ai_socktype = SOCK_STREAM;
    destAddr.ai_protocol = IPPROTO_TCP;
    destAddr.ai_flags = AI_PASSIVE;


    int iResult;

    iResult = getaddrinfo(NULL,PORT,&destAddr,&result);
    if (iResult != 0)
    {
        ESP_LOGE(TAG, "getaddrinfo failed with error: errno %d", errno);
        vTaskDelete(NULL);
        return;
    }

    //inet_ntoa_r(destAddr.sin_addr, addr_str, sizeof(addr_str) - 1);

    m_listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (m_listenSocket < 0) {
        ESP_LOGE(TAG, "Unable to create socket: errno %d", errno);
        vTaskDelete(NULL);
        return;
    }
    ESP_LOGI(TAG, "Socket created");

    int err = bind(m_listenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (err != 0) {
        ESP_LOGE(TAG, "Socket unable to bind: errno %d", errno);
        close(m_listenSocket);
        vTaskDelete(NULL);
        return;
    }
    ESP_LOGI(TAG, "Socket bound, port %d", 8080);



}



void TcpHandler::acceptClient()
{

    int err = listen(m_listenSocket, 1);
    if (err != 0) {
        ESP_LOGE(TAG, "Error occurred during listen: errno %d", errno);
        close(m_listenSocket);
        vTaskDelete(NULL);
        return;
    }
    ESP_LOGI(TAG, "Socket listening");


    //struct sockaddr_in6 sourceAddr;
    //uint addrLen = sizeof(sourceAddr);
    m_clientSocket = accept(m_listenSocket, NULL, NULL);
    if (m_clientSocket < 0) 
    {
        ESP_LOGE(TAG, "Unable to accept connection: errno %d", errno);
    }

    ESP_LOGI(TAG, "Socket accepted");


}



void TcpHandler::write(char buff, int buffsize)
{
    int result;

    ESP_LOGI(TAG, "Sent: %d", buff);

    result = send(m_clientSocket, &buff, buffsize, 0);

    if (result < 0)
    {
        ESP_LOGE(TAG, "Error occurred during sending: errno %d", errno);
    }

}



std::string TcpHandler::readSocket()
{
    int result;
    char readBuffer[100];
    result = read(m_clientSocket, readBuffer, sizeof(readBuffer) -1);

    ESP_LOGI(TAG, "Read: %s", readBuffer);

    std::string buffer(readBuffer,result);

    return buffer;

}
