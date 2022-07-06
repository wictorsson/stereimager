/*
  ==============================================================================

    LAFcrossover.h
    Created: 6 Jul 2022 5:35:00pm
    Author:  Fredrik Wictorsson

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>


class CustomCrossover : public juce::LookAndFeel_V4
{
public:
    CustomCrossover();
   
    
    void drawLinearSlider (juce::Graphics&, int x, int y, int width, int height,
                           float sliderPos, float minSliderPos, float maxSliderPos,
                           const juce::Slider::SliderStyle, juce::Slider&) override;
    
private:
    
    
};

