# Hardware

This directory contains the KiCad files for manufacturing the VasVas Noisebox PCB. Includes schematic, pcb, and BOM.

## VasVas Noisebox v1

The VasVas Noisebox v1 uses a plywood case with the following characteristics:
- Stereo output as two separate jacks. One of them can be used as a stereo (left channel) if the right channel jack is not connected.
- 9..15v power
- 4 potentiometer knobs 
- 1 push button
- 2 LEDs

As for the elements that create mechanical vibrations, you can use them at your discretion. These can be compression and tension springs, various plates, gongs, tuning forks, spinners, radiators and other sources of vibrations. Appropriate fasteners must also be used to secure these elements in the form of bolts, nuts, washers, self-tapping screws, etc.

![app](https://github.com/stone-voices/vasvas/blob/main/hardware/img/vasvas_v1_inside.jpg)

The electrical circuits are mounted by soldering using a MGTF mounting wire (stranded copper wire with fluoroplastic insulation) with a cross section of 0.07 square mm. The output connectors and piezoelectric elements are connected by a shielded wire to reduce interference. The piezoelectric elements are attached to the body by gluing with an adhesive gun with nylon rods.

The prototype was assembled using a 70 x 50 mm breadboard.

![app](https://github.com/stone-voices/vasvas/blob/main/hardware/img/vasvas_v1_breadboard.jpg)

The LEDs, together with current-limiting resistors, are mounted on a small piece of a breadboard.

The layout of the elements is very arbitrary, so you can change it at your discretion based on your needs.

### Some remarks

You can change the schematic diagram of the pre-amplification stage by changing the brand of the operational amplifier to a higher-quality one, or abandon it altogether by replacing it with an amplification stage on a field-effect or bipolar transistor.
