# track-buddy
Arduino based head tracker with the BNO08X IMU.

## Hardware
Track Buddy has a very BOM from an electronics perspective.  It requires an Arduino Pro Micro, a BNO080, and a tactile switch.  The tactile switch is wired into pin 10 and attached to the ground of the Pro Micro. I used a 6mm x 6mm x 9mm one I had lying around, but realistically anything longer than 4mm should work (the top case thickness is ~3mm when printed). The BNO080 is wired to the Pro Micro as follows:

|Pro Micro Pin|BNO080 Pin|
|-----|-----|
|VIN |VIN|
|GND|GND|
|PIN 2|SDA|
|PIN 3|SCL|

I didn't use a PCB for any of the connections.  All connections were soldered in place with wire.  I used some heat shrink tubing on the tactile switches to make sure they didn't short anything.

## Housing
Housing is completely 3D printed.  Original housing was created with a QIDI Print X-Plus with PLA filament at 35% infill.  Any 3D printer should be sufficient for printing this and you can probably get away will less infill if you want.  In addition to the 3D printed parts you also need four M2 nuts and four M2 screws.  The M2 nuts were bonded to the 3D printed housing with a small amount of epoxy (I like to use JB Weld Kwik, but any epoxy should do).  After bonding the nuts you can affix the tactile switch to the top of the cover using some hot glue (or other adhesive, though I didn't need to use any since everything fit quite snug without it).

## Software

Coming Soon: Custom Board Definition (copy folder into Arduino directory to see the board and select it so it says track-buddy)

Here Now:

Flashing the board is pretty easy.  I used Arduino IDE 1.8.15.  Make sure you install the IDE and all the Arduino drivers.  Install the Joystick library and the Sparkfun BNO080 library (I need to check licenses to see if I am allowed to package in this repo).  Select the Leonardo Board and the COM port of your board and hit upload.

## Configuring with OpenTrack

Track Buddy is meant to be used in conjunction with the OpenTrack software.  There are a lot of tutorials online that show you how to set up OpenTrack and adjust curves for playing your games, so I won't get into that here.  What I will provide is how to get yaw/pitch/roll into OpenTrack to get you going.

First: Make sure that you select your input as Joystick input and select the appropriate device (Either Arduino Leonardo or Track Buddy depending on how you flashed it).

Second: Configure your mapping as follows:
|Mapping|Value|
|-----|-----|
|X|Disabled|
|Y|Disabled|
|Z|Disabled|
|Yaw|Joystick Axis #7|
|Pitch|Joystick Axis #2|
|Roll|Joystick Axis #3|

I think I know why the order is weird, and in a subsequent release I plan to change the axis to 1,2,3 for simplicity.  I will update the documentation when I make that update.  For all other settings (output and filter), I left them as the default values (i.e. didn't touch them).  I do not plan to try to implement X,Y,Z, but I know there are ways to pseudo implement them.  Mainly, I don't like them when playing flight sims, but that's just my opinion.

After configuration put on your headset, make sure you are facing forward and click start.  If you find you are drifting (I've not have this problem yet) face the direction you want forward to be and click the button on the top of Track Buddy to internally offset for the drift.

## SPECIAL IMPORTANT NOTE
BNO080 has an internal compass.  Don't put Track Buddy too close to a magnetic source (e.g. headphone speaker) or you may get undesirable behavior.  This is because the device uses the compass in order to help mitigate drift.

