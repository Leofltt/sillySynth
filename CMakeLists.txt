cmake_minimum_required(VERSION 3.25)

project(SillySynth VERSION 0.1.0 LANGUAGES C CXX OBJC OBJCXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

# Fetch JUCE with SYSTEM so its headers are passed via -isystem,
# keeping emocxx's shadow header scanner focused solely on our emoji sources.
FetchContent_Declare(
    JUCE
    GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
    GIT_TAG        8.0.4
    GIT_SHALLOW    TRUE
    SYSTEM
)
FetchContent_MakeAvailable(JUCE)

juce_add_plugin(SillySynth
    COMPANY_NAME "leofltt"
    PLUGIN_NAME "SillySynth"
    PRODUCT_NAME "SillySynth"
    IS_SYNTH TRUE
    NEEDS_MIDI_INPUT TRUE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS TRUE
    COPY_PLUGIN_AFTER_BUILD FALSE
    PLUGIN_MANUFACTURER_CODE "FLTT"
    PLUGIN_CODE "SLLY"
    FORMATS Standalone VST3 AU
)

target_sources(SillySynth PRIVATE
    "src/🔊.cpp"
    "src/🎛️.cpp"
    "src/🎨.cpp"
)

target_include_directories(SillySynth PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

# Workaround for emocxx moving juce_audio_processors_ara.cpp into .emocxx_cache/src
# where relative #include "utilities/ARA/juce_ARADebug.h" needs its parent directory:
target_include_directories(SillySynth SYSTEM PRIVATE
    "${juce_SOURCE_DIR}/modules/juce_audio_processors"
)

target_compile_definitions(SillySynth PUBLIC
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0
)

target_link_libraries(SillySynth PRIVATE
    juce::juce_audio_utils
    juce::juce_audio_processors
    juce::juce_dsp
    juce::juce_gui_extra
    PUBLIC
    juce::juce_recommended_config_flags
    juce::juce_recommended_lto_flags
    juce::juce_recommended_warning_flags
)

if (NOT DEFINED SIGN_ID)
    if (DEFINED ENV{SIGN_ID})
        set(SIGN_ID "$ENV{SIGN_ID}")
    else()
        set(SIGN_ID "-")
    endif()
endif()

if (APPLE)
    get_target_property(active_targets SillySynth JUCE_ACTIVE_PLUGIN_TARGETS)
    if (active_targets)
        foreach(sub_target IN LISTS active_targets)
            set(COPY_DEST "")
            if (sub_target MATCHES ".*_AU$")
                set(COPY_DEST "$ENV{HOME}/Library/Audio/Plug-Ins/Components/SillySynth.component")
            elseif (sub_target MATCHES ".*_VST3$")
                set(COPY_DEST "$ENV{HOME}/Library/Audio/Plug-Ins/VST3/SillySynth.vst3")
            endif()

            if (NOT "${COPY_DEST}" STREQUAL "")
                add_custom_command(
                    TARGET ${sub_target} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E rm -rf "${COPY_DEST}"
                    COMMAND ${CMAKE_COMMAND} -E copy_directory "$<TARGET_FILE_DIR:${sub_target}>/../.." "${COPY_DEST}"
                    COMMAND codesign --force --deep --sign "${SIGN_ID}" "${COPY_DEST}"
                    COMMENT "Installing and codesigning ${sub_target} in local plugin folder (${COPY_DEST})"
                    VERBATIM
                )
            else()
                add_custom_command(
                    TARGET ${sub_target} POST_BUILD
                    COMMAND codesign --force --deep --sign "${SIGN_ID}" "$<TARGET_FILE_DIR:${sub_target}>/../.."
                    COMMENT "Codesigning ${sub_target}"
                    VERBATIM
                )
            endif()
        endforeach()
    endif()
endif()
