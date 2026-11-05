#pragma once

#include <juce_audio_utils/juce_audio_utils.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include "🎛️.h"

namespace 🎵 {

// ============================================================================
// 🎨 Emoji-Driven Plugin Editor UI
// ============================================================================
class 🎨 : public juce::AudioProcessorEditor, private juce::Timer {
public:
    explicit 🎨(🎛️& 🧠_proc);
    ~🎨() override;

    void paint(juce::Graphics& 🖌️) override;
    void resized() override;

private:
    void timerCallback() override;

    struct 🎚️_Knob {
        juce::Slider 🎡;
        juce::Label 🏷️;
        std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> 🔗;
    };

    void 🛠️_setupKnob(🎚️_Knob& 🎛️_knob, const juce::String& 🆔, const juce::String& 💬);

    🎛️& 🧠;

    juce::ComboBox 🌊_selector;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> 🌊_attachment;

    🎚️_Knob ✂️_cutoff;
    🎚️_Knob 🔔_resonance;
    🎚️_Knob 🐍_vibrato;
    🎚️_Knob 🔊_gain;

    🎚️_Knob ⚡_attack;
    🎚️_Knob 💧_decay;
    🎚️_Knob 🧘_sustain;
    🎚️_Knob 🎈_release;

    juce::MidiKeyboardComponent 🎹_keyboard;
    juce::Rectangle<int> 📺_scopeBounds;
    std::array<float, 🎛️::📺_ScopeSize> 📺_samples{};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(🎨)
};

} // namespace 🎵
