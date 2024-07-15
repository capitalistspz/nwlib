#include "RemoteSpeaker.h"
#include "snd/internal/RemoteSpeakerManager.h"
#include <coreinit/memory.h>


nw::snd::RemoteSpeaker::RemoteSpeaker() {
    OSCreateAlarm(&m_continueAlarm);
    OSSetAlarmUserData(&m_continueAlarm, this);
    OSCreateAlarm(&m_intervalAlarm);
    OSSetAlarmUserData(&m_intervalAlarm, this);
}

void nw::snd::RemoteSpeaker::Initialize(nw::snd::RemoteSpeaker::SpeakerCallback callback) {
    if (m_callback) {
        m_callback(m_channel, 0);
        m_notificationAvailable = false;
    }
    m_initialized = true;
    m_callback = callback;
    m_priorityCmd = SpeakerCommand::INITIALIZE;
}

void nw::snd::RemoteSpeaker::Finalize(SpeakerCallback callback) {
    if (m_callback) {
        m_callback(m_channel, 0);
        m_notificationAvailable = false;
    }
    m_initialized = false;
    m_callback = callback;
    m_priorityCmd = SpeakerCommand::FINALIZE;
}

BOOL nw::snd::RemoteSpeaker::EnableOutput(bool enable) {
    if (!m_initialized)
        return FALSE;
    m_outputEnabled = true;
    RemoteSpeaker* example;
    return true;
}

bool nw::snd::RemoteSpeaker::IsEnabledOutput() const {
    if (!m_initialized)
        return false;
    return m_outputEnabled;
}

void nw::snd::RemoteSpeaker::ExecCommand(SpeakerCommand command) {
    if (command == SpeakerCommand::NONE)
        return;
    if (command == SpeakerCommand::INITIALIZE) {
        m_cmdInProgress = true;
        m_mode = SpeakerMode::REQUESTING_ON;
        m_notificationAvailable = true;
        WPADControlSpeaker(m_channel, WPAD_SPEAKER_CMD_ON, SpeakerOnCallback);
        return;
    }
    if (command < SpeakerCommand::FINALIZE) {
        m_cmdInProgress = true;
        m_notificationAvailable = true;
        m_mode = SpeakerMode::REQUESTING_PLAY;
        WPADControlSpeaker(m_channel, WPAD_SPEAKER_CMD_PLAY, SpeakerPlayCallback);
        return;
    }
    if (command != SpeakerCommand::FINALIZE)
        return;
    m_cmdInProgress = true;
    m_notificationAvailable = true;
    m_mode = SpeakerMode::REQUESTING_OFF;
    WPADControlSpeaker(m_channel, WPAD_SPEAKER_CMD_OFF, SpeakerOffCallback);
}

bool nw::snd::RemoteSpeaker::IsPlaying() const {
    if (m_mode != SpeakerMode::PLAY)
        return false;
    return m_playing;
}

uint32_t nw::snd::RemoteSpeaker::GetContinuousPlayTime() const {
    if (m_mode != SpeakerMode::PLAY && m_continueAlarmRunning) {
        const auto currentTime =  OSGetTime();
        return OSTicksToMilliseconds(currentTime - m_playStartTime);
    }
    return 0;
}


BOOL nw::snd::RemoteSpeaker::IsAllSampleZero(const short* samples) const {
    for (auto i = 0; i < 20; ++i, samples += 2) {
        if (*reinterpret_cast<const int*>(samples) != 0) {
            return FALSE;
        }
    }
    return TRUE;
}

void nw::snd::RemoteSpeaker::ClearParam() {
    m_playing = false;
    m_outputEnabled = false;
    OSCancelAlarm(&m_continueAlarm);
    m_continueAlarmRunning = false;
    OSCancelAlarm(&m_intervalAlarm);
    m_intervalAlarmRunning = false;
}

void nw::snd::RemoteSpeaker::InitParam() {
    ClearParam();
    m_continueAlarmRunning = false;
    m_intervalAlarmRunning = false;
    m_outputEnabled = true;
    m_playing = false;
}

void nw::snd::RemoteSpeaker::NotifyCallback(WPADChan chan, int32_t result) {
    if (m_notificationAvailable && m_callback) {
        m_callback(chan, result);
    }
}

void nw::snd::RemoteSpeaker::ContinueAlarmHandler(OSAlarm* alarm, OSContext* context) {
    // Yes it actually does nothing with it
    OSGetAlarmUserData(alarm);
}

