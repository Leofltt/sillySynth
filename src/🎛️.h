#pragma once

#include <array>
#include <atomic>
#include <juce_audio_processors/juce_audio_processors.h>
#include "🔊.h"

namespace 🎵 {

// UTF-8 Emoji String Literal for JUCE (bypasses JUCE's 7-bit ASCII const char* constructor!)
inline juce::String operator""_💬(const char* 🔤, size_t 📏) {
    return juce::String::fromUTF8(🔤, static_cast<int>(📏));
}

// ============================================================================
// 🎛️ JUCE AudioProcessor for SillySynth 🤪🎹
// ============================================================================
class 🎛️ : public juce::AudioProcessor {
public:
    🎛️();
    ~🎛️() override = default;

    void prepareToPlay(double ⏱️, int 📦_samples) override;
    void releaseResources() override {}
    bool isBusesLayoutSupported(const BusesLayout& 🚌) const override;
    void processBlock(juce::AudioBuffer<float>& 🪣, juce::MidiBuffer& 🎹_midi) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }

    const juce::String getName() const override { return "SillySynth"; }
    bool acceptsMidi() const override { return true; }
    bool producesMidi() const override { return false; }
    bool isMidiEffect() const override { return false; }
    double getTailLengthSeconds() const override { return 0.5; }

    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram(int) override {}
    const juce::String getProgramName(int) override { return "🤪 Default Emoji Patch"_💬; }
    void changeProgramName(int, const juce::String&) override {}

    void getStateInformation(juce::MemoryBlock& 💾) override;
    void setStateInformation(const void* 📥, int 📏) override;

    juce::AudioProcessorValueTreeState& 🌳() { return 🌳_apvts; }
    juce::MidiKeyboardState& 🎹() { return 🔊_engine.🎹(); }

    // 📺 Lock-free oscilloscope ring buffer for the UI
    static constexpr size_t 📺_ScopeSize = 512;
    void 📺_copyScope(std::array<float, 📺_ScopeSize>& 📤) const;

private:
    static juce::AudioProcessorValueTreeState::ParameterLayout 🗺️_createLayout();
    🎛️_Patch 📸_readPatch() const;

    🔊 🔊_engine{ 8 };
    juce::AudioProcessorValueTreeState 🌳_apvts;

    std::array<float, 📺_ScopeSize> 📺_buffer{};
    std::atomic<size_t> 📺_writePos{ 0 };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(🎛️)
};

} // namespace 🎵
