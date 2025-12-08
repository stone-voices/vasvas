#include "daisy_petal.h"
#include "daisysp.h"

#include "tools.h"
#include "reverb.h"
#include "fastsin.h"
#include <q/support/literals.hpp>
#include <q/fx/biquad.hpp>
#include "multirate.h"
#include "octaver.h"

#define BUFFER_SIZE 48
#define LED_1 22
#define FOOTSWITCH_1 0
#define FOOTSWITCH_2 1
#define KNOB_1 4
#define	KNOB_2 2
#define	KNOB_3 0
#define	KNOB_4 1

namespace q = cycfi::q;
using namespace q::literals;

using namespace daisy;
using namespace daisysp;


DaisyPetal hw;
::daisy::Parameter rsize, mix, rdecay, shimmer;
float psize, pmix, pdecay, pshimmer;
float vmix, vdecay, vsize, vshimmer;
Led led1;

float dryMix = 1;
float wetMix = 0;

TReverb reverb;

bool freeze = false;

static const auto sample_rate_temp = 48000; //hard code for now 
static Decimator2 decimate;
static Interpolator interpolate; 
static OctaveGenerator octave(sample_rate_temp / resample_factor); // resample_factor is defined in Multirate.h and equals 6
static q::highshelf eq1(-11, 140_Hz, sample_rate_temp);
static q::lowshelf eq2(5, 160_Hz, sample_rate_temp);   // NOTE: the sample_rate must be divisible by the resample_factor (48/6 = 8)

float buff[6];
float buff_out[6];
int bin_counter = 0;

bool KnobMoved(float old_value, float new_value)
{
    float tolerance = 0.005f;
    if (new_value > (old_value + tolerance) || new_value < (old_value - tolerance)) 
        return true;
    else 
        return false;
}

void UpdateButtons()
{
    if (hw.switches[FOOTSWITCH_1].FallingEdge())
    {
      freeze = !freeze;
			reverb.SetFreeze(freeze);	
    }

    led1.Set(freeze ? 1.0f : 0.0f); 
}


// This runs at a fixed rate, to prepare audio samples
static void AudioCallback(AudioHandle::InputBuffer  in,
                          AudioHandle::OutputBuffer out,
                          size_t                    size)
{

    hw.ProcessAnalogControls();
    hw.ProcessDigitalControls();
    led1.Update();

    UpdateButtons();


    vdecay = rdecay.Process();
		vsize = rsize.Process();
		vshimmer = shimmer.Process();
    vmix = mix.Process();

    if (pmix != vmix) 
		{
        if (KnobMoved(pmix, vmix)) 
				{
            //    A cheap mostly energy constant crossfade from SignalSmith Blog
            float x2 = 1.0 - vmix;
            float A = vmix * x2;
            float B = A * (1.0f + 1.4186f * A);
            float C = B + vmix;
            float D = B + x2;

            wetMix = C * C;
            dryMix = D * D;
            pmix = vmix;
        }
    }


    if (KnobMoved(pdecay, vdecay)) 
		{
        pdecay = vdecay;
				reverb.SetDecay(vdecay);
    }


    if (KnobMoved(psize, vsize)) 
		{      
        psize = vsize;    
				reverb.SetSize(vsize);
    }
		
		if (KnobMoved(pshimmer, vshimmer)) 
		{
        pshimmer = vshimmer;
    }

    float inputL;
    float inputR;

    for (size_t i = 0; i < size; ++i)
    {

        inputL = in[0][i];
        inputR = in[1][i];
 
        // NOTE: Octave before reverb sounds better (personal preference), and doing octave after reverb would require another polyoctave for second channel anyway
        buff[bin_counter] = (inputL + inputR) * 0.5f;
        // do calculation every 6 samples
        if (bin_counter > 4) 
				{

            std::span<const float, resample_factor> in_chunk(&(buff[0]), resample_factor);  // std::span is c++ 20 feature
                
            const auto sample = decimate(in_chunk); 
  
            octave.update(sample);

            float octave_mix = pshimmer * octave.up1() * 2.0;

            auto out_chunk = interpolate(octave_mix);
            for (size_t j = 0; j < out_chunk.size(); ++j)
            {
                float mix = eq2(eq1(out_chunk[j]));        
                mix += 0.5 * buff[j]; 
                buff_out[j] = mix;    
            }

        }
                // Sets increments the buffer index from 0 to 5 (workaround to adapt code)
        bin_counter += 1;
        if (bin_counter > 5)
            bin_counter = 0;

        float reverb_in = buff_out[bin_counter]; 
        float outL, outR;
        reverb.Process(reverb_in, outL, outR);


      out[0][i] = in[0][i] * dryMix + outL * wetMix; 
      out[1][i] = in[1][i] * dryMix + outR * wetMix;
    }
}

int main(void)
{
    hw.Init(true);
    float samplerate;
    samplerate = hw.AudioSampleRate();

    TFastSin::Initialize();
   

		vmix = vdecay = vsize = vshimmer = 0.0;
		pmix = pdecay = psize = pshimmer = 0.0;
		
		reverb.SetSampleRate(samplerate);

    hw.SetAudioBlockSize(BUFFER_SIZE);

    // Initialize buffers to 0
    for (int j = 0; j < 6; ++j) 
	  {
        buff[j] = 0.0;
        buff_out[j] = 0.0;
    }

    rdecay.Init(hw.knob[KNOB_1], 1.0f, 100.0f, ::daisy::Parameter::EXPONENTIAL);
		rsize.Init(hw.knob[KNOB_2], 0.25f, 4.0f, ::daisy::Parameter::EXPONENTIAL);
		shimmer.Init(hw.knob[KNOB_3], 0.0f, 1.0f, ::daisy::Parameter::LINEAR);
    mix.Init(hw.knob[KNOB_4], 0.0f, 1.0f, ::daisy::Parameter::LINEAR);
		   

    led1.Init(hw.seed.GetPin(LED_1),false);
    led1.Update();
    

    hw.StartAdc();
    hw.StartAudio(AudioCallback);
    while(1)
    {
	    System::Delay(100);
    }
}