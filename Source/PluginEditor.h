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
#include "../Assets/VLevelMeter.h"

//==============================================================================
/**
*/
class StereoimagerAudioProcessorEditor  : public juce::AudioProcessorEditor, public juce::Button::Listener, public juce::Timer
{
public:
    StereoimagerAudioProcessorEditor (StereoimagerAudioProcessor&);
    ~StereoimagerAudioProcessorEditor() override;
    void timerCallback() override;
    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
    void unlink();
    void link();
    void buttonClicked (juce::Button* button) override;
    
 
private:
    StereoimagerAudioProcessor& audioProcessor;
    
    Gui::VMeter vMeterLeft;
    Gui::VMeter vMeterRight;
    
    juce::Slider widthBand1Slider;
    juce::Label band1Label;
    juce::Label band2Label;
    juce::Label crossLabel;
    juce::Label stereo2Label;
    juce::Label stereoLabel;
    juce::Slider widthBand2Slider;
    juce::Slider crossOverSlider;
    juce::Slider gainSliderLeft;
    juce::Slider gainSliderRight;
    juce::Slider gainSliderLeftLinked;
    juce::Slider gainSliderRightLinked;
    juce::Label title;

  //  juce::ScopedPointer<juce::Drawable> knob_image;
    
    enum class LinkState
    {
            Link,
            Unlink,
    };
    
    LinkState linkState {LinkState::Link};
   // juce::ImageButton linkButtonGain{};
    juce::DrawableButton linkButtonGain{ "linkButtonGain", juce::DrawableButton::ButtonStyle::ImageOnButtonBackground };
    

   
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> widthSliderAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> width2SliderAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> crossoverSliderAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> gainSliderLeftAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> gainSliderRightAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> gainSliderLinkedLAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> gainSliderLinkedRAttach;

    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> linkButtonGainAttach;
   
    
   // std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> gainSumAttachLR;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> gainSumAttach;
    
    
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
   

    // set different attributes - call overloaded constructors....
    CustomSlider lafSlider;
    CustomCrossover lafCrossOv;
    CustomGainSlider lafGain;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StereoimagerAudioProcessorEditor)
};
