#pragma once

#include <algorithm>
#include <cmath>
#include <concepts>
#include <numbers>
#include <random>

namespace 🎵 {

  // ============================================================================
  // 📏 User-Defined Emoji Literals (UDLs)
  // ============================================================================
  // Frequency in Hz: e.g. 440.0_🎵
  constexpr double operator""_🎵(long double 🎼) {
    return static_cast<double>(🎼);
  }

  // Time in milliseconds -> seconds: e.g. 15.0_⏱️
  constexpr double operator""_⏱️(long double ⌛) {
    return static_cast<double>(⌛) * 0.001;
  }

  // ============================================================================
  // 🧩 C++20 Emoji Concept for DSP Blocks
  // ============================================================================
  template <typename 🧩>
  concept 🔌 = requires(🧩 📠, double ⏱️) {
    { 📠.🎯(⏱️) } -> std::same_as<void>;
    { 📠.🧹() } -> std::same_as<void>;
  };

  // ============================================================================
  // 🌊 Waveform Selection Enum
  // ============================================================================
  enum class 🌊 : int{
    〰️ = 0, // Sine wave
    📐 = 1, // Triangle wave
    🪚 = 2, // Sawtooth wave
    🟦 = 3, // Square wave
    🎲 = 4 // White noise
  };

  // ============================================================================
  // 🩹 PolyBLEP & PolyBLAMP Anti-Aliasing Magic ✨🪄
  // ============================================================================
  // 🩹_blep: 2-point Polynomial Band-Limited Step (PolyBLEP) residual
  // ✂️ Smoothes out sharp vertical jump discontinuities in 🪚 Sawtooth and 🟦 Square!
  constexpr double 🩹_blep(double 🧭_t, double 🏃_dt) noexcept {
    if (🏃_dt <= 0.0 || 🏃_dt >= 0.5)
      return 0.0;
    if (🧭_t < 🏃_dt) {
      const double 🤏 = 🧭_t / 🏃_dt - 1.0;
      return -🤏 * 🤏;
    }
    if (🧭_t > 1.0 - 🏃_dt) {
      const double 🤏 = (🧭_t - 1.0) / 🏃_dt + 1.0;
      return 🤏 * 🤏;
    }
    return 0.0;
  }

  // 📐_blamp: Polynomial Band-Limited Ramp (PolyBLAMP) residual
  // 🧈 Rounds off sharp derivative corners in 📐 Triangle waves!
  constexpr double 📐_blamp(double 🧭_t, double 🏃_dt) noexcept {
    if (🏃_dt <= 0.0 || 🏃_dt >= 0.5)
      return 0.0;
    if (🧭_t < 🏃_dt) {
      const double 🤏 = 🧭_t / 🏃_dt - 1.0;
      return -(1.0 / 3.0) * 🤏 * 🤏 * 🤏;
    }
    if (🧭_t > 1.0 - 🏃_dt) {
      const double 🤏 = (🧭_t - 1.0) / 🏃_dt + 1.0;
      return (1.0 / 3.0) * 🤏 * 🤏 * 🤏;
    }
    return 0.0;
  }

  // ============================================================================
  // 🌀 Anti-Aliased Multi-Waveform Emoji Oscillator (✨ PolyBLEP / PolyBLAMP ✨)
  // ============================================================================
  class 🌀 {
  public:
    void 🎯(double ⏱️) {
      🍽️ = (⏱️ > 0.0) ? ⏱️ : 44100.0;
      🧹();
    }

    void 🧹() {
        🧭 = 0.0; }

    void 🎵(double 🎼) {
        🏃 = 🎼 / 🍽️; }

