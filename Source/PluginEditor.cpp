/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
StereoimagerAudioProcessorEditor::StereoimagerAudioProcessorEditor (StereoimagerAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setResizable (true, true);
    const float ratio = 4.0/ 3.0;
    setResizeLimits (350,  juce::roundToInt (350.0 / ratio),
                         550, juce::roundToInt (500.0 / ratio));
    getConstrainer()->setFixedAspectRatio (ratio);
    setSize (450, 450/ratio);
    
    widthSlider.setSliderStyle(juce::Slider::LinearVertical);
    
    widthSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    addAndMakeVisible(widthSlider);
    
    widthSlider.setLookAndFeel(&lafSlider);
    widthSlider.setDoubleClickReturnValue(true, 0.0f);

    widthSliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "width", widthSlider);
    
    
    gainSlider.setSliderStyle(juce::Slider::LinearVertical);
    gainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    addAndMakeVisible(gainSlider);
    gainSlider.setDoubleClickReturnValue(true, 0.0f);

    gainSliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "gain", gainSlider);
    
}

StereoimagerAudioProcessorEditor::~StereoimagerAudioProcessorEditor()
{
}

//==============================================================================
void StereoimagerAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (juce::Colour::fromFloatRGBA (0.08f, 0.08f, 0.08f, 1.0f));


}

void StereoimagerAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    auto leftMargin = getWidth() * 0.1;
    auto topMargin = getHeight() * 0.1;
    //auto bottomMargin = getHeight() * 0.25;
    auto sliderSize = getWidth() * 0.4;
    
    widthSlider.setBounds(leftMargin, topMargin, sliderSize, sliderSize );
    gainSlider.setBounds(widthSlider.getX() + widthSlider.getWidth(), topMargin, sliderSize, sliderSize);
}