void nw::snd::RemoteSpeaker::IntervalAlarmHandler(OSAlarm* alarm, OSContext*) {
    auto* speaker = reinterpret_cast<RemoteSpeaker*>(OSGetAlarmUserData(alarm));
    if (speaker->m_intervalAlarmRunning) {
        OSCancelAlarm(&speaker->m_continueAlarm);
        speaker->m_continueAlarmRunning = false;
    }
    speaker->m_intervalAlarmRunning = false;
}

void nw::snd::RemoteSpeaker::SpeakerOnCallback(WPADChan chan, int32_t result) {
    auto* speakerManager = internal::RemoteSpeakerManager::GetInstance();
    auto* speaker = speakerManager->GetRemoteSpeaker(chan);
    if (result == 0) {
        speaker->m_firstStream = true;
        OSBlockSet(&speaker->m_encodeParams, 0, 32);
        speaker->m_nextCmd = SpeakerCommand::PLAY;
        speaker->m_mode = SpeakerMode::ON;
    } else if (result == -2) {
        speaker->m_nextCmd = SpeakerCommand::INITIALIZE;
    } else {
        speaker->m_mode = SpeakerMode::UNKNOWN;
        speaker->NotifyCallback(chan, result);
    }
    speaker->m_cmdInProgress = false;
}

void nw::snd::RemoteSpeaker::SpeakerOffCallback(WPADChan chan, int32_t result) {
    auto* speakerManager = internal::RemoteSpeakerManager::GetInstance();
    auto* speaker = speakerManager->GetRemoteSpeaker(chan);
    if (result == -2) {
        speaker->m_nextCmd = SpeakerCommand::FINALIZE;
    } else {
        speaker->m_mode = result == 0 ? SpeakerMode::OFF : SpeakerMode::UNKNOWN;
        speaker->NotifyCallback(chan, 0);
    }
    speaker->m_cmdInProgress = false;
}

void nw::snd::RemoteSpeaker::SpeakerPlayCallback(WPADChan chan, int32_t result) {
    auto* speakerManager = internal::RemoteSpeakerManager::GetInstance();
    auto* speaker = speakerManager->GetRemoteSpeaker(chan);
    if (result == -2) {
        speaker->m_nextCmd = SpeakerCommand::PLAY;
    } else {
        speaker->m_mode = result == 0 ? SpeakerMode::PLAY : SpeakerMode::UNKNOWN;
        speaker->NotifyCallback(chan, 0);
    }
    speaker->m_cmdInProgress = false;
}

void nw::snd::RemoteSpeaker::Update() {
    if (m_cmdInProgress)
        return;
    auto command = m_priorityCmd;
    if (command == SpeakerCommand::NONE)
        command = m_nextCmd;
    this->m_nextCmd = SpeakerCommand::NONE;
    this->m_priorityCmd = SpeakerCommand::NONE;
    ExecCommand(command);
}

void nw::snd::RemoteSpeaker::UpdateStreamData(const int16_t* samples) {
    uint8_t adpcmData[20];
    if (m_mode != SpeakerMode::PLAY)
        return;
    bool shouldStreamData = true;
    if (!m_outputEnabled || IsAllSampleZero(samples))
        shouldStreamData = false;
    bool startContinueAlarm = false;
    if (!m_playing && shouldStreamData)
        startContinueAlarm = true;
    bool startIntervalAlarm = false;
    if (m_playing & !shouldStreamData)
        startIntervalAlarm = true;
    if (shouldStreamData) {
        BOOL canSendStreamData = WPADCanSendStreamData(m_channel);
        if (canSendStreamData == FALSE)
            return;
        bool continuing = m_firstStream;
        m_firstStream = false;
        WENCGetEncodeData(&m_encodeParams, continuing ^ 1, samples, 40, adpcmData);
        auto result = WPADSendStreamData(m_channel, adpcmData, 20);
        if (result != 0) {
            m_mode = SpeakerMode::UNKNOWN;
            m_nextCmd = SpeakerCommand::INITIALIZE;
            InitParam();
            return;
        }
    }
    if (startContinueAlarm) {
        if (!m_continueAlarmRunning) {
            OSSetAlarm(&m_continueAlarm, OSSecondsToTicks(480), ContinueAlarmHandler);
            m_playStartTime = OSGetTime();
            m_continueAlarmRunning = true;
        }
        OSCancelAlarm(&m_intervalAlarm);
        m_intervalAlarmRunning = false;
    }
    if (startIntervalAlarm) {
        m_intervalAlarmRunning = true;
        OSCancelAlarm(&m_intervalAlarm);
        OSSetAlarm(&m_intervalAlarm, OSSecondsToTicks(1), IntervalAlarmHandler);
    }
    m_playing = shouldStreamData;
}
