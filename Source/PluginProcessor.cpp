/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
StereoimagerAudioProcessor::StereoimagerAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), apvts(*this, nullptr, "PARAMETERS", createParameterLayout())
#endif
{
    apvts.addParameterListener("width", this);
}

StereoimagerAudioProcessor::~StereoimagerAudioProcessor()
{
    apvts.removeParameterListener("width", this);
}



juce::AudioProcessorValueTreeState::ParameterLayout StereoimagerAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
    auto width = std::make_unique<juce::AudioParameterFloat> ("width","Width", juce::NormalisableRange<float>(0,2),1);
    params.push_back(std::move(width));
    
    return {params.begin(), params.end()};
}

void StereoimagerAudioProcessor::parameterChanged(const juce::String& parameterID, float newValue)
{
    if (parameterID == "width")
    {
        //widthValue = newValue;
        widthValue.setTargetValue(newValue);
        DBG(newValue);
    }
}


//==============================================================================
const juce::String StereoimagerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool StereoimagerAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool StereoimagerAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool StereoimagerAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double StereoimagerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int StereoimagerAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int StereoimagerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void StereoimagerAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String StereoimagerAudioProcessor::getProgramName (int index)
{
    return {};
}

void StereoimagerAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void StereoimagerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    widthValue.reset(sampleRate, 0.03);
}

void StereoimagerAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool StereoimagerAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void StereoimagerAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelDataL = buffer.getWritePointer (0);
        auto* channelDataR = buffer.getWritePointer (1);
        
        for(int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
        //Create mid side channels
            const auto side = 0.5 * (channelDataL[sample] - channelDataR[sample]);
            const auto mid = 0.5 * (channelDataL[sample] + channelDataR[sample]);

            const auto sideNew = widthValue.getNextValue() * side;
            const auto midNew = (2 - widthValue.getNextValue()) * mid;

            const auto newLeft = midNew + sideNew;
            const auto newRight = midNew - sideNew;
            
            if(widthValue.getNextValue() >= 1.0)
            {
                const auto volumeScale = juce::jmap(widthValue.getNextValue(),1.0f, 2.0f, 0.0f, 6.0f);
                channelDataL[sample] = newLeft * juce::Decibels::decibelsToGain(volumeScale);
                channelDataR[sample] = newRight * juce::Decibels::decibelsToGain(volumeScale);
            }
            else
            {
                const auto volumeScale = juce::jmap(widthValue.getNextValue(),1.0f, 0.0f, 0.0f, -6.0f);
                channelDataL[sample] = newLeft * juce::Decibels::decibelsToGain(volumeScale);
                channelDataR[sample] = newRight * juce::Decibels::decibelsToGain(volumeScale);
            }
        }
        
        
        // ..do something to the data...
    }
}

//==============================================================================
bool StereoimagerAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* StereoimagerAudioProcessor::createEditor()
{
    //return new StereoimagerAudioProcessorEditor (*this);
    
    return new juce::GenericAudioProcessorEditor(*this);
}

//==============================================================================
void StereoimagerAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void StereoimagerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new StereoimagerAudioProcessor();
}
