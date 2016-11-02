#ifndef ZIQE_PROTOCOL_MESSAGESERVER_HPP
#define ZIQE_PROTOCOL_MESSAGESERVER_HPP

#include "Network/Server.hpp"

#include "Network/TcpServer.hpp"
#include "Network/UdpServer.hpp"

#include "MessageStream.hpp"

namespace Ziqe {
namespace Protocol {

class MessageServer
{
public:
    MessageServer(Base::UniquePointer<Net::Server> &&server);

    MessageStream acceptClient () {
        auto maybeClientStream = mServer->acceptClient ();

        DEBUG_CHECK (maybeClientStream);
        return MessageStream{Base::move (maybeClientStream)};
    }

private:
    Base::UniquePointer<Net::Server> mServer;
};

} // namespace Protocol
} // namespace Ziqe

#endif // ZIQE_PROTOCOL_MESSAGESERVER_HPP
