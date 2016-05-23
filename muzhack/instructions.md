# Shopping List

You will need the following stuff -

* [LittleBits synth kit](http://littlebits.cc/kits/synth-kit)
* EITHER
  * 2x JST SH Jumper 3 Wire Assembly (available at [Sparkfun](https://www.sparkfun.com/products/10358))
* OR
  * 2x JST-SH 3-pin 1mm connectors (available at [Sparkfun](https://www.sparkfun.com/products/10358) in the US or [Protopic](http://proto-pic.co.uk/connector-1-0mm-horizontal-3-pin/) in the UK)
  * **Materials to etch your own, small, simple PCB** (unless you order them from a PCB manufacturer)
    * I won't go into the details in this project, but there's many other guides on how to make them -
    http://www.instructables.com/id/How-to-Etch-a-PCB/
    http://www.instructables.com/id/Cheap-and-Easy-Toner-Transfer-for-PCB-Making/
* An **Arduino Leonardo**, must be a Leonardo if you want to do USB MIDI, other Arduinos will work otherwise.
* [A Breadboard](http://en.wikipedia.org/wiki/Breadboard)
* **Some solid core wire**, the right size to fit in the breadboard (around 1mm) [like this](http://www.hobbytronics.co.uk/breadboard-wire), available at any electronics shop.
* **MIDI Sequencing software**, I use [Ableton Live](http://www.ableton.com/), but [Garageband](https://www.apple.com/mac/garageband/), [FL Studio](http://www.image-line.com/flstudio/) or [Reaper](http://www.reaper.fm/) (free!) will work.

# Step 1: Make the Connector

![Connector](http://aknuds1.github.io/Littlebits-Arduino/assets/images/connector1.jpg "Connector")

We're going to connect the littlebits to the Arduino via a breadboard. Currently there's no way to get or make your own 'bitsnap' connector, that's the three-pinned magnetic plastic thing on your littlebits. So instead of making our own, we'll use the 'split' module, which handily uses readily available connectors for it's wires. This means we **don't need to make any permanent changes to the littlebits :)**

Instead of cutting cables, or breaking apart the connectors, we can take advantage of the little white connector on the split module, replacing the input on that with our own from the Arduino. There's a couple of options here, you can either use the jumper-wires in the first picture (much easier, harder to find, a bit more expensive) or use the surface-mount connectors shown in the second picture (advanced, easer to find, cheaper).

## Jumper wires

Simply strip the ends of the wires and either solder a short length of solid-core wire to each of the three ends. If you're relatively new to soldering you can [find out how to do it on this instructable](http://www.instructables.com/id/Soldering-wires-together/).

Now, take your *split* module and remove the two input ends, those are the ones that only have one set of wires going to them. Just pull the white plug out of it's plastic socket. It might need a bit of a wiggle but they're fairly easy.

Insert your plug (along with it's nicely soldered tails) into the socket on the input bit. You've now got a connector to plug your littlebits into your breadboard! Simple as that.

## Surface-mount connectors

Now, this option may not be suitable for any beginner solderers out there, it involves some VERY small pins, that are very close together. With that in mind, here we go…

The eagle files and printable board for the little adapter board are up on the [project's github](https://github.com/tub/Littlebits-Arduino/tree/master/adapter-board-files).

It was very nicely laid out and etched for me by [Alex](https://github.com/alexanderhosford) at [my local hackspace](http://www.buildbrighton.com/) (I was feeling lazy and he's easily persuaded). I won't go into the PCB making details in this instructable, but there's many other guides on how to make them, e.g

* http://www.instructables.com/id/Etching-Method-and-Sticker-Method-of-PCB-fabricati/
* http://www.instructables.com/id/How-to-Etch-a-PCB/

If you're using the PNG image of the board layout to etch, remember to set the print-size to 2400 DPI otherwise it'll print at the wrong size!

Since the boards are so small we etched eight of them in one go (photo #4), to get them apart I just scored them **carefully** with a craft knife a few times and snapped them (photo #5).

## Soldering Tips

I managed to solder the surface mount connector by simply tinning each pin, along with the two anchoring tags at the front of the connector, then tinning the pads on the board (photo #6). All I had to do then was heat up each pad until the solder on them melted and stuck the component to the board. It wasn't as tricky as I thought it was going to be.

The three header pins go in the three holes towards the back of the board, these are soldered like a normal through-hole component, no special treatment. The finished boards can be seen in the last photo on this step.

Next, just unplug the wires from the input end of the split bit and plug them into your adapter boards. Now you've got a breadboard to littlebits adapter!

# Step 2: PWM filter circuit

![PWM](http://aknuds1.github.io/Littlebits-Arduino/assets/images/pwm1.jpg "PWM")

The Arduino sketch we're going to use takes advantage of the analogWrite function, which uses pulse-width modulation to fake an analogue output. This means that instead of actually turning the voltage up and down, it switches it on and off very quicky. To fake low voltages, it leaves an output mostly off but turns it on for a reeeeeeally short amount of time, thousands of times a second. To fake higher voltages it turns the output on (which is 5v) most of the time, turning it off for a tiny amount of time, again, thousands of times a second. This is better summarised on the [Secrets of Arduino PWM page](http://arduino.cc/en/Tutorial/SecretsOfArduinoPWM).

Annoying this **thousands of times a second** is 490 Hz which is in the frequency range we can hear, so it turns up as a buzz on the littlebits speaker! You can hear this in the video attached to this step, it's supposed to just be a rising tone, but instead there's also the buzz. In order to get rid of this buzz we need to put the signal through a low-pass filter to smooth out the choppy waveform. We can achieve this using just a resistor and a capacitor.

The first picture shows an approximation of what I set up on my breadboard, except it's easier to follow (and says Arduino Uno, but ignore that bit).

It's worth pointing out that the colours on the littlebits wires are slightly confusing -

* Red - Ground, 0v
* Orange - Signal
* White - 5v

# Step 3: Arduino software

![Arduino Software](http://aknuds1.github.io/Littlebits-Arduino/assets/images/arduino-software.jpg "Arduino Software")

## USB MIDI with arcore

In order to make the MIDI side of this project really easy, I've used a modified version of the Leonardo firmware called arcore. It adds USB MIDI support so you can just plug in the Arduino and it gets recognised as a MIDI interface in your sequencing software. Installation pretty simple, instructions are on [the arcore project page](https://github.com/rkistner/arcore).

Once it's installed and you've restarted the Arduino software, make sure you select the *"Arduino Leonardo (arcore)"* board from the Board menu as shown in the picture.

### The sketch

The Arduino sketch, which is on [my github project](https://github.com/tub/Littlebits-Arduino), does several things with the MIDI data it receives.

### MIDI Clock

When the Arduino is sent a MIDI clock signal, it will pull pin 3 high, and output an inverted pulse for every beat. When a stop signal is sent, pin 3 will go back to 0v. This allows us to hook up a MIDI clock to trigger the micro-sequencer littlebit, and keep it in time.

### MIDI Notes

If note data is sent to the Arduino, pin 5 is set to a voltage proportional to the note's frequency, which allows us to use MIDI notes as a control voltage. e.g. to control an oscillator's pitch. Pin 4 will get a pulse sent to it when a note-on is fired, this allows us to trigger envelopes or other bits when a note is pressed.

### MIDI CC messages

Pin 6 will output a voltage proportional to any MIDI Control Change #1 messages received. This allows us to link controls within software or even knobs on MIDI controllers to littlebits.

### Installing the sketch
Simply download the [.ino file the project page](https://github.com/tub/Littlebits-Arduino/blob/master/littleBitsMidiNotes.ino) and open it in the Arduino editor. Hit upload (remember to select the *arcore* board in the Boards menu).

# Step 4: Go!

![Go!](http://aknuds1.github.io/Littlebits-Arduino/assets/images/go.jpg "Go!")

Now you're ready to experiment with your MIDI Littlebits.

There's a quick project to get you started in the picture attached to this step. It synchronises the *micro sequencer bit* with your MIDI clock. Make sure you have 5v and GND connected to the correct wires (check step 2 if you're not sure), and connect the middle wire to pin 3, the clock pulse pin. Then connect the micro sequencer to your Arduino-littlebit converter, then two oscillators, then the speaker.

In your MIDI sequencing software, make sure that the MIDI clock is being sent to the *Arduino Leonardo* MIDI device.
