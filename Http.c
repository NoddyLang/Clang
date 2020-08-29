#define _CRT_SECURE_NO_WARNINGS
 
#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>
 
#define BUF_LEN 1024                      /* バッファのサイズ */
 
int main(int argc, char *argv[]){
    /* IP アドレス、ポート番号、ソケット */
    char destination[] = "192.168.0.50";
    unsigned short port = 80;
    char httppath[] = "/adlogger/";
    char httphost[] = "192.168.0.50";
    int dstSocket;
    int result;          //戻り値
 
    /* sockaddr_in 構造体 */
    struct sockaddr_in dstAddr;
 
    /* 各種パラメータ */
    char toSendText[BUF_LEN];
    char buf[BUF_LEN];
    int read_size;
 
 
    /* Windows 独自の設定 */
    WSADATA data;
    WSAStartup(MAKEWORD(2, 0), &data);
 
    /* sockaddr_in 構造体のセット */
    memset(&dstAddr, 0, sizeof(dstAddr));
    dstAddr.sin_port = htons(port);
    dstAddr.sin_family = AF_INET;
    dstAddr.sin_addr.s_addr = inet_addr(destination);
 
    /* ソケット生成 */
    dstSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (dstSocket < 0){
        printf("%d\n", GetLastError());
        printf("ソケット生成エラー\n");
    }
 
    /* 接続 */
    result = connect(dstSocket, (struct sockaddr *) &dstAddr, sizeof(dstAddr));
    if (result < 0){
        printf("%d\n", GetLastError());
        printf("バインドエラー\n");
    }
 
    /* HTTP プロトコル生成 & サーバに送信 */
    sprintf(toSendText, "POST %s HTTP/1.1\r\n", httppath);
    send(dstSocket, toSendText, strlen(toSendText) , 0);
 
    sprintf(toSendText, "Host: %s:%d\r\n", httphost, port);
    send(dstSocket, toSendText, strlen(toSendText), 0);
 
     
    sprintf(toSendText, "Content-Length: 11\r\n", httphost, port);
    send(dstSocket, toSendText, strlen(toSendText), 0);
         
    sprintf(toSendText, "\r\n");
    send(dstSocket, toSendText, strlen(toSendText), 0);
 
    // HTTP Body部の作成
    memset(toSendText, 0, sizeof(toSendText));
    for (int i = 0; i < 11; i++){
        toSendText[i] = i+5 ;
    }
    send(dstSocket, toSendText, 11, 0);
 
    //Dump HTTP response
    while (1){
        memset(buf, 0, sizeof(buf));
        read_size = recv(dstSocket, buf, BUF_LEN, 0);
        if (read_size > 0){
            printf("%s",  buf);
        }
        else {
            break;
        }
    }
 
    /* Windows 独自の設定 */
    closesocket(dstSocket);
    WSACleanup();
}