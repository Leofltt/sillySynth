#pragma once

#include <juce_audio_basics/juce_audio_basics.h>
#include "🌊.h"

namespace 🎵 {

// ============================================================================
// 🎛️ Synth Patch State (Shared across Voices)
// ============================================================================
struct 🎛️_Patch {
    🌊 🎭 = 🌊::📐;          // Primary waveform (defaults to Triangle 📐)
    float ✂️ = 2400.0f;      // Filter cutoff (Hz)
    float 🔔 = 0.8f;         // Filter resonance
    float 🐍 = 0.15f;        // Vibrato amount
    float 🔊 = 0.5f;         // Master level
    ✉️ ✉️_adsr{};            // ADSR parameters
};

// ============================================================================
// 🔔 JUCE SynthesiserSound
// ============================================================================
struct 🔔 : public juce::SynthesiserSound {
    bool appliesToNote(int) override { return true; }
    bool appliesToChannel(int) override { return true; }
};

// ============================================================================
// 🗣️ Polyphonic Emoji Synth Voice
// ============================================================================
class 🗣️ : public juce::SynthesiserVoice {
public:
    🗣️();

    bool canPlaySound(juce::SynthesiserSound* 🎵_sound) override;
    void startNote(int 🎹_note, float 💪_vel, juce::SynthesiserSound*, int) override;
    void stopNote(float 💪_vel, bool 💨_tail) override;
    void pitchWheelMoved(int) override {}
    void controllerMoved(int, int) override {}
    void prepareToPlay(double ⏱️, int 📦_samples);
    void 📥(const 🎛️_Patch& 🎁);

    using juce::SynthesiserVoice::renderNextBlock;
    void renderNextBlock(juce::AudioBuffer<float>& 🪣, int 🏁_start, int 📏_num) override;

private:
    // ZWJ compound emoji identifier for our voice performer state!
    struct 👨‍🎤 {
        double 🎼_baseHz = 440.0_🎵;
        float 💪_level = 0.0f;
    } 👨‍🎤_state;

    🌀 🌀_main;              // Main oscillator
    🌀 🌀_sub;               // Sub-oscillator (-1 octave sine 〰️)
    🌀 🐍_lfo;               // Vibrato LFO
    🕳️ 🕳️_filter;            // State-variable filter
    juce::ADSR 📈_env;       // Amplitude envelope
    🎛️_Patch 🎁_patch;       // Current patch parameters
};

// ============================================================================
// 🔊 Polyphonic Synth Engine
// ============================================================================
class 🔊 {
public:
    explicit 🔊(int 👥_voices = 8);

    void 🎯(double ⏱️, int 📦_samples);
    void 📥(const 🎛️_Patch& 🎁);
    void 🔊_render(juce::AudioBuffer<float>& 🪣, juce::MidiBuffer& 🎹_midi);

    juce::MidiKeyboardState& 🎹() { return 🎹_state; }

private:
    juce::Synthesiser 🎻_synth;
    juce::MidiKeyboardState 🎹_state;
};

} // namespace 🎵
