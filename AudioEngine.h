//
//  AudioEngine.h
//  MyStringModelPlugIn
//
//  Created by 樊嘉琪 on 26/04/2020.
//

#ifndef AudioEngine_h
#define AudioEngine_h

#include "Voice.h"
#include "Distortion.h"
#include "CabSimulator.h"

class AudioEngine  : public juce::MPESynthesiser
{
public:
    static constexpr size_t maxNumVoices = 4;

    //==============================================================================
    AudioEngine()
    {
        for (size_t i = 0; i < maxNumVoices; ++i)
            addVoice (new Voice);

        setVoiceStealingEnabled (true);
    }

    //==============================================================================
    void prepare (const juce::dsp::ProcessSpec& spec) noexcept
    {
        setCurrentPlaybackSampleRate (spec.sampleRate);

        for (auto* v : voices)
            dynamic_cast<Voice*> (v)->prepare (spec);

        fxChain.prepare (spec);
    }

private:
    //==============================================================================
    enum
    {
        distortionIndex,
        cabSimulatorIndex,
        delayIndex,
        reverbIndex
    };

    juce::dsp::ProcessorChain<Distortion<float>, CabSimulator<float>,Delay<float>, juce::dsp::Reverb> fxChain;

    //==============================================================================
    void renderNextSubBlock (AudioBuffer<float>& outputAudio, int startSample, int numSamples) override
    {
        MPESynthesiser::renderNextSubBlock (outputAudio, startSample, numSamples);

        auto block = juce::dsp::AudioBlock<float> (outputAudio).getSubBlock ((size_t) startSample, (size_t) numSamples);
        auto context = juce::dsp::ProcessContextReplacing<float> (block);
        fxChain.process (context);
    }
};


#endif /* AudioEngine_h */
