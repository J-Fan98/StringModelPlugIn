//
//  Voice.h
//  DSPDelayLineTutorial
//
//  Created by 樊嘉琪 on 20/04/2020.
//  Copyright © 2020 JUCE. All rights reserved.
//

#ifndef Voice_h
#define Voice_h
#include "CustomOscillator.h"
#include "WaveguideString.h"
//==============================================================================
class Voice  : public juce::MPESynthesiserVoice
{
public:
    Voice()
    {
        auto waveform = CustomOscillator<float>::Waveform::saw;
        processorChain.get<oscIndex>().setWaveform (waveform);

        auto& masterGain = processorChain.get<masterGainIndex>();
        masterGain.setGainLinear (0.7f);
    }

    //==============================================================================
    void prepare (const juce::dsp::ProcessSpec& spec)
    {
        tempBlock = juce::dsp::AudioBlock<float> (heapBlock, spec.numChannels, spec.maximumBlockSize);
        processorChain.prepare (spec);
        env.setSampleRate (spec.sampleRate);
    }

    //==============================================================================
    void noteStarted() override
    {
        auto velocity = getCurrentlyPlayingNote().noteOnVelocity.asUnsignedFloat();
        auto freqHz = (float) getCurrentlyPlayingNote().getFrequencyInHertz();

        processorChain.get<oscIndex>().setFrequency (freqHz, true);

        //processorChain.get<oscIndex>().setLevel (velocity);
        
        processorChain.get<stringIndex>().setPickupPosition(pickupPosValue);
        processorChain.get<stringIndex>().setDecayTime(decayTimeValue);
        processorChain.get<stringIndex>().setTriggerPosition(triggerPosValue);

        env.noteOn();
        env.setParameters(adsrParams);


        auto& stringModel = processorChain.get<stringIndex>();
        stringModel.setFrequency (freqHz);
        stringModel.trigger (velocity);

    }

    //==============================================================================
    void notePitchbendChanged () override
    {
        auto freqHz = (float) getCurrentlyPlayingNote().getFrequencyInHertz();
        processorChain.get<oscIndex>().setFrequency (freqHz);
    }

    //==============================================================================
    void noteStopped (bool) override
    {
        env.noteOff();
        processorChain.get<oscIndex>().setLevel (0.0f);
    }
    
    void setPickupPos (float newPos)
    {
        pickupPosValue = newPos;
    }
    
    void setTriggerPos(float newPos)
    {
        triggerPosValue = newPos;
    }
    
    void setDecayTime (float newDecayTime)
    {
        decayTimeValue = newDecayTime;
    }
    
    
    void getEnvelopeParameter(float attack, float decay, float sustain, float release)
    {
        adsrParams.attack = attack;
        adsrParams.decay = decay;
        adsrParams.sustain = sustain;
        adsrParams.release = release;
    }
    
    //==============================================================================
    void notePressureChanged() override {}
    void noteTimbreChanged() override   {}
    void noteKeyStateChanged() override {}

    //==============================================================================
    void renderNextBlock (AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override
    {
        auto block = tempBlock.getSubBlock (0, (size_t) numSamples);
        block.clear();
        juce::dsp::ProcessContextReplacing<float> context (block);
        processorChain.process (context);

        // silence detector
        bool active = false;
        for (size_t ch = 0; ch < block.getNumChannels(); ++ch)
        {
            auto* channelPtr = block.getChannelPointer (ch);

            for (int i = 0; i < numSamples; ++i)
            {
                if (channelPtr[i] != 0.0f)
                {
                    active = true;
                    break;
                }
            }
        }

        if (active)
        {
            juce::dsp::AudioBlock<float> (outputBuffer)
                .getSubBlock ((size_t) startSample, (size_t) numSamples)
                .add (tempBlock);
            //env.applyEnvelopeToBuffer (outputBuffer, startSample, numSamples);
            
        }
        else
        {
            clearCurrentNote();
        }
    }

private:
    //==============================================================================
    juce::HeapBlock<char> heapBlock;
    juce::dsp::AudioBlock<float> tempBlock;
    
    float pickupPosValue;
    float decayTimeValue;
    float triggerPosValue;

    ADSR env; // ADSR envelope
    ADSR::Parameters adsrParams;

    enum
    {
        oscIndex,
        stringIndex,
        masterGainIndex
    };

    juce::dsp::ProcessorChain<CustomOscillator<float>, WaveguideString<float>, juce::dsp::Gain<float>> processorChain;
};

#endif /* Voice_h */