    float 🔊(🌊 🎭) {
      float 💧 = 0.0f;
      const double 🔄 = 🧭;

      // 🎶 Anti-aliased high-fidelity band-limited synthesis 🎷✨
      switch (🎭) {
      case 🌊::〰️: // 〰️ Pure Sine 🧘✨)
        💧 = static_cast<float>(std::sin(🔄 * 2.0 * std::numbers::pi));
        break;

      case 🌊::📐: { // 📐 Anti-aliased Triangle wave via PolyBLAMP 🧈📐
        double 🧭_t1 = 🔄 + 0.25;
        if (🧭_t1 >= 1.0)
          🧭_t1 -= 1.0;
        double 🧭_t2 = 🔄 + 0.75;
        if (🧭_t2 >= 1.0)
          🧭_t2 -= 1.0;

        double 💧_tri = 🔄 * 4.0;
        if (💧_tri >= 3.0) {
          💧_tri -= 4.0;
        } else if (💧_tri > 1.0) {
          💧_tri = 2.0 - 💧_tri;
        }

        // 🧈 Round off the two slope discontinuities using PolyBLAMP residuals!
        💧_tri += 4.0 * 🏃 * (📐_blamp(🧭_t1, 🏃) - 📐_blamp(🧭_t2, 🏃));
        💧 = static_cast<float>(💧_tri);
        break;
      }

      case 🌊::🪚: { // 🪚 Anti-aliased Sawtooth wave via PolyBLEP 🩹🪚
        double 🧭_saw = 🔄 + 0.5;
        if (🧭_saw >= 1.0)
          🧭_saw -= 1.0;

        double 💧_saw = 2.0 * 🧭_saw - 1.0;
        // 🩹 Smooth the vertical falling step discontinuity at the wrap!
        💧_saw -= 🩹_blep(🧭_saw, 🏃);
        💧 = static_cast<float>(💧_saw);
        break;
      }

      case 🌊::🟦: { // 🟦 Anti-aliased Square wave via PolyBLEP 🩹🟦
        double 🧭_sqr = 🔄 + 0.5;
        if (🧭_sqr >= 1.0)
          🧭_sqr -= 1.0;

        double 💧_sqr = (🔄 < 0.5) ? 1.0 : -1.0;
        // 🩹 Smooth both rising (t=0) and falling (t=0.5) step edges!
        💧_sqr += 🩹_blep(🔄, 🏃) - 🩹_blep(🧭_sqr, 🏃);
        💧 = static_cast<float>(💧_sqr);
        break;
      }

      case 🌊::🎲: // 🎲 White noise 🎰
        💧 = 🎰(🌱);
        break;
      }

      🧭 += 🏃;
      if (🧭 >= 1.0)
        🧭 -= std::floor(🧭);

      return 💧;
    }

  private:
    double 🍽️ = 44100.0; // Sample rate
    double 🧭 = 0.0;    // Normalized phase [0, 1)
    double 🏃 = 0.0;    // Phase increment
    std::mt19937 🌱{1337};
    std::uniform_real_distribution<float> 🎰{-1.0f, 1.0f};
  };

  // ============================================================================
  // ✉️ ADSR Envelope Parameters
  // ============================================================================
  struct ✉️ {
    float ⚡ = 0.01f; // Attack (s)
    float 💧 = 0.15f; // Decay (s)
    float 🧘 = 0.75f; // Sustain [0..1]
    float 🎈 = 0.30f; // Release (s)
  };

  // ============================================================================
  // 🕳️ Cytomic TPT State-Variable Filter (Lowpass / Bandpass / Highpass)
  // ============================================================================
  enum class 🚦 {
    🟢, // Low-pass
    🟡, // Band-pass
    🔴 // High-pass
  };

  class 🕳️ {
  public:
    void 🎯(double ⏱️) {
      🍽️ = (⏱️ > 0.0) ? ⏱️ : 44100.0;
      🧹();
      🔄();
    }

    void 🧹() {
      🧊1 = 0.0f;
      🧊2 = 0.0f;
    }

    void 🎛️(float ✂️_hz, float 🔔_res, 🚦 🛣️ = 🚦::🟢) {
      ✂️ = std::clamp(✂️_hz, 20.0f, static_cast<float>(🍽️ * 0.48));
      🔔 = std::clamp(🔔_res, 0.1f, 10.0f);
      🚦_mode = 🛣️;
      🔄();
    }

    float 🔊(float 📥) {
      const float 🏔️ = (📥 - (🏢 + 📐_g) * 🧊1 - 🧊2) * 🧮;
      const float 🎗️ = 📐_g * 🏔️ + 🧊1;
      🧊1 = 📐_g * 🏔️ + 🎗️;
      const float 🌊_lp = 📐_g * 🎗️ + 🧊2;
      🧊2 = 📐_g * 🎗️ + 🌊_lp;

      switch (🚦_mode) {
      case 🚦::🟢:
        return 🌊_lp;
      case 🚦::🟡:
        return 🎗️;
      case 🚦::🔴:
        return 🏔️;
      }
      return 🌊_lp;
    }

  private:
    void 🔄() {
      📐_g = std::tan(static_cast<float>(std::numbers::pi) * ✂️ /
                      static_cast<float>(🍽️));
      🏢 = 1.0f / 🔔;
      🧮 = 1.0f / (1.0f + 📐_g * (📐_g + 🏢));
    }

    double 🍽️ = 44100.0;
    float ✂️ = 2000.0f;
    float 🔔 = 0.707f;
    🚦 🚦_mode = 🚦::🟢;

    float 📐_g = 0.0f;
    float 🏢 = 1.0f;
    float 🧮 = 1.0f;
    float 🧊1 = 0.0f;
    float 🧊2 = 0.0f;
  };

  // Verify our DSP components satisfy the C++20 emoji concept 🔌
  static_assert(🔌<🌀>);
  static_assert(🔌<🕳️>);

} // namespace 🎵
