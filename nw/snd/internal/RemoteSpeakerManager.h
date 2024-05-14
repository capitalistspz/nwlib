#pragma once
#include <snd/RemoteSpeaker.h>

namespace nw
{
    namespace snd
    {
        class RemoteSpeakerManager {
            RemoteSpeakerManager();
        public:
            void Initialize();
            RemoteSpeaker* GetRemoteSpeaker(WPADChan chan);
            static RemoteSpeakerManager* GetInstance();
            static void RemoteSpeakerAlarmProc(OSAlarm* alarm, OSContext* context);

        private:
            static RemoteSpeakerManager s_instance;
            static BOOL s_initialized;
        private:
            bool m_initialized;
            uint8_t field1;
            WUT_UNKNOWN_BYTES(6);
            OSAlarm m_alarm;
            RemoteSpeaker m_speakers[4];
        };

    }
}