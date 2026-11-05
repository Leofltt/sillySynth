#include "🎨.h"

namespace 🎵 {

🎨::🎨(🎛️& 🧠_proc)
    : AudioProcessorEditor(&🧠_proc),
      🧠(🧠_proc),
      🎹_keyboard(🧠_proc.🎹(), juce::MidiKeyboardComponent::horizontalKeyboard) {

    // Waveform selector 🌊
    🌊_selector.addItemList({ "〰️ Sine"_💬, "📐 Triangle"_💬, "🪚 Saw"_💬, "🟦 Square"_💬, "🎲 Noise"_💬 }, 1);
    addAndMakeVisible(🌊_selector);
    🌊_attachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        🧠.🌳(), "waveform", 🌊_selector
    );

    // Top row: tone & modulation knobs
    🛠️_setupKnob(✂️_cutoff,    "cutoff",    "✂️ Cutoff"_💬);
    🛠️_setupKnob(🔔_resonance, "resonance", "🔔 Reso"_💬);
    🛠️_setupKnob(🐍_vibrato,   "vibrato",   "🐍 Vibrato"_💬);
    🛠️_setupKnob(🔊_gain,      "gain",      "🔊 Gain"_💬);

    // Bottom row: ADSR envelope knobs
    🛠️_setupKnob(⚡_attack,  "attack",  "⚡ Attack"_💬);
    🛠️_setupKnob(💧_decay,   "decay",   "💧 Decay"_💬);
    🛠️_setupKnob(🧘_sustain, "sustain", "🧘 Sustain"_💬);
    🛠️_setupKnob(🎈_release, "release", "🎈 Release"_💬);

    addAndMakeVisible(🎹_keyboard);
    🎹_keyboard.setAvailableRange(36, 84);

    setSize(720, 460);
    startTimerHz(30);
}

🎨::~🎨() {
    stopTimer();
}

void 🎨::🛠️_setupKnob(🎚️_Knob& 🎛️_knob, const juce::String& 🆔, const juce::String& 💬) {
    🎛️_knob.🎡.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    🎛️_knob.🎡.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 68, 18);
    addAndMakeVisible(🎛️_knob.🎡);

    🎛️_knob.🏷️.setText(💬, juce::dontSendNotification);
    🎛️_knob.🏷️.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(🎛️_knob.🏷️);

    🎛️_knob.🔗 = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        🧠.🌳(), 🆔, 🎛️_knob.🎡
    );
}

void 🎨::timerCallback() {
    🧠.📺_copyScope(📺_samples);
    repaint(📺_scopeBounds);
}

void 🎨::paint(juce::Graphics& 🖌️) {
    🖌️.fillAll(juce::Colour(0xff161622));

    // Header bar
    🖌️.setColour(juce::Colours::white);
    🖌️.setFont(juce::FontOptions(22.0f, juce::Font::bold));
    🖌️.drawText("🤪🎹 SillySynth "_💬, 20, 12, 420, 32, juce::Justification::centredLeft);

    // Oscilloscope panel 📺
    🖌️.setColour(juce::Colour(0xff0d0d15));
    🖌️.fillRoundedRectangle(📺_scopeBounds.toFloat(), 8.0f);
    🖌️.setColour(juce::Colour(0xff3a3a5c));
    🖌️.drawRoundedRectangle(📺_scopeBounds.toFloat(), 8.0f, 1.5f);

    if (!📺_scopeBounds.isEmpty()) {
        juce::Path 📈_path;
        const float 📏_w = static_cast<float>(📺_scopeBounds.getWidth());
        const float 📏_h = static_cast<float>(📺_scopeBounds.getHeight());
        const float 🎯_midY = static_cast<float>(📺_scopeBounds.getCentreY());

        for (size_t 🔢 = 0; 🔢 < 📺_samples.size(); ++🔢) {
            const float 📍_x = static_cast<float>(📺_scopeBounds.getX())
                             + (static_cast<float>(🔢) / static_cast<float>(📺_samples.size() - 1)) * 📏_w;
            const float 📍_y = 🎯_midY - std::clamp(📺_samples[🔢], -1.0f, 1.0f) * (📏_h * 0.42f);
            if (🔢 == 0)
                📈_path.startNewSubPath(📍_x, 📍_y);
            else
                📈_path.lineTo(📍_x, 📍_y);
        }

        🖌️.setColour(juce::Colour(0xff50fa7b));
        🖌️.strokePath(📈_path, juce::PathStrokeType(2.0f));
    }
}

void 🎨::resized() {
    auto 📐_area = getLocalBounds().reduced(16);

    // Top bar: Title + Waveform selector
    auto 🎩_top = 📐_area.removeFromTop(40);
    🌊_selector.setBounds(🎩_top.removeFromRight(180).reduced(0, 4));

    📐_area.removeFromTop(8);

    // Live Oscilloscope 📺
    📺_scopeBounds = 📐_area.removeFromTop(84);

    📐_area.removeFromTop(10);

    // MIDI Keyboard at the bottom 🎹
    🎹_keyboard.setBounds(📐_area.removeFromBottom(80));

    📐_area.removeFromBottom(10);

    // Two rows of 4 knobs
    auto 🧱_row1 = 📐_area.removeFromTop(📐_area.getHeight() / 2);
    auto 🧱_row2 = 📐_area;

    auto 📐_placeKnob = [](🎚️_Knob& 🎛️_k, juce::Rectangle<int> 📦_cell) {
        🎛️_k.🏷️.setBounds(📦_cell.removeFromTop(20));
        🎛️_k.🎡.setBounds(📦_cell);
    };

    const int 📏_colW = 🧱_row1.getWidth() / 4;
    📐_placeKnob(✂️_cutoff,    🧱_row1.removeFromLeft(📏_colW));
    📐_placeKnob(🔔_resonance, 🧱_row1.removeFromLeft(📏_colW));
    📐_placeKnob(🐍_vibrato,   🧱_row1.removeFromLeft(📏_colW));
    📐_placeKnob(🔊_gain,      🧱_row1);

    📐_placeKnob(⚡_attack,  🧱_row2.removeFromLeft(📏_colW));
    📐_placeKnob(💧_decay,   🧱_row2.removeFromLeft(📏_colW));
    📐_placeKnob(🧘_sustain, 🧱_row2.removeFromLeft(📏_colW));
    📐_placeKnob(🎈_release, 🧱_row2);
}

} // namespace 🎵
