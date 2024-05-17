#include "Thread.h"
#include "../notyetwut/thread.h"

namespace nw {
namespace ut {

Thread::Thread(ThreadHandler* handler) {
    m_handler = handler;
    m_createArg = CreateArg{};
    OSInitThreadQueue(&m_threadQueue);
}

bool Thread::Create(Thread::CreateArg* arg) {
    m_createArg = *arg;
    const auto result = altered::OSCreateThread(m_thread,
                                                ThreadFunc,
                                                0,
                                                this,
                                                (uint8_t*) (arg->stack) + arg->stackSize,
                                                arg->stackSize,
                                                arg->priority,
                                                arg->attributes);
    if (result != 0 && arg->attributes) {
        OSSetThreadName(m_thread, arg->name);
    }
    return result != 0;
}

void Thread::Destroy() {
}

BOOL Thread::Join() {
    return OSJoinThread(m_thread, nullptr);
}

void Thread::Resume() {
    OSResumeThread(m_thread);
}

int Thread::ThreadFunc(int, void* arg) {
    auto thread = (Thread*) arg;
    (*thread->m_handler).vtable->ThreadHandlerProc();
    return 0;
}

int32_t Thread::GetPriority() const {
    return m_createArg.priority;
}

BOOL Thread::SetPriority(int32_t priority) {
    m_createArg.priority = priority;
    return OSSetThreadPriority(m_thread, priority);
}

BOOL Thread::IsTerminated() {
    return OSIsThreadTerminated(m_thread) != 0;
}

void Thread::Sleep() {

    OSSleepThread(&m_threadQueue);
}

void Thread::Wakeup() {
    OSWakeupThread(&m_threadQueue);
}

void Thread::Exit() {
    OSExitThread(0);
}



}// namespace ut
}// namespace nw