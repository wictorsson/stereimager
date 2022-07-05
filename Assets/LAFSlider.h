/*
  ==============================================================================

    LAFSlider.h
    Created: 4 Jul 2022 6:28:42pm
    Author:  Fredrik Wictorsson

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>


class CustomSlider : public juce::LookAndFeel_V4
{
public:
    CustomSlider();
   
    
    void drawLinearSlider (juce::Graphics&, int x, int y, int width, int height,
                           float sliderPos, float minSliderPos, float maxSliderPos,
                           const juce::Slider::SliderStyle, juce::Slider&) override;
    
private:
    std::string type;
    
};
