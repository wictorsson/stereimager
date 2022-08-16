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
    setResizeLimits (470,  juce::roundToInt (470.0 / ratio),
                         650, juce::roundToInt (650.0 / ratio));
    getConstrainer()->setFixedAspectRatio (ratio);
    setSize (550, 550/ratio);
    
    widthBand1Slider.setSliderStyle(juce::Slider::LinearVertical);
    widthBand1Slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    addAndMakeVisible(widthBand1Slider);
    widthBand1Slider.setLookAndFeel(&lafSlider);
    widthBand1Slider.setDoubleClickReturnValue(true, 0.0f);
    widthSliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "width1", widthBand1Slider);
    
    addAndMakeVisible(band1Label);
    band1Label.setText("BAND 1", juce::dontSendNotification);
    band1Label.attachToComponent(& widthBand1Slider, true);
    band1Label.setJustificationType(juce::Justification::bottomRight);
    
    addAndMakeVisible(stereoLabel);
    stereoLabel.setText("OO", juce::dontSendNotification);
    stereoLabel.attachToComponent(& widthBand1Slider, false);
    stereoLabel.setJustificationType(juce::Justification::centred);
    
    crossOverSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    crossOverSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 50, 20);
    crossOverSlider.setLookAndFeel(&lafCrossOv);
    
    addAndMakeVisible(crossOverSlider);
    
   // crossOverSlider.setLookAndFeel(&lafSlider);
    crossOverSlider.setDoubleClickReturnValue(true, 0.0f);
    crossoverSliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "crossover", crossOverSlider);
    
    
    addAndMakeVisible(crossLabel);
    crossLabel.setText("X OVER", juce::dontSendNotification);
    crossLabel.attachToComponent(& crossOverSlider, true);
    crossLabel.setJustificationType(juce::Justification::centredRight);
    
    widthBand2Slider.setSliderStyle(juce::Slider::LinearVertical);
    widthBand2Slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    addAndMakeVisible(widthBand2Slider);
    widthBand2Slider.setLookAndFeel(&lafSlider);
    widthBand2Slider.setDoubleClickReturnValue(true, 0.0f);
    width2SliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "width2", widthBand2Slider);
    
    addAndMakeVisible(stereo2Label);
    stereo2Label.setText("OO", juce::dontSendNotification);
    stereo2Label.attachToComponent(& widthBand2Slider, false);
    stereo2Label.setJustificationType(juce::Justification::centred);
    
    addAndMakeVisible(band2Label);
    band2Label.setText("BAND 2", juce::dontSendNotification);
    band2Label.attachToComponent(& widthBand2Slider, true);
    band2Label.setJustificationType(juce::Justification::bottomRight);
   
    gainSliderLeft.setSliderStyle(juce::Slider::LinearVertical);
    gainSliderLeft.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    addAndMakeVisible(gainSliderLeft);
    gainSliderLeft.setLookAndFeel(&lafGain);
    gainSliderLeft.setDoubleClickReturnValue(true, 0.0f);
    gainSliderLeftAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "gainLeft", gainSliderLeft);
    gainSliderLeft.setVisible(false);


    gainSliderRight.setSliderStyle(juce::Slider::LinearVertical);
    gainSliderRight.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    addAndMakeVisible(gainSliderRight);
    gainSliderRight.setLookAndFeel(&lafGain);
    gainSliderRight.setDoubleClickReturnValue(true, 0.0f);
    gainSliderRightAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "gainRight", gainSliderRight);
    gainSliderRight.setVisible(false);
    
    
    gainSliderLeftLinked.setSliderStyle(juce::Slider::LinearVertical);
    gainSliderLeftLinked.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    addAndMakeVisible(gainSliderLeftLinked);
    gainSliderLeftLinked.setLookAndFeel(&lafGain);
    gainSliderLeftLinked.setDoubleClickReturnValue(true, 0.0f);
    gainSliderLinkedLAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "gainLinked", gainSliderLeftLinked);

    gainSliderRightLinked.setSliderStyle(juce::Slider::LinearVertical);
    gainSliderRightLinked.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    addAndMakeVisible(gainSliderRightLinked);
    gainSliderRightLinked.setLookAndFeel(&lafGain);
    gainSliderRightLinked.setDoubleClickReturnValue(true, 0.0f);
    gainSliderLinkedRAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "gainLinked", gainSliderRightLinked);
    
    
    auto knob_image = juce::Drawable::createFromImageData (BinaryData::link_svg, BinaryData::link_svgSize);
    auto knob_imageBroken = juce::Drawable::createFromImageData (BinaryData::brokenlink_svg, BinaryData::brokenlink_svgSize);
    
    
    linkButtonGain.setImages(knob_imageBroken.get(),knob_imageBroken.get(),knob_imageBroken.get(), knob_imageBroken.get(), knob_image.get(), knob_image.get(), knob_image.get(), knob_image.get());
    
    linkButtonGain.setToggleState(true, juce::NotificationType::dontSendNotification);
   
    linkButtonGain.onClick = [this](){unlink();};
    linkButtonGain.addListener(this);
    addAndMakeVisible(linkButtonGain);

    // Gör två nya sliders och göm dom ifall linked/ byt ut
    
    addAndMakeVisible(vMeterLeft);
    addAndMakeVisible(vMeterRight);
    startTimerHz(24);
    
    title.setText ("F.W IMAGER v1.0", juce::dontSendNotification);
    title.setJustificationType(juce::Justification::left);
    title.setColour(0x1000281, juce::Colours::whitesmoke);
    title.setFont (juce::Font (10.0f));
    title.setInterceptsMouseClicks(false, false);
    addAndMakeVisible (title);
}

