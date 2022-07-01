/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class StereoimagerAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    StereoimagerAudioProcessorEditor (StereoimagerAudioProcessor&);
    ~StereoimagerAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    
    juce::Slider widthSlider;
    juce::Slider gainSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> widthSliderAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> gainSliderAttach;
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    StereoimagerAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StereoimagerAudioProcessorEditor)
};
