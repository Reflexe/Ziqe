#include "GlobalMessageStreamFactory.hpp"

namespace Ziqe {

GlobalMessageStreamFactory::GlobalMessageStreamFactory(const Address &globalAddress, const Port &globalPort)
    : mPort{globalPort}, mAddress{globalAddress}
{

}

Protocol::MessageStream GlobalMessageStreamFactory::create() const
{
    return Protocol::MessageStream::createGlobalConnection ()
}

} // namespace Ziqe
