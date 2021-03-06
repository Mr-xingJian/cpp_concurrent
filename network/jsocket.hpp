#ifndef J_SOCKET_H
#define J_SOCKET_H

#include <cstddef>
#include <cstdint>
#include <netinet/in.h>
#include <string>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "util/jbuffer.hpp"

namespace jarvis
{

namespace jnet
{

/*
 * NetAddress
 * 1.GetDomain 获取套接字通信域
 * 2.GetProtocolType 获取协议类型
 * 3.GetAddress 获取用于连接的地址
 */
class JNetAddress
{
public:
    // 可以表示多种类型的地址
    union GeneralAddr
    {
        sockaddr_in ipv4Addr;
        sockaddr_in6 ipv6Addr;
    };
    // AddrType
    enum AddrType
    {
        NET_IPv4,
        NET_IPV6,
        UNIX_LOCAL,
    };

    static int ResolveAddrIpV4(const sockaddr_in * addr, std::string & ip); 
    static int ResolveStrIpV4(const std::string & ip, sockaddr_in * addr);

public:
    JNetAddress(const std::string & ip, uint16_t port);
    JNetAddress();
    ~JNetAddress();
    int GetDomain() const;
    void SetDomain(int dm);
    int GetProtocolType() const;
    sockaddr * GetAddress();
    const sockaddr * GetAddress() const;
    socklen_t GetAddrLen() const;
private:
    GeneralAddr genAddr;
    AddrType addrType;
    int domain;
    int type;
};

/*
 * Socket
 * 1.GetSockFd 获取handle
 * 2.shutdown 关闭套接字
 * 3.Send 发送数据，继承类可以有具体的实现
 * 4.Recv 接收数据，继承类可以有具体实现
 */
class JSocket
{
public:
    JSocket(int domain, int type, int protocol);
    JSocket(int fd);
    virtual ~JSocket();
    int GetSockFd() const;
    int ShutDown(int how = SHUT_RDWR);
    int Close();

    size_t GetSendBufSize() const;
    size_t GetRecvBufSize() const;

    /*
     * Write和Read都是操作jsocket的缓存
     * Send和Recv则是真的从socket中读写数据
     */
    size_t Write(const void * buf, size_t sz);
    size_t Read(void * buf, size_t sz);
    size_t Pick(void * buf, size_t sz);

    size_t Send(size_t sz, bool again = false, bool * isAgain = NULL);
    size_t Recv(size_t sz, bool again = false, bool * isAgain = NULL);
private:
    jarvis::jutil::JNetBuffer * sendBuf;
    jarvis::jutil::JNetBuffer * recvBuf;
protected:
    int sockFd;
};

/*
 * ClientSocket
 * 1.Connect connect to server
 */
class JClientSocket: public JSocket
{
public:
    JClientSocket(int domain, int type, int protocol);
    JClientSocket(int fd);
    virtual ~JClientSocket();
    int Connect(const JNetAddress & address);
};

/*
 * ServerSocket
 * 1.Bind bind to address
 * 2.Accept accept a socket
 */
class JServerSocket: public JSocket
{
public:
    JServerSocket(int domain, int type, int ptotocol);
    virtual ~JServerSocket();
    int Bind(const JNetAddress & address);
    int Listen();
    int Accept(JNetAddress * clientAddr);
};

/*
 * DataHandler用于检查数据包的完整性
 * 1.CheckData
 */
class JDataHandler
{
public:
    virtual int HandleIn(jarvis::jnet::JClientSocket * socket) = 0;
    virtual int HandleOut(jarvis::jnet::JClientSocket * socket) = 0;
    virtual int CheckData(jarvis::jnet::JClientSocket * socket) = 0;
};

/*
 * EasyDataHandler一个简单的lv协议检测
 * Protocol: MagicNum(3) + Len(4) + Data(Len)
 */
class JEasyDataHandler: public JDataHandler
{
public:
    virtual int HandleIn(jarvis::jnet::JClientSocket * socket);
    virtual int HandleOut(jarvis::jnet::JClientSocket * socket);
    virtual int CheckData(jarvis::jnet::JClientSocket * socket);

};

}   // namespace jnet

}   // namespace jarvis

#endif
