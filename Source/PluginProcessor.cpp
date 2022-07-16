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
    apvts.addParameterListener("width1", this);
    apvts.addParameterListener("width2", this);
    apvts.addParameterListener("crossover", this);
    apvts.addParameterListener("gainLeft", this);
    apvts.addParameterListener("gainRight", this);
    apvts.addParameterListener("linkGain", this);
    apvts.addParameterListener("gainLinked", this);
  
}

StereoimagerAudioProcessor::~StereoimagerAudioProcessor()
{
    apvts.removeParameterListener("width1", this);
    apvts.removeParameterListener("width2", this);
    apvts.removeParameterListener("crossover", this);
    apvts.removeParameterListener("gainLeft", this);
    apvts.removeParameterListener("gainRight", this);
    apvts.removeParameterListener("linkGain", this);
    
    apvts.removeParameterListener("gainLinked", this);


}



juce::AudioProcessorValueTreeState::ParameterLayout StereoimagerAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
    
    auto width = std::make_unique<juce::AudioParameterFloat> ("width1","Width1", juce::NormalisableRange<float>(-100,100, 1.0),0);
    params.push_back(std::move(width));
    
    auto width2 = std::make_unique<juce::AudioParameterFloat> ("width2","Width2", juce::NormalisableRange<float>(-100,100, 1.0),0);
    params.push_back(std::move(width2));
    
    auto crossOv = std::make_unique<juce::AudioParameterFloat> ("crossover","Crossover", juce::NormalisableRange<float>(0.0,20000.0, 1.0, 0.2),1000.0);
    params.push_back(std::move(crossOv));
    
    auto gainLeft = std::make_unique<juce::AudioParameterFloat> ("gainLeft", "GainLeft", juce::NormalisableRange<float>(-24, 6, 0.1),0);
    params.push_back(std::move(gainLeft));
    
    auto gainRight = std::make_unique<juce::AudioParameterFloat> ("gainRight", "GainRight", juce::NormalisableRange<float>(-24, 6, 0.1),0);
    params.push_back(std::move(gainRight));
    
    auto gainLinked = std::make_unique<juce::AudioParameterFloat> ("gainLinked", "GainLinked", juce::NormalisableRange<float>(-24, 6, 0.1),0);
    params.push_back(std::move(gainLinked));
    
    
    auto linkGain = std::make_unique<juce::AudioParameterBool>("linkGain", "LinkGain", true);
    params.push_back(std::move(linkGain));
    
    return {params.begin(), params.end()};
}

