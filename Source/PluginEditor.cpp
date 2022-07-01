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
    setSize (400, 300);
    
    widthSlider.setSliderStyle(juce::Slider::LinearVertical);
    widthSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    addAndMakeVisible(widthSlider);
    
    widthSliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "width", widthSlider);
    
    gainSlider.setSliderStyle(juce::Slider::LinearVertical);
    gainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    addAndMakeVisible(gainSlider);
    gainSliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "gain", gainSlider);
    
}

StereoimagerAudioProcessorEditor::~StereoimagerAudioProcessorEditor()
{
}

//==============================================================================
void StereoimagerAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));


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
