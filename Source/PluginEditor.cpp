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
                         550, juce::roundToInt (550.0 / ratio));
    getConstrainer()->setFixedAspectRatio (ratio);
    setSize (450, 450/ratio);
    
    widthBand1Slider.setSliderStyle(juce::Slider::LinearVertical);
    widthBand1Slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    addAndMakeVisible(widthBand1Slider);
    widthBand1Slider.setLookAndFeel(&lafSlider);
    widthBand1Slider.setDoubleClickReturnValue(true, 0.0f);
    widthSliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "width1", widthBand1Slider);
    
    crossOverSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    crossOverSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 50, 20);
    crossOverSlider.setLookAndFeel(&lafCrossOv);
    addAndMakeVisible(crossOverSlider);
    
   // crossOverSlider.setLookAndFeel(&lafSlider);
    crossOverSlider.setDoubleClickReturnValue(true, 0.0f);
    crossoverSliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "crossover", crossOverSlider);
    
    widthBand2Slider.setSliderStyle(juce::Slider::LinearVertical);
    widthBand2Slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    addAndMakeVisible(widthBand2Slider);
    widthBand2Slider.setLookAndFeel(&lafSlider);
    widthBand2Slider.setDoubleClickReturnValue(true, 0.0f);
    width2SliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "width2", widthBand2Slider);
   

    gainSliderLeft.setSliderStyle(juce::Slider::LinearVertical);
    gainSliderLeft.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    addAndMakeVisible(gainSliderLeft);
    gainSliderLeft.setLookAndFeel(&lafGain);
    gainSliderLeft.setDoubleClickReturnValue(true, 0.0f);
    gainSliderLeftAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "gainLeft", gainSliderLeft);
    
    gainSliderRight.setSliderStyle(juce::Slider::LinearVertical);
    gainSliderRight.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    addAndMakeVisible(gainSliderRight);
    gainSliderRight.setLookAndFeel(&lafGain);
    gainSliderRight.setDoubleClickReturnValue(true, 0.0f);
    gainSliderRightAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "gainRight", gainSliderRight);
    
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
    auto widthMargin = getWidth() * 0.1;
    auto heightMargin = getHeight() * 0.1;
  
    auto topRow = (getHeight() - heightMargin * 2) * 0.4;
    //Slider width - 8 objects
    auto sliderWidth = (getWidth() - heightMargin * 2) * 0.125;
    auto sliderHeight = getHeight() - topRow - heightMargin;
    
    crossOverSlider.setBounds(widthMargin, heightMargin, sliderWidth * 4, sliderHeight * 0.2 );
    
    widthBand1Slider.setBounds(widthMargin, topRow, sliderWidth, sliderHeight );
    widthBand2Slider.setBounds(sliderWidth * 3, topRow, sliderWidth, sliderHeight);
    
    gainSliderLeft.setBounds(sliderWidth * 6, heightMargin, sliderWidth, sliderHeight + topRow - heightMargin);
    // position 7 = level meters
    gainSliderRight.setBounds(sliderWidth * 8, heightMargin, sliderWidth, sliderHeight + topRow - heightMargin);
}
