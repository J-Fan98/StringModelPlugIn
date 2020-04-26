//
//  CustomOscillator.h
//  DSPDelayLineTutorial
//
//  Created by 樊嘉琪 on 20/04/2020.
//  Copyright © 2020 JUCE. All rights reserved.
//

#ifndef CustomOscillator_h
#define CustomOscillator_h


#include <JuceHeader.h>

template <typename Type>
class CustomOscillator
{
public:
    //==============================================================================
    CustomOscillator()
    {
        setWaveform (Waveform::sine);

        auto& gain = processorChain.template get<gainIndex>();
        gain.setRampDurationSeconds (3e-2);
        gain.setGainLinear (Type (0));
    }

    //==============================================================================
    enum class Waveform
    {
        sine,
        saw
    };

    void setWaveform (Waveform waveform)
    {
        switch (waveform)
        {
        case Waveform::sine:
            processorChain.template get<oscIndex>().initialise ([] (Type x)
                                                                {
                                                                    return std::sin (x);
                                                                }, 128);
            break;

        case Waveform::saw:
            processorChain.template get<oscIndex>().initialise ([] (Type x)
                                                                {
                                                                    return jmap (x, Type (-double_Pi), Type (double_Pi), Type (-1), Type (1));
                                                                }, 128);
            break;

        default:
            jassertfalse;
            break;
        }
    }

    //==============================================================================
    void setFrequency (Type newValue, bool force = false)
    {
        processorChain.template get<oscIndex>().setFrequency (newValue, force);
    }

    void setLevel (Type newValue)
    {
        processorChain.template get<gainIndex>().setGainLinear (newValue);
    }

    void reset() noexcept
    {
        processorChain.reset();
    }

    //==============================================================================
    template <typename ProcessContext>
    void process (const ProcessContext& context) noexcept
    {
        auto&& outBlock = context.getOutputBlock();
        auto blockToUse = tempBlock.getSubBlock (0, outBlock.getNumSamples());
        juce::dsp::ProcessContextReplacing<float> tempContext (blockToUse);
        processorChain.process (tempContext);

        outBlock.copyFrom (context.getInputBlock()).add (blockToUse);
    }

    //==============================================================================
    void prepare (const juce::dsp::ProcessSpec& spec)
    {
        tempBlock = juce::dsp::AudioBlock<float> (heapBlock, spec.numChannels, spec.maximumBlockSize);
        processorChain.prepare (spec);
    }

private:
    //==============================================================================
    juce::HeapBlock<char> heapBlock;
    juce::dsp::AudioBlock<float> tempBlock;

    enum
    {
        oscIndex,
        gainIndex,
    };

    juce::dsp::ProcessorChain<juce::dsp::Oscillator<Type>, juce::dsp::Gain<Type>> processorChain;
};
#endif /* CustomOscillator_h */
