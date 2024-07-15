#include "RemoteSpeakerManager.h"
#include "../notyetwut/sndcore2.h"

nw::snd::internal::RemoteSpeakerManager::RemoteSpeakerManager() {
    for (auto i = 0; i < 4; ++i) {
        m_speakers[i].m_channel = static_cast<WPADChan>(i);
    }
}

void nw::snd::internal::RemoteSpeakerManager::Initialize() {
    if (m_initialized)
        return;
    OSCreateAlarm(&m_alarm);
    const auto currentTime = OSGetTime();
    OSSetPeriodicAlarm(&m_alarm, currentTime, OSNanosecondsToTicks(6666667), RemoteSpeakerAlarmProc);
    m_initialized = true;
}

void nw::snd::internal::RemoteSpeakerManager::Finalize() {
    if (m_initialized) {
        OSCancelAlarm(&m_alarm);
        m_initialized = false;
    }
}

void nw::snd::internal::RemoteSpeakerManager::RemoteSpeakerAlarmProc(OSAlarm* alarm, OSContext* context) {
    auto* manager = GetInstance();
    const auto samplesLeft = AXRmtGetSamplesLeft();
    int16_t samples[40];
    if (samplesLeft < 40)
        return;
    for (auto i = 0; i < 4; ++i) {
        if (manager->m_speakers[i].m_mode == RemoteSpeaker::SpeakerMode::PLAY) {
            AXRmtGetSamples(i, samples, 40);
            manager->m_speakers[i].UpdateStreamData(samples);
        }
        manager->m_speakers[i].Update();
    }
    AXRmtAdvancePtr(40);
}

nw::snd::internal::RemoteSpeakerManager* nw::snd::internal::RemoteSpeakerManager::GetInstance() {
    if (!s_initialized) {
        s_initialized = true;
        s_instance = RemoteSpeakerManager();
    }
    return &s_instance;
}

nw::snd::RemoteSpeaker* nw::snd::internal::RemoteSpeakerManager::GetRemoteSpeaker(WPADChan chan) {
    return m_speakers + chan;
}
