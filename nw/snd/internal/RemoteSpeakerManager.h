#pragma once
#include <snd/RemoteSpeaker.h>

namespace nw {
namespace snd {
namespace internal {
class RemoteSpeakerManager {
    //! Methods
public:
    //! __ct__Q4_2nw3snd8internal20RemoteSpeakerManagerFv
    RemoteSpeakerManager();
    //! Initialize__Q4_2nw3snd8internal20RemoteSpeakerManagerFv
    void Initialize();
    //! Finalize__Q4_2nw3snd8internal20RemoteSpeakerManagerFv
    void Finalize();
    //! GetRemoteSpeaker__Q4_2nw3snd8internal20RemoteSpeakerManagerFi
    RemoteSpeaker *GetRemoteSpeaker(WPADChan chan);
    //! Static Methods
public:
    //! GetInstance__Q4_2nw3snd8internal20RemoteSpeakerManagerSFv
    static RemoteSpeakerManager *GetInstance();
    //! RemoteSpeakerAlarmProc__Q4_2nw3snd8internal20RemoteSpeakerManagerSFP7OSAlarmP9OSContext
    static void RemoteSpeakerAlarmProc(OSAlarm *alarm, OSContext *context);
    //! Fields
public:
    bool m_initialized;
    WUT_UNKNOWN_BYTES(7);
    OSAlarm m_alarm;
    RemoteSpeaker m_speakers[4];
    // Non-exports
private:
    static RemoteSpeakerManager s_instance;
    static BOOL s_initialized;
};

WUT_CHECK_OFFSET(RemoteSpeakerManager, 0x00, m_initialized);
WUT_CHECK_OFFSET(RemoteSpeakerManager, 0x08, m_alarm);
WUT_CHECK_OFFSET(RemoteSpeakerManager, 0x60, m_speakers);
WUT_CHECK_SIZE(RemoteSpeakerManager, 0x440);
}// namespace internal
}// namespace snd
}// namespace nw