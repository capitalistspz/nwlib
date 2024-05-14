#pragma once
#include <coreinit/alarm.h>
#include <coreinit/context.h>
#include <padscore/wpad.h>
#include "../notyetwut/padscore.h"

namespace nw {
namespace snd {
    class RemoteSpeaker {
        friend class RemoteSpeakerManager;
    public:
        using SpeakerCallback = void(*)(int32_t, int32_t);
        enum class SpeakerCommand : uint32_t {
            NONE = 0,
            INITIALIZE = 1,
            PLAY = 2,
            FINALIZE = 3
        };
    public:
        //! _ct_Q3_2nw3snd13RemoteSpeakerFv
        RemoteSpeaker();
        //! Finalize__Q3_2nw3snd13RemoteSpeakerFPFiT1_v
        void Finalize(SpeakerCallback callback);
        //! Initialize__Q3_2nw3snd13RemoteSpeakerFPFiT1_v
        void Initialize(SpeakerCallback callback);

        //! EnableOutput_Q3_2nw3snd13RemoteSpeakerFb
        BOOL EnableOutput(bool enable);
        //! IsEnabledOutput__Q3_2nw3snd13RemoteSpeakerFCv
        bool IsEnabledOutput() const;

        //!__CPR80__ExecCommand__Q3_2nw3snd13RemoteSpeakerFQ4_nw3sndJ23J14SpeakerCommand
        void ExecCommand(SpeakerCommand command);

        //! IsPlaying__Q3_2nw3snd13RemoteSpeakerFCv
        bool IsPlaying() const;
        //! GetContinuousPlayTime__Q3_2nw3snd13RemoteSpeakerCFv
        uint32_t GetContinuousPlayTime() const;

        //! Update_Q3_2nw3snd13RemoteSpeakerFv
        void Update();
        //! UpdateStreamData_Q3_2nw3snd13RemoteSpeakerFCs
        void UpdateStreamData(const short* samples);
        //! ClearParam_Q3_2nw3snd13RemoteSpeakerFv
        void ClearParam();

        void InitParam();

        //! IsAllSampleZero__Q3_2nw3snd13RemoteSpeakerFPCs
        BOOL IsAllSampleZero(const short* samples) const;

        //! NotifyCallback__Q3_2nw3snd13RemoteSpeakerFiT1
        void NotifyCallback(WPADChan chan, int32_t result);

        //! ContinueAlarmHandler_Q3_2nw3snd13RemoteSpeakerSFP7OSAlarmP9OSContext
        static void ContinueAlarmHandler(OSAlarm* alarm, OSContext*);
        //! IntervalAlarmHandler_Q3_2nw3snd13RemoteSpeakerSFP7OSAlarmP9OSContext
        static void IntervalAlarmHandler(OSAlarm* alarm, OSContext*);
        //! SpeakerOnCallback__Q3_2nw3snd13RemoteSpeakerSFiT1
        static void SpeakerOnCallback(WPADChan chan, int32_t result);
        //! SpeakerOffCallback__Q3_2nw3snd13RemoteSpeakerSFiT1
        static void SpeakerOffCallback(WPADChan chan, int32_t result);
        //! SpeakerPlayCallback__Q3_2nw3snd13RemoteSpeakerSFiT1
        static void SpeakerPlayCallback(WPADChan chan, int32_t result);
    private:
        enum class SpeakerMode : uint32_t
        {
            UNKNOWN = 0,
            ON = 2,
            READY = 4,
            OFF = 6
        };

        bool m_initialized{false};
        bool m_isPlaying;
        bool m_outputEnabled{false};
        bool m_firstStream{false};
        bool m_notificationAvailable{false};
        bool m_cmdInProgress{false};
        bool m_continueAlarmRunning{false};
        bool m_intervalAlarmRunning{false};
        SpeakerMode m_mode{SpeakerMode::UNKNOWN};
        SpeakerCommand m_priorityCmd{SpeakerCommand::NONE};
        SpeakerCommand m_nextCmd{SpeakerCommand::NONE};
        WENCParams m_encodeParams;
        WUT_UNKNOWN_BYTES(4);
        WPADChan m_channel;
        SpeakerCallback m_callback{nullptr};
        WUT_UNKNOWN_BYTES(4);
        OSAlarm m_continueAlarm;
        OSAlarm m_intervalAlarm;
        OSTime m_playStartTime;
    };

    WUT_CHECK_SIZE(RemoteSpeaker, 0xf8);
}
}
