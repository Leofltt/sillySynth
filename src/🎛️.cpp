#include "🎛️.h"
#include "🎨.h"

namespace 🎵 {

🎛️::🎛️()
    : AudioProcessor(BusesProperties().withOutput("Output 🔊"_💬, juce::AudioChannelSet::stereo(), true)),
      🌳_apvts(*this, nullptr, "SillySynthState", 🗺️_createLayout()) {
}

juce::AudioProcessorValueTreeState::ParameterLayout 🎛️::🗺️_createLayout() {
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> 📋;

    📋.push_back(std::make_unique<juce::AudioParameterChoice>(
        juce::ParameterID{ "waveform", 1 },
        "Waveform 🌊"_💬,
        juce::StringArray{ "〰️ Sine"_💬, "📐 Triangle"_💬, "🪚 Saw"_💬, "🟦 Square"_💬, "🎲 Noise"_💬 },
        1 // Default: 📐 Triangle
    ));

    📋.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{ "cutoff", 1 },
        "Cutoff ✂️"_💬,
        juce::NormalisableRange<float>(80.0f, 14000.0f, 1.0f, 0.35f),
        2600.0f
    ));

    📋.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{ "resonance", 1 },
        "Resonance 🔔"_💬,
        juce::NormalisableRange<float>(0.3f, 8.0f, 0.01f, 0.5f),
        1.1f
    ));

    📋.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{ "vibrato", 1 },
        "Vibrato 🐍"_💬,
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f),
        0.15f
    ));

    📋.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{ "attack", 1 },
        "Attack ⚡"_💬,
        juce::NormalisableRange<float>(0.002f, 2.0f, 0.001f, 0.4f),
        0.015f
    ));

    📋.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{ "decay", 1 },
        "Decay 💧"_💬,
        juce::NormalisableRange<float>(0.01f, 2.5f, 0.001f, 0.4f),
        0.20f
    ));

    📋.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{ "sustain", 1 },
        "Sustain 🧘"_💬,
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f),
        0.70f
    ));

    📋.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{ "release", 1 },
        "Release 🎈"_💬,
        juce::NormalisableRange<float>(0.01f, 4.0f, 0.001f, 0.4f),
        0.35f
    ));

    📋.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{ "gain", 1 },
        "Master 🔊"_💬,
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f),
        0.55f
    ));

    return { 📋.begin(), 📋.end() };
}

void 🎛️::prepareToPlay(double ⏱️, int 📦_samples) {
    🔊_engine.🎯(⏱️, 📦_samples);
}

bool 🎛️::isBusesLayoutSupported(const BusesLayout& 🚌) const {
    const auto& 📤 = 🚌.getMainOutputChannelSet();
    return 📤 == juce::AudioChannelSet::mono() || 📤 == juce::AudioChannelSet::stereo();
}

🎛️_Patch 🎛️::📸_readPatch() const {
    🎛️_Patch 🎁;
    const int 🎭_idx = static_cast<int>(🌳_apvts.getRawParameterValue("waveform")->load());
    🎁.🎭 = static_cast<🌊>(std::clamp(🎭_idx, 0, 4));
    🎁.✂️ = 🌳_apvts.getRawParameterValue("cutoff")->load();
    🎁.🔔 = 🌳_apvts.getRawParameterValue("resonance")->load();
    🎁.🐍 = 🌳_apvts.getRawParameterValue("vibrato")->load();
    🎁.🔊 = 🌳_apvts.getRawParameterValue("gain")->load();
    🎁.✉️_adsr.⚡ = 🌳_apvts.getRawParameterValue("attack")->load();
    🎁.✉️_adsr.💧 = 🌳_apvts.getRawParameterValue("decay")->load();
    🎁.✉️_adsr.🧘 = 🌳_apvts.getRawParameterValue("sustain")->load();
    🎁.✉️_adsr.🎈 = 🌳_apvts.getRawParameterValue("release")->load();
    return 🎁;
}

void 🎛️::processBlock(juce::AudioBuffer<float>& 🪣, juce::MidiBuffer& 🎹_midi) {
    juce::ScopedNoDenormals 🚫;
    🪣.clear();

    🔊_engine.📥(📸_readPatch());
    🔊_engine.🔊_render(🪣, 🎹_midi);

    // Feed channel 0 into our oscilloscope ring buffer 📺
    if (🪣.getNumChannels() > 0) {
        const float* 🌊_ch0 = 🪣.getReadPointer(0);
        size_t 📍 = 📺_writePos.load(std::memory_order_relaxed);
        for (int 🔢 = 0; 🔢 < 🪣.getNumSamples(); ++🔢) {
            📺_buffer[📍] = 🌊_ch0[🔢];
            📍 = (📍 + 1) % 📺_ScopeSize;
        }
        📺_writePos.store(📍, std::memory_order_release);
    }
}

void 🎛️::📺_copyScope(std::array<float, 📺_ScopeSize>& 📤) const {
    const size_t 📍 = 📺_writePos.load(std::memory_order_acquire);
    for (size_t 🔢 = 0; 🔢 < 📺_ScopeSize; ++🔢) {
        📤[🔢] = 📺_buffer[(📍 + 🔢) % 📺_ScopeSize];
    }
}

juce::AudioProcessorEditor* 🎛️::createEditor() {
    return new 🎨(*this);
}

void 🎛️::getStateInformation(juce::MemoryBlock& 💾) {
    auto 🌲 = 🌳_apvts.copyState();
    std::unique_ptr<juce::XmlElement> 📄(🌲.createXml());
    copyXmlToBinary(*📄, 💾);
}

void 🎛️::setStateInformation(const void* 📥, int 📏) {
    std::unique_ptr<juce::XmlElement> 📄(getXmlFromBinary(📥, 📏));
    if (📄 != nullptr && 📄->hasTagName(🌳_apvts.state.getType())) {
        🌳_apvts.replaceState(juce::ValueTree::fromXml(*📄));
    }
}

} // namespace 🎵

// JUCE plugin factory entry point
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() {
    return new 🎵::🎛️();
}
