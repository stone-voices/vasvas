# VasVas Noisebox

Noisebox with FX Processor based on Daisy Seed microcontroller. 

[![Demo Video](https://i.ytimg.com/vi/F5kSkdeNpbo/maxresdefault.jpg)](https://www.youtube.com/watch?v=F5kSkdeNpbo)

This noise box has two piezo pickups, the signal from which falls into the left and right channels, respectively, which allows you to get a real stereo signal at the output. In firmware version 1.0, the effects processor includes a reverb based on the Dattorro algorithm and a Shimmer effect that extracts the second harmonic from the signal. A special feature of the reverb is the presence of delay lines and all-pass filters with proportionally variable sizes, as well as a long decay, which can be switched to freezing mode to obtain an undamped reverberation.
In future versions, some changes in the effects used are possible, although this will only affect the software, leaving the hardware unchanged.

This repository includes the circuit diagram and KiCad design files for the PCB, as well as the source code, including all the libraries needed for building.
