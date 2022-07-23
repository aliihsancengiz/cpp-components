#pragma once
#include "Buffer.hpp"
#include "Endpoint.hpp"
#include "IOContext.hpp"
#include "IOMultiplexer.hpp"
#include "Logger.hpp"

#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>

namespace ip {
enum SocketType
{
    TCP,
    UDP
};

struct SocketOps
{};

struct Socket : io_multiplexer::ioObject
{
    Socket(Endpoint _ep, SocketType _type = SocketType::TCP) : ep(_ep), type(_type)
    {
        fd = ::socket(AF_INET, SOCK_STREAM, 0);
    }
    Socket(io_multiplexer::ioObject socketObj)
    {
        fd = socketObj.fd;
    }
    void read() {}
    void write() {}
    void connect() {}
    void disconnect() {}
    void close()
    {
        ::close(fd);
    }
    auto getEndPoint()
    {
        return ep;
    }

  private:
    Endpoint ep;
    SocketType type;
};

namespace tcp {
    struct Acceptor : Socket
    {
        using AcceptCallback = std::function<void(Socket)>;
        Acceptor(io_context::IOContext& ioCtx, Endpoint _ep) : Socket(_ep), _ioCtx(ioCtx)
        {
            bind();
            listen();
        }
        void setAsNonBlocking()
        {
            int flags = fcntl(fd, F_GETFL, 0);
            flags |= O_NONBLOCK;
            fcntl(fd, F_SETFL, flags);
        }
        Socket accept()
        {
            return _accept();
        }
        void async_accept(AcceptCallback cb)
        {
            mAccpetCallback = cb;
            setAsNonBlocking();
            io_multiplexer::IOMultiplexer::getInstance().registerEvent(
              *this, io_multiplexer::EventType::READ,
              std::bind(&Acceptor::acceptHandler, this, std::placeholders::_1,
                        std::placeholders::_2));
        }

      private:
        struct sockaddr_in address;
        AcceptCallback mAccpetCallback;
        io_context::IOContext& _ioCtx;

        void acceptHandler(io_multiplexer::ioObject ioObj, io_multiplexer::EventType evType)
        {
            _ioCtx.post(mAccpetCallback, _accept());
        }
        Socket _accept()
        {
            int addrlen = sizeof(address);
            int newConnection = ::accept(fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
            return Socket(newConnection);
        }
        void bind()
        {
            address.sin_family = AF_INET;
            address.sin_addr.s_addr = inet_addr(getEndPoint().address.c_str());
            address.sin_port = htons(getEndPoint().port);
            ::bind(fd, (struct sockaddr*)&address, sizeof(address));
        }
        void listen(std::size_t backlogSize = 1)
        {
            ::listen(fd, backlogSize);
        }
        void close()
        {
            ::close(fd);
        }
    };

}  // namespace tcp
}  // namespace ip
