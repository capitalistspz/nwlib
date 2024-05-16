#pragma once

extern "C"
{
    //! Returns amount actually advanced
    uint32_t AXRmtAdvancePtr(uint32_t count);
    //! Gets samples, returns
    uint32_t AXRmtGetSamples(uint32_t index, int16_t* samples, uint32_t sampleCount);
    //! Returns number of available samples
    uint32_t AXRmtGetSamplesLeft();
}