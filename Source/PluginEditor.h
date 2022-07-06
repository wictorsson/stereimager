/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "../Assets/LAFSlider.h"
#include "../Assets/LAFgainSlider.h"
#include "../Assets/LAFcrossover.h"

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
    
    juce::Slider widthBand1Slider;
    juce::Slider widthBand2Slider;
    juce::Slider crossOverSlider;
    juce::Slider gainSliderLeft;
    juce::Slider gainSliderRight;
   
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> widthSliderAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> width2SliderAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> crossoverSliderAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> gainSliderLeftAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> gainSliderRightAttach;
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    StereoimagerAudioProcessor& audioProcessor;

    // set different attributes - call overloaded constructors....
    CustomSlider lafSlider;
    CustomCrossover lafCrossOv;
    CustomGainSlider lafGain;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StereoimagerAudioProcessorEditor)
};
