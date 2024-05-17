#include <coreinit/thread.h>


namespace altered {
using OSThreadEntryPointFn = int (*)(int, void*);
using OSThreadAttributes = ushort;
inline int32_t
OSCreateThread(OSThread* thread,
               OSThreadEntryPointFn fn,
               int32_t v,
               void* userData,
               void* stack,
               uint32_t stackSize,
               int32_t priority,
               OSThreadAttributes attr) {

    return ::OSCreateThread(thread,
                            reinterpret_cast<::OSThreadEntryPointFn>(fn),
                            v,
                            reinterpret_cast<char*>(userData),
                            reinterpret_cast<uint8_t*>(stack),
                            stackSize, priority, attr);
}
}// namespace altered