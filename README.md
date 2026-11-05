# SillySynth 🤪🎹

**SillySynth** is a polyphonic subtractive synthesizer built with [JUCE](https://juce.com/) where the entire C++ codebase filenames, `#include`s, namespaces, classes, waveforms, filter states, C++20 concepts, and user-defined literals is written using Unicode emojis.

This repository serves as a real-world showcase and stress-test demo for **[emocxx](https://github.com/Leofltt/emocxx)**, a transparent Unicode Emoji Compiler Proxy for C++20/C++23.

## 🛠️ Prerequisites

1. **Install [`emocxx`](https://github.com/Leofltt/emocxx)**:

   ```bash
   cargo install --git https://github.com/Leofltt/emocxx
   ```

   *(Or build it locally from a sibling clone at `../emocxx` via `cargo build --release`.)*
2. **CMake** (3.25+) and **Ninja** (recommended)
3. **Clang++** or **G++** with C++20 support

---

## 🚀 Building (Standalone, VST3, AU)

JUCE 8 is fetched automatically by CMake via `FetchContent`.

### Option 1: Using `emocxx` in `$PATH` (Recommended)

Configure the project with `emocxx` as the C++ compiler proxy:

```bash
cmake -B build -G Ninja \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_CXX_COMPILER=emocxx \
  -DREAL_CXX=clang++
```

Then build all plugin formats at once, or build a specific target:

```bash
# Build all targets (Standalone + VST3 + AU on macOS)
cmake --build build

# Or build individual targets:
cmake --build build --target SillySynth_Standalone
cmake --build build --target SillySynth_VST3
cmake --build build --target SillySynth_AU
```

### Option 2: Using the CMake Preset / Toolchain File

If you have [`emocxx`](https://github.com/Leofltt/emocxx) cloned next to this repo (`/Users/leofltt/repos/emocxx`), you can use the bundled preset:

```bash
cmake --preset emocxx
cmake --build --preset emocxx
```

---

## 📦 Build Output & Automatic Plugin Installation

Once compiled, the generated artifacts are located in `build/SillySynth_artefacts/`:

- **Standalone App**: `build/SillySynth_artefacts/Release/Standalone/SillySynth.app`
- **VST3 Plugin**: `build/SillySynth_artefacts/Release/VST3/SillySynth.vst3` *(automatically copied & codesigned to `~/Library/Audio/Plug-Ins/VST3/SillySynth.vst3` on macOS)*
- **AU Plugin (macOS)**: `build/SillySynth_artefacts/Release/AU/SillySynth.component` *(automatically copied & codesigned to `~/Library/Audio/Plug-Ins/Components/SillySynth.component`)*
