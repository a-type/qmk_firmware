# Trackball!

![An assembled Nibble with a trackball](https://i.imgur.com/tHx8BzQ.jpg)

You've found the guide on how to mod a Nibble to incorporate a Pimoroni trackball!

Features of this firmware:

1. Trackball support!
2. Bongo cat!
3. Mac / Windows compat modes!

## Firmware Details

I'm kind of opinionated about stuff, so you'll likely want to modify things. I'll try to clean the code up so that's not so daunting, but for now here's the gist of my decisions. As I clean up hopefully I can extract a lot of these choices to defined flags.

### Trackball behavior

By default the trackball scrolls. I find it more helpful than using it as a pointer. Scroll direction is "classic" (i.e. not "inverted" or "natural") because that's how I do things.
Activate the FN layer to switch to pointing (see below).ß

### FN layer

Bottom left corner is FN, which switches to the function layer while held.
Function layer has the F1-12 keys on the number row.
WASD moves quickly through text (Up = Page Up, Down = Page Down, Left = Prev Word, Right = Next Word).
Esc becomes `/~`.

### Mac Mode

Press the encoder to toggle between Mac and Windows modes. Right now this just swaps Alt and GUI keys. I plan on adding some specific key combination overrides to make working on Windows easier for someone adjusted to Mac.

### Encoder

Defaults to moving forward and backward one word at a time (⌘+L/R on Mac, Ctrl+L/R on Win). Hold FN (bottom left corner) to adjust volume instead.

## Build Guide

The setup is actually quite simple - no different from wiring the OLED. Both the OLED and the trackball communicate with the keyboard using the same i2c bus. That means you actually wire them into the same exact through holes on the PCB. Easy!

![OLED and trackball wires going into the same i2c holes](https://i.imgur.com/kAr2nvd.jpg)

The most important part is to ensure you're wiring the correct line to the right hole. When looking at the PCB, the order of the 4 i2c holes is:

1. Ground (GND) (Black wire)
2. Power (5V) (Red wire - 5v is fine even though the trackball says 3.5)
3. Clock (SCL) (White wire)
4. Data (SDA) (Green wire)

You do **not** need to connect the INT position on the trackball board, we don't use it.

If you use thin braided wire (like the kind you can harvest out of an old microUSB cable) you should be able to fit both sets of wires into the through-holes on the PCB.

Depending on where your OLED and trackball are, you may find it easier to daisy-chain the trackball off the OLED or vice versa. That's up to you. You just need to make sure you connect those 4 wires to the right positions somewhere. Since i2c is a bus, all devices connect to the same sources, so you have a lot of freedom in how and where you connect.

![Trackball and OLED wires both braided together through the holes](https://i.imgur.com/3tOAMMn.jpg)

I used a piece of electrical tape to hold them further in place while I assembled the rest of the board. It's a good idea to get your microcontroller socketed and test the firmware before soldering. Bending the wires over like this should establish enough contact to test functionality.

Check and double-check your wires. Then route them to wherever you want to place your trackball on the keyboard. I like it in between a split space.

![The trackball with wiring into the PCB](https://i.imgur.com/XR4StwH.jpg)

Using the plate is recommended as it holds the trackball perfectly in place with no extra fastening required.

The last thing to get right is avoid getting any of the wires beneath a switch while assembling. I used a thin screwdriver to push the wires out of the way from an adjacent switch position while I socketed each one.

![The wiring beneath the plate](https://i.imgur.com/vf6jx65.jpg)

Once it's all routed, that's it! Connect your MCU, flash the firmware using the `trackball` keymap, and you should see the ball light up.

If not, check your wiring again and ensure you've got all 4 connected to the right place. Check for shorts. The usual. I had a few shorts because I clipped the braided wires and some small filaments fell between the joints.
