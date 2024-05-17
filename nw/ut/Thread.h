#pragma once
#include "ThreadHandler.h"
#include <coreinit/thread.h>
#include <coreinit/threadqueue.h>
#include <wut_structsize.h>
namespace nw {
namespace ut {

class Thread {
public:
    struct CreateArg
    {
        int32_t priority;
        void* stack;
        uint32_t stackSize;
        WUT_UNKNOWN_BYTES(2);
        uint16_t attributes;
        char* name;
    };

    explicit Thread(ThreadHandler* handler);
    bool Create(CreateArg* arg);

    void Destroy();

    //! Resume__Q3_2nw2ut6ThreadFv
    void Resume();


    BOOL Join();

    //! Sleep__Q3_2nw2ut6ThreadFv
    void Sleep();

    void Wakeup();

    //! GetPriority__Q3_2nw2ut6ThreadCFv
    int32_t GetPriority() const;

    //! SetPriority__Q3_2nw2ut6ThreadFi
    BOOL SetPriority(int32_t priority);


    //! IsTerminated__Q3_2nw2ut6ThreadFv
    BOOL IsTerminated();
    //! Exit__Q3_2nw2ut6ThreadFv
    void Exit();

    static int ThreadFunc(int, void* arg);

    WUT_UNKNOWN_BYTES(0x6a8);
    OSThread* m_thread;
    OSThreadQueue m_threadQueue;
    CreateArg m_createArg;
    ThreadHandler* m_handler;
};
WUT_CHECK_OFFSET(Thread::CreateArg, 0x00, priority);
WUT_CHECK_OFFSET(Thread::CreateArg, 0x04, stack);
WUT_CHECK_OFFSET(Thread::CreateArg, 0x08, stackSize);
WUT_CHECK_OFFSET(Thread::CreateArg, 0xe, attributes);

WUT_CHECK_OFFSET(Thread, 0x6ac, m_threadQueue);
WUT_CHECK_OFFSET(Thread, 0x6a8, m_thread);
WUT_CHECK_OFFSET(Thread, 0x6bc, m_createArg);
WUT_CHECK_OFFSET(Thread, 0x6d0, m_handler);


}// namespace ut
}// namespace nw
