#ifndef ZIQE_GLOBALMESSAGESTREAMFACTORY_HPP
#define ZIQE_GLOBALMESSAGESTREAMFACTORY_HPP

#include "Protocol/MessageStream.hpp"

namespace Ziqe {

/**
   @brief   A facory for global (broadcast) instances @c Protocol::MessageStream.
 */
class GlobalMessageStreamFactory
{
public:
    typedef Protocol::MessageStream::Port Port;
    typedef Protocol::MessageStream::Address Address;

    GlobalMessageStreamFactory(const Address &globalAddress,
                               const Port    &globalPort);

    Protocol::MessageStream create() const;

private:
    Port mPort;
    Address mAddress;
};

} // namespace Ziqe

#endif // ZIQE_GLOBALMESSAGESTREAMFACTORY_HPP
