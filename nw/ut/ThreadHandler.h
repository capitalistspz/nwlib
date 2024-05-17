#pragma once
#include <wut_structsize.h>

namespace nw {
namespace ut {

struct ThreadHandlerVTable {
    void* a;
    void* b;
    void* c;
    void* Destructor;
    void* e;
    void*(*ThreadHandlerProc)();
};
WUT_CHECK_OFFSET(ThreadHandlerVTable, 0x0c, Destructor);
WUT_CHECK_OFFSET(ThreadHandlerVTable, 0x14, ThreadHandlerProc);
class ThreadHandler {
public:
    ThreadHandlerVTable* vtable;
};
WUT_CHECK_OFFSET(ThreadHandler, 0x00, vtable);

}// namespace ut
}// namespace nw