StereoimagerAudioProcessorEditor::~StereoimagerAudioProcessorEditor()
{
    // destroy Look and feel here!!!
}

//==============================================================================
void StereoimagerAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (juce::Colour::fromFloatRGBA (0.08f, 0.08f, 0.08f, 1.0f));
  //  g.fillAll (juce::Colour::fromFloatRGBA (1.0f, 1.0f, 1.0f, 1.0f));
  
}

void StereoimagerAudioProcessorEditor::resized()
{
    auto widthMargin = getWidth() * 0.12;
    auto heightMargin = getHeight() * 0.1;
    auto topRow = (getHeight() - heightMargin * 2) * 0.4;
    auto sliderWidth = (getWidth() - heightMargin * 2) * 0.125;
    auto sliderHeight = getHeight() - topRow - heightMargin;
    title.setBounds(widthMargin * 0.1, heightMargin * 0.05, 80, 30);
    crossOverSlider.setBounds(widthMargin * 1.2, heightMargin * 1.2, sliderWidth * 4, sliderHeight * 0.2 );
    widthBand1Slider.setBounds(widthMargin * 1.5, topRow, sliderWidth, sliderHeight );
    widthBand2Slider.setBounds(sliderWidth * 3.8, topRow, sliderWidth, sliderHeight);
    gainSliderLeft.setBounds(sliderWidth * 6.5, heightMargin, sliderWidth, sliderHeight + topRow - heightMargin);
    gainSliderRight.setBounds(sliderWidth * 8, heightMargin, sliderWidth, sliderHeight + topRow - heightMargin);
    linkButtonGain.setBounds(sliderWidth * 7.3, sliderHeight + topRow - heightMargin + linkButtonGain.getHeight()/2 , sliderWidth * 0.9, heightMargin * 0.6 );
    gainSliderLeftLinked.setBounds(sliderWidth * 6.5, heightMargin, sliderWidth, sliderHeight + topRow - heightMargin);
    gainSliderRightLinked.setBounds(sliderWidth * 8, heightMargin, sliderWidth, sliderHeight + topRow - heightMargin);
    vMeterLeft.setBounds(sliderWidth * 7.3, heightMargin, sliderWidth * 0.4, sliderHeight + topRow - heightMargin - heightMargin);
    vMeterRight.setBounds(sliderWidth * 7.8, heightMargin, sliderWidth * 0.4, sliderHeight + topRow - heightMargin - heightMargin);

  

}

void StereoimagerAudioProcessorEditor::unlink()
{
    linkState = LinkState::Unlink;
    linkButtonGain.setToggleState(false, juce::NotificationType::dontSendNotification);
    
    gainSliderLeft.setValue(gainSliderLeftLinked.getValue());
    gainSliderRight.setValue(gainSliderRightLinked.getValue());
    
    gainSliderLeft.setVisible(true);
    gainSliderRight.setVisible(true);
    
    gainSliderLeftLinked.setVisible(false);
    gainSliderRightLinked.setVisible(false);
    
    audioProcessor.linkGainButton = false;
    
}

void StereoimagerAudioProcessorEditor::link()
{
    linkState = LinkState::Link;
    linkButtonGain.setToggleState(true, juce::NotificationType::dontSendNotification);
    
    gainSliderLeftLinked.setValue(gainSliderLeft.getValue());
    gainSliderRightLinked.setValue(gainSliderRight.getValue());

    
    gainSliderLeft.setVisible(false);
    gainSliderRight.setVisible(false);
    
    gainSliderLeftLinked.setVisible(true);
    gainSliderRightLinked.setVisible(true);
    
    audioProcessor.linkGainButton = true;
    
}

void StereoimagerAudioProcessorEditor::buttonClicked(juce::Button *button)
{
    
    if (button == &linkButtonGain)
    {
        if(linkState == LinkState::Unlink)
        {
            linkButtonGain.onClick = [this](){link();};
        }
        else if(linkState == LinkState::Link)
        {
            linkButtonGain.onClick = [this](){unlink();};
        }
    }
}

void StereoimagerAudioProcessorEditor::timerCallback()
{
    vMeterLeft.setLevel(audioProcessor.getRmsValue(0));
    vMeterRight.setLevel(audioProcessor.getRmsValue(1));
    vMeterLeft.repaint();
    vMeterRight.repaint();
}
