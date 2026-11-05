#include "🔊.h"

namespace 🎵 {

🗣️::🗣️() {
    🐍_lfo.🎵(5.5_🎵);
}

bool 🗣️::canPlaySound(juce::SynthesiserSound* 🎵_sound) {
    return dynamic_cast<🔔*>(🎵_sound) != nullptr;
}

void 🗣️::startNote(int 🎹_note, float 💪_vel, juce::SynthesiserSound*, int) {
    👨‍🎤_state.🎼_baseHz = juce::MidiMessage::getMidiNoteInHertz(🎹_note);
    👨‍🎤_state.💪_level = 💪_vel;

    🌀_main.🧹();
    🌀_sub.🧹();
    🌀_main.🎵(👨‍🎤_state.🎼_baseHz);
    🌀_sub.🎵(👨‍🎤_state.🎼_baseHz * 0.5);

    🕳️_filter.🧹();
    📈_env.noteOn();
}

void 🗣️::stopNote(float, bool 💨_tail) {
    if (💨_tail) {
        📈_env.noteOff();
    } else {
        📈_env.reset();
        clearCurrentNote();
    }
}

void 🗣️::prepareToPlay(double ⏱️, int) {
    🌀_main.🎯(⏱️);
    🌀_sub.🎯(⏱️);
    🐍_lfo.🎯(⏱️);
    🐍_lfo.🎵(5.5_🎵);
    🕳️_filter.🎯(⏱️);
    📈_env.setSampleRate(⏱️);
}

void 🗣️::📥(const 🎛️_Patch& 🎁) {
    🎁_patch = 🎁;

    juce::ADSR::Parameters ✉️_params;
    ✉️_params.attack  = 🎁.✉️_adsr.⚡;
    ✉️_params.decay   = 🎁.✉️_adsr.💧;
    ✉️_params.sustain = 🎁.✉️_adsr.🧘;
    ✉️_params.release = 🎁.✉️_adsr.🎈;
    📈_env.setParameters(✉️_params);
}

void 🗣️::renderNextBlock(juce::AudioBuffer<float>& 🪣, int 🏁_start, int 📏_num) {
    if (!isVoiceActive())
        return;

    for (int 🔢 = 0; 🔢 < 📏_num; ++🔢) {
        const float 📉_envVal = 📈_env.getNextSample();

        // Vibrato modulation via 🐍_lfo
        const float 🐍_mod = 🐍_lfo.🔊(🌊::〰️) * 🎁_patch.🐍 * 0.03f;
        const double 🎼_modHz = 👨‍🎤_state.🎼_baseHz * (1.0 + static_cast<double>(🐍_mod));
        🌀_main.🎵(🎼_modHz);
        🌀_sub.🎵(🎼_modHz * 0.5);

        // Main waveform + subtle sub-oscillator
        const float 🌊_raw = 0.8f * 🌀_main.🔊(🎁_patch.🎭) + 0.2f * 🌀_sub.🔊(🌊::〰️);

        // Envelope-modulated filter cutoff
        const float ✂️_mod = 🎁_patch.✂️ * (0.35f + 0.65f * 📉_envVal);
        🕳️_filter.🎛️(✂️_mod, 🎁_patch.🔔, 🚦::🟢);

        const float 🔊_sample = 🕳️_filter.🔊(🌊_raw) * 📉_envVal * 👨‍🎤_state.💪_level * 🎁_patch.🔊;

        for (int 📺_ch = 0; 📺_ch < 🪣.getNumChannels(); ++📺_ch) {
            🪣.addSample(📺_ch, 🏁_start + 🔢, 🔊_sample);
        }

        if (!📈_env.isActive()) {
            clearCurrentNote();
            break;
        }
    }
}

// ============================================================================
// 🔊 Synth Engine Implementation
// ============================================================================
🔊::🔊(int 👥_voices) {
    for (int 🔢 = 0; 🔢 < 👥_voices; ++🔢) {
        🎻_synth.addVoice(new 🗣️());
    }
    🎻_synth.addSound(new 🔔());
}

void 🔊::🎯(double ⏱️, int 📦_samples) {
    🎻_synth.setCurrentPlaybackSampleRate(⏱️);
    for (int 🔢 = 0; 🔢 < 🎻_synth.getNumVoices(); ++🔢) {
        if (auto* 🗣️_voice = dynamic_cast<🗣️*>(🎻_synth.getVoice(🔢))) {
            🗣️_voice->prepareToPlay(⏱️, 📦_samples);
        }
    }
}

void 🔊::📥(const 🎛️_Patch& 🎁) {
    for (int 🔢 = 0; 🔢 < 🎻_synth.getNumVoices(); ++🔢) {
        if (auto* 🗣️_voice = dynamic_cast<🗣️*>(🎻_synth.getVoice(🔢))) {
            🗣️_voice->📥(🎁);
        }
    }
}

void 🔊::🔊_render(juce::AudioBuffer<float>& 🪣, juce::MidiBuffer& 🎹_midi) {
    🎹_state.processNextMidiBuffer(🎹_midi, 0, 🪣.getNumSamples(), true);
    🎻_synth.renderNextBlock(🪣, 🎹_midi, 0, 🪣.getNumSamples());
}

} // namespace 🎵
