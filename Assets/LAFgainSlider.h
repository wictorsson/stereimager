/*
  ==============================================================================

    LAFSlider.h
    Created: 4 Jul 2022 6:28:42pm
    Author:  Fredrik Wictorsson

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>


class CustomGainSlider : public juce::LookAndFeel_V4
{
public:
    CustomGainSlider();
   
    
    void drawLinearSlider (juce::Graphics&, int x, int y, int width, int height,
                           float sliderPos, float minSliderPos, float maxSliderPos,
                           const juce::Slider::SliderStyle, juce::Slider&) override;
    
    juce::Label* createSliderTextBox (juce::Slider& slider) override;

    
private:
    std::string type;
    
};
