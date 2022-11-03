#pragma once
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

struct SocketWrapper : io_multiplexer::ioObject
{
    SocketWrapper()
    {
        this->open();
    }

    SocketWrapper(const SocketWrapper& sockerWrapper)
    {
        this->fd = sockerWrapper.getUnderlyingFileDescriptor();
    }

    ~SocketWrapper()
    {
        this->close();
    }

    void bind(const Endpoint& ep)
    {
        struct sockaddr_in address;
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = inet_addr(ep.address.c_str());
        address.sin_port = htons(ep.port);
        ::bind(fd, (struct sockaddr*)&address, sizeof(address));
    }

    void listen(std::size_t backlogSize = 1)
    {
        ::listen(fd, backlogSize);
    }

    std::pair<int,struct sockaddr_in> accept()
    {
        struct sockaddr_in address;
        int addrlen = sizeof(address);
        return {::accept(fd, (struct sockaddr*)&address, (socklen_t*)&addrlen),address};
    }

  private:
    void close()
    {
        ::close(fd);
    }
    void open()
    {
        fd = ::socket(AF_INET, SOCK_STREAM, 0);
    }
};

struct Socket
{
    Socket(Endpoint _ep) : ep(_ep)
    {
        mSocketPtr = std::make_unique<SocketWrapper>();
    }
    void bind()
    {
        mSocketPtr->bind(ep);
    }
    void listen(std::size_t backlogSize = 1)
    {
        mSocketPtr->listen(backlogSize);
    }
	auto accept()
	{
		return mSocketPtr->accept();
	}

  private:
    Endpoint ep;
    std::unique_ptr<SocketWrapper> mSocketPtr;
};

namespace tcp {
    struct Acceptor : Socket
    {
        using AcceptCallback = std::function<void(Socket)>;
        Acceptor(io_context::IOContext& ioCtx, Endpoint _ep) : Socket(_ep), _ioCtx(ioCtx)
        {
            bind();
            listen(4);
        }

        Socket accept()
        {
			auto res = accept();

            return Socket();
        }

        // void async_accept(AcceptCallback cb)
        // {
        //     mAccpetCallback = cb;
        //     setAsNonBlocking();
        //     io_multiplexer::IOMultiplexer::getInstance().registerEvent(
        //       *this, io_multiplexer::EventType::READ,
        //       std::bind(&Acceptor::acceptHandler, this, std::placeholders::_1,
        //                 std::placeholders::_2));
        // }

      private:
        struct sockaddr_in address;
        AcceptCallback mAccpetCallback;
        io_context::IOContext& _ioCtx;

        void acceptHandler(io_multiplexer::ioObject ioObj, io_multiplexer::EventType evType)
        {
            _ioCtx.post(mAccpetCallback, _accept());
        }

    };

}  // namespace tcp
}  // namespace ip
