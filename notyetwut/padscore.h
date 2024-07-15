#pragma once

#include <padscore/wpad.h>
#include <padscore/kpad.h>


extern "C"
{
struct WENCParams
{
    WUT_UNKNOWN_BYTES(32);
};

uint32_t WENCGetEncodeData(WENCParams* params, BOOL continuing, const int16_t* samples, uint32_t count, uint8_t* encodedData);
}