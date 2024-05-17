#include "MessageQueue.h"

namespace nw {
namespace ut {
void MessageQueue::Initialize(OSMessage* messages, uint32_t size) {
    OSInitMessageQueue(&m_queue, messages, size);
}
void MessageQueue::Finalize() {
    //! No-op
}

BOOL MessageQueue::Jam(void* data, bool blocking) {
    OSMessage out;
    out.message = data;
    return OSSendMessage(&m_queue, &out, (OSMessageFlags)(blocking | OS_MESSAGE_FLAGS_HIGH_PRIORITY));
}

BOOL MessageQueue::Send(void* data, bool blocking) {
    OSMessage out;
    out.message = data;
    return OSSendMessage(&m_queue, &out, (OSMessageFlags)(blocking));
}
BOOL MessageQueue::Recv(void** outData, bool blocking) {
    OSMessage in;
    BOOL res = OSReceiveMessage(&m_queue, &in,  (OSMessageFlags)blocking);
    *outData = in.message;
    return res != 0;
}
}// namespace ut
}// namespace nw