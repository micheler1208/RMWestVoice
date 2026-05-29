cmake_minimum_required(VERSION 3.22)

project(RMWestVoice VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()

set(JUCE_ROOT "C:/D/MIKE-VST/WORKSPACE JUCE/JUCE8/JUCE" CACHE PATH "Path to JUCE 8")
add_subdirectory("${JUCE_ROOT}" "${CMAKE_CURRENT_BINARY_DIR}/JUCE")

juce_add_plugin(RMWestVoice
    VERSION ${PROJECT_VERSION}
    COMPANY_NAME "RM Audio"
    BUNDLE_ID "com.rmaudio.RMWestVoice"
    PLUGIN_MANUFACTURER_CODE Rmau
    PLUGIN_CODE Rwvo
    FORMATS VST3 Standalone
    IS_SYNTH TRUE
    NEEDS_MIDI_INPUT TRUE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
    COPY_PLUGIN_AFTER_BUILD FALSE
    VST3_CATEGORIES Instrument Synth
    PRODUCT_NAME "RM West Voice"
    PLUGIN_NAME "RM West Voice")

juce_generate_juce_header(RMWestVoice)

juce_add_binary_data(RMWestVoiceBinaryData
    SOURCES
        Source/img/background.jpg
        Source/font/caviar.ttf
        Source/font/gameboy.ttf
        Source/font/graffiti.ttf
        Source/font/heaters.ttf
        Source/font/timegoing.ttf
        Source/font/wholecar.ttf)

target_sources(RMWestVoice
    PRIVATE
        Source/PluginParameters.h
        Source/PluginProcessor.cpp
        Source/PluginEditor.cpp
        Source/Engine/MonoLeadEngine.cpp
        Source/Engine/MonoLeadEngine.h
        Source/Engine/CharacterState.cpp
        Source/Engine/CharacterState.h
        Source/Engine/FilterModulationState.cpp
        Source/Engine/FilterModulationState.h
        Source/Engine/PitchModulationState.cpp
        Source/Engine/PitchModulationState.h
        Source/Engine/GlideState.cpp
        Source/Engine/GlideState.h
        Source/Engine/MonoNoteStack.cpp
        Source/Engine/MonoNoteStack.h
        Source/Data/AdsrData.cpp
        Source/Data/FilterData.cpp
        Source/Data/LeadFilterData.cpp
        Source/Data/LeadFilterData.h
        Source/Data/OscData.cpp
        Source/UI/AdsrComponent.cpp
        Source/UI/FilterComponent.cpp
        Source/UI/OscComponent.cpp)

target_compile_definitions(RMWestVoice
    PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_ASIO=1
        JUCE_VST3_CAN_REPLACE_VST2=0
        JUCE_STRICT_REFCOUNTEDPOINTER=1)

target_link_libraries(RMWestVoice
    PRIVATE
        RMWestVoiceBinaryData
        juce::juce_audio_utils
        juce::juce_dsp
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)

add_executable(RMWestVoiceTests
    Tests/MonoNoteStackTests.cpp
    Source/Engine/GlideState.cpp
    Source/Engine/GlideState.h
    Source/Engine/CharacterState.cpp
    Source/Engine/CharacterState.h
    Source/Engine/FilterModulationState.cpp
    Source/Engine/FilterModulationState.h
    Source/Engine/MonoNoteStack.cpp
    Source/Engine/MonoNoteStack.h
    Source/Engine/PitchModulationState.cpp
    Source/Engine/PitchModulationState.h)

target_include_directories(RMWestVoiceTests
    PRIVATE
        Source)

target_compile_features(RMWestVoiceTests
    PRIVATE
        cxx_std_17)

add_test(NAME RMWestVoiceTests COMMAND RMWestVoiceTests)
