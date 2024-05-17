#pragma once
#include <coreinit/messagequeue.h>
namespace nw {
namespace ut {

class MessageQueue {
public:
    //! __ct__Q3_2nw2ut12MessageQueueFv
    MessageQueue() = default;

    //! __dt__Q3_2nw2ut12MessageQueueFv
    ~MessageQueue() = default;

    //! Initialize__Q3_2nw2ut12MessageQueueFP9OSMessageUi
    void Initialize(OSMessage* message, uint32_t size);

    //! Finalize__Q3_2nw2ut12MessageQueueFv
    void Finalize();

    //! Jam__Q3_2nw2ut12MessageQueueFPvb
    BOOL Jam(void* data, bool blocking);

    //! Send__Q3_2nw2ut12MessageQueueFPvb
    BOOL Send(void* data, bool blocking);

    //! Recv__Q3_2nw2ut12MessageQueueFPPvb
    BOOL Recv(void** outData, bool blocking);


    OSMessageQueue m_queue;
};

}// namespace ut
}// namespace nw
