/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MyStringModelPlugInAudioProcessor::MyStringModelPlugInAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       ),
#endif
parameters(*this, nullptr, "ParamTreeID", {
    std::make_unique<AudioParameterFloat>("del_time", "Delay Time", 0.01f, 0.99f, 0.25f),
    std::make_unique<AudioParameterFloat>("wet_level", "Wet Level", 0.01f, 0.99f, 0.8f),
    std::make_unique<AudioParameterFloat>("pickup_pos", "Pick up Position", 0.01f, 0.99f, 0.8f),
    std::make_unique<AudioParameterFloat>("decay_time", "Decay Time", 0.01f, 0.99f, 0.5f),
    std::make_unique<AudioParameterFloat>("trigger_pos", "Trigger Position",0.01f, 0.99f, 0.2f),
    std::make_unique<AudioParameterFloat>("attack_val", "Attack",0.01f, 0.99f, 0.1f),
    std::make_unique<AudioParameterFloat>("decay_val", "Decay",0.01f, 0.99f, 0.1f),
    std::make_unique<AudioParameterFloat>("sustain_val", "Sustain",0.01f, 0.99f, 0.99f),
    std::make_unique<AudioParameterFloat>("release_val", "Release",0.01f, 0.99f, 0.1f),
    
    
})
{

    delTimeParameter = parameters.getRawParameterValue("del_time");
    wetLevelParameter = parameters.getRawParameterValue("wet_level");
    pickupPosParameter = parameters.getRawParameterValue("pickup_pos");
    decayTimeParameter = parameters.getRawParameterValue("decay_time");
    triggerPosParameter = parameters.getRawParameterValue("trigger_pos");
    attackParameter = parameters.getRawParameterValue("attack_val");
    decayParameter = parameters.getRawParameterValue("decay_val");
    sustainParameter = parameters.getRawParameterValue("sustain_val");
    releaseParameter = parameters.getRawParameterValue("release_val");
    
  
}

MyStringModelPlugInAudioProcessor::~MyStringModelPlugInAudioProcessor()
{
}

//==============================================================================
const String MyStringModelPlugInAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool MyStringModelPlugInAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool MyStringModelPlugInAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool MyStringModelPlugInAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double MyStringModelPlugInAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int MyStringModelPlugInAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int MyStringModelPlugInAudioProcessor::getCurrentProgram()
{
    return 0;
}

void MyStringModelPlugInAudioProcessor::setCurrentProgram (int index)
{
}

const String MyStringModelPlugInAudioProcessor::getProgramName (int index)
{
    return {};
}

void MyStringModelPlugInAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void MyStringModelPlugInAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    audioEngine.prepare ({ sampleRate, (uint32) samplesPerBlock, 2 });
}

void MyStringModelPlugInAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool MyStringModelPlugInAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
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

void MyStringModelPlugInAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
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
 
 
    audioEngine.setDelayTime(*delTimeParameter);
    audioEngine.setWetLevel(*wetLevelParameter);
    audioEngine.setPickupPosition(*pickupPosParameter);
    audioEngine.setDecayTime(*decayTimeParameter);
    audioEngine.setTriggerPosition(*triggerPosParameter);
    audioEngine.setADSRParameters(*attackParameter, *decayParameter, *sustainParameter, *releaseParameter);

    
    audioEngine.renderNextBlock (buffer, midiMessages, 0, buffer.getNumSamples());
}

//==============================================================================
bool MyStringModelPlugInAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* MyStringModelPlugInAudioProcessor::createEditor()
{
    return new GenericAudioProcessorEditor (*this);
}

//==============================================================================
void MyStringModelPlugInAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    auto state = parameters.copyState();
    std::unique_ptr<XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void MyStringModelPlugInAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
  
   std::unique_ptr<XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
           
           if (xmlState.get() != nullptr)
               if (xmlState->hasTagName (parameters.state.getType()))
                   parameters.replaceState (ValueTree::fromXml (*xmlState));
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MyStringModelPlugInAudioProcessor();
}
