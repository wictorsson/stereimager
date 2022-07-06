/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class StereoimagerAudioProcessor  : public juce::AudioProcessor, juce::AudioProcessorValueTreeState::Listener
{
public:
    //==============================================================================
    StereoimagerAudioProcessor();
    ~StereoimagerAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    juce::AudioProcessorValueTreeState apvts;
    
private:
 
    juce::LinearSmoothedValue<float> width1Value{1};
    juce::LinearSmoothedValue<float> width2Value{1};
  
    float crossoverValue;
    float lp;
    juce::LinearSmoothedValue<float> gainLeftValue;
    juce::LinearSmoothedValue<float> gainRightValue;

    juce::dsp::LinkwitzRileyFilter<float> crossOverFilterModule;
    juce::dsp::LinkwitzRileyFilter<float> crossOverFilterModule2;
    
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    
    void parameterChanged(const juce::String& parameterID, float newValue) override;
  
  
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StereoimagerAudioProcessor)
};
