#pragma once

#include <padscore/wpad.h>
#include <padscore/kpad.h>


extern "C"
{

typedef enum WPADSpeakerCmd {
    //! Deinitializes and turns off speaker
    WPAD_SPEAKER_CMD_OFF = 0,
    //! Turns on and initializes speaker to use 4-bit Yamaha ADPCM data format at 3000 Hz
    WPAD_SPEAKER_CMD_ON = 1,
    //! Mutes speaker
    WPAD_SPEAKER_CMD_MUTE = 2,
    //! Unmutes speaker
    WPAD_SPEAKER_CMD_UNMUTE = 3,
    //! Allows sound to play
    WPAD_SPEAKER_CMD_PLAY = 4,
    //! Does the same as WPAD_SPEAKER_CMD_ON
    WPAD_SPEAKER_CMD_ON_ALT = 5
} WPADSpeakerCmd;

struct WENCParams
{
    WUT_UNKNOWN_BYTES(32);
};

typedef void (*WPADControlSpeakerCallback)(WPADChan chan, int32_t status);

int32_t WPADControlSpeaker(WPADChan chan, WPADSpeakerCmd cmd, WPADControlSpeakerCallback callback);

BOOL WPADCanSendStreamData(WPADChan chan);

int32_t WPADSendStreamData(WPADChan chan, void* data, uint32_t length);

uint32_t WENCGetEncodeData(WENCParams* params, BOOL continuing, const int16_t* samples, uint32_t count, uint8_t* encodedData);
}