void StereoimagerAudioProcessor::parameterChanged(const juce::String& parameterID, float newValue)
{
    if (parameterID == "width1")
    {
        float scaledValue;
        if(newValue > 0)
        {
            scaledValue = juce::jmap(newValue, 0.0f, 100.0f, 1.0f, 1.2f);
        }
        else
        {
            scaledValue = juce::jmap(newValue, -100.0f, 0.0f, 0.0f, 1.0f);
        }
        
        width1Value.setTargetValue(scaledValue);
    }
    
    if (parameterID == "width2")
    {
        float scaledValue;
        if(newValue > 0)
        {
            scaledValue = juce::jmap(newValue, 0.0f, 100.0f, 1.0f, 1.2f);
        }
        else
        {
            scaledValue = juce::jmap(newValue, -100.0f, 0.0f, 0.0f, 1.0f);
        }
        
        width2Value.setTargetValue(scaledValue);
    }
    
    if (parameterID == "crossover")
    {

        crossoverValue = newValue;
      
    }
    
    if(linkGainButton)
    {
    if( parameterID == "gainLinked")
    {
    

        
        gainLeftValue.setTargetValue(juce::Decibels::decibelsToGain(newValue * 0.5));
                   gainRightValue.setTargetValue(juce::Decibels::decibelsToGain(newValue * 0.5));
      }
     }

     else if(!linkGainButton)
    {
        
    if (parameterID == "gainLeft")
        {
         
        gainLeftValue.setTargetValue(juce::Decibels::decibelsToGain(newValue * 0.5));
            
        }
    
        if(parameterID == "gainRight")
            {
            gainRightValue.setTargetValue(juce::Decibels::decibelsToGain(newValue * 0.5));
      }        
    }
    
    
    if (parameterID == "linkGain")
    {
        linkGainButton = newValue;
  
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
    rmsLevelLeft.reset(sampleRate, 0.7f);
    rmsLevelRight.reset(sampleRate, 0.7f);
    rmsLevelRight.setTargetValue(-100.0f);
    rmsLevelLeft.setTargetValue(-100.0f);
    //update parameters, call function()
    width1Value.reset(sampleRate, 0.03);
    width2Value.reset(sampleRate, 0.03);
    gainLeftValue.reset(sampleRate, 0.03);
    gainRightValue.reset(sampleRate, 0.03);
    gainLeftValueLinked.reset(sampleRate, 0.03);
    gainRightValueLinked.reset(sampleRate, 0.03);
    crossOverFilterModule.reset();
    crossoverValue = *apvts.getRawParameterValue("crossover");
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = getTotalNumOutputChannels();
    crossOverFilterModule.prepare(spec);
    crossOverFilterModule2.prepare(spec);
    linkGainButton = *apvts.getRawParameterValue("linkGain");
   
    crossOverFilterModule.setType(juce::dsp::LinkwitzRileyFilterType::lowpass);
    crossOverFilterModule2.setType(juce::dsp::LinkwitzRileyFilterType::highpass);
 
    gainLeftValue = juce::Decibels::decibelsToGain(static_cast<float>(*apvts.getRawParameterValue("gainLeft")));
    gainRightValue = juce::Decibels::decibelsToGain(static_cast<float>(*apvts.getRawParameterValue("gainRight")));
    
    gainRightValueLinked = juce::Decibels::decibelsToGain(static_cast<float>(*apvts.getRawParameterValue("gainLinked")));
    
    gainLeftValueLinked = juce::Decibels::decibelsToGain(static_cast<float>(*apvts.getRawParameterValue("gainLinked")));

    
  
    float newValue = static_cast<float>(*apvts.getRawParameterValue("width1"));
    if(newValue > 0)
       {
        width1Value = juce::jmap(newValue, 0.0f, 100.0f, 1.0f, 1.2f);
        }
    else
    {
        width1Value = juce::jmap(newValue, -100.0f, 0.0f, 0.0f, 1.0f);
    }
    
    float newValue2 = static_cast<float>(*apvts.getRawParameterValue("width2"));
    if(newValue > 0)
       {
        width2Value = juce::jmap(newValue2, 0.0f, 100.0f, 1.0f, 1.2f);
        }
    else
    {
        width2Value = juce::jmap(newValue2, -100.0f, 0.0f, 0.0f, 1.0f);
    }
   
    
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


    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

 
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelDataL = buffer.getWritePointer (0);
        auto* channelDataR = buffer.getWritePointer (1);
        
        for(int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
        //Create mid side channels
            
            const auto sideBand1Raw = 0.5 * (channelDataL[sample] - channelDataR[sample]);
            const auto midBand1Raw = 0.5 * (channelDataL[sample] + channelDataR[sample]);
            
            crossOverFilterModule.setCutoffFrequency(crossoverValue);
            crossOverFilterModule2.setCutoffFrequency(crossoverValue);
            const auto sideBand1 = crossOverFilterModule.processSample(channel,sideBand1Raw);
            const auto midBand1 = crossOverFilterModule2.processSample(channel,midBand1Raw);
 
            const auto sideNewBand1 = width1Value.getNextValue() * sideBand1;
            const auto midNewBand1 = (2 - width1Value.getNextValue()) * midBand1;

            const auto newLeftBand1 = midNewBand1 + sideNewBand1;
            const auto newRightBand1 = midNewBand1 - sideNewBand1;
            
            //
            
            const auto sideBand2 = sideBand1Raw - sideBand1;
            const auto midBand2 = midBand1Raw - midBand1;

            const auto sideNewBand2 = width2Value.getNextValue() * sideBand2;
            const auto midNewBand2 = (2 - width2Value.getNextValue()) * midBand2;

            const auto newLeftBand2 = midNewBand2 + sideNewBand2;
            const auto newRightBand2 = midNewBand2 - sideNewBand2;
            
            auto gainLeft = gainLeftValue.getNextValue();
            auto gainRight = gainRightValue.getNextValue();
         
        
            const auto widthScale = juce::Decibels::decibelsToGain(juce::jmap(width1Value.getNextValue(),1.0f, 2.0f, 0.0f, 6.0f));
            const auto widthScaleNeg = juce::Decibels::decibelsToGain(juce::jmap(width2Value.getNextValue(),1.0f, 0.0f, 0.0f, -6.0f));
   
            // Implement gain later!!!!

            channelDataL[sample] =  (newLeftBand1 * widthScale + newLeftBand2 * widthScaleNeg) * gainLeft;
            channelDataR[sample] =  (newRightBand1 * widthScale + newRightBand2 * widthScaleNeg) * gainRight;
   
        }
        
        
        // ..do something to the data...
    }
    rmsLevelLeft.skip(buffer.getNumSamples());
    rmsLevelRight.skip(buffer.getNumSamples());
    {

        auto value = juce::Decibels::gainToDecibels(buffer.getRMSLevel(0, 0, buffer.getNumSamples()));

        if (value < rmsLevelLeft.getCurrentValue())
            rmsLevelLeft.setTargetValue(value);
        else
            rmsLevelLeft.setCurrentAndTargetValue(value);

    }

    {
        auto value = juce::Decibels::gainToDecibels(buffer.getRMSLevel(1, 0, buffer.getNumSamples()));


        if (value < rmsLevelRight.getCurrentValue())
            rmsLevelRight.setTargetValue(value);
        else
            rmsLevelRight.setCurrentAndTargetValue(value);

    }
    
}

//==============================================================================
bool StereoimagerAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* StereoimagerAudioProcessor::createEditor()
{
    return new StereoimagerAudioProcessorEditor (*this);
    
    //return new juce::GenericAudioProcessorEditor(*this);
}

//==============================================================================
void StereoimagerAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
   
    //Save
    juce::MemoryOutputStream stream(destData, false);
    apvts.state.writeToStream(stream);
    
}

void StereoimagerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    //call the save state
    auto tree = juce::ValueTree::readFromData(data, size_t (sizeInBytes));
    
    if(tree.isValid())
    {
        apvts.state = tree;
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new StereoimagerAudioProcessor();
}


float StereoimagerAudioProcessor::getRmsValue(const int channel) const
{
    jassert(channel == 0 || channel == 1);
    if (channel == 0)
        return rmsLevelLeft.getCurrentValue();
    if (channel == 1)
        return rmsLevelRight.getCurrentValue();
    return 0.0f;
    
}

