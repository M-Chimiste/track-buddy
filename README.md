# track-buddy
Arduino based head tracker with the BNO08X IMU.

## Hardware
Track Buddy has a very small BOM from an electronics perspective.  It requires an Arduino Pro Micro, a BNO080, and a tactile switch.  The tactile switch is wired into pin 10 and attached to the ground of the Pro Micro. I used a 6mm x 6mm x 9mm one I had lying around, but realistically anything longer than 4mm should work (the top case thickness is ~3mm when printed). The BNO080 is wired to the Pro Micro as follows:

|Pro Micro Pin|BNO080 Pin|
|-----|-----|
|VIN |VIN|
|GND|GND|
|PIN 2|SDA|
|PIN 3|SCL|

I didn't use a PCB for any of the connections.  All connections were soldered in place with wire.  I used some heat shrink tubing on the tactile switch to make sure it wouldn't short anything.

Note on the MK2 and M1.5 variants: Instead of one button there are two buttons both wired to ground and two open digital pins (code has 9,10).  One button has a pin going to ground and another wire connecting it to the other button to share a common ground.  Due to the number of ground pins on the pro micro you could simply just wired each button to a unique ground pin on the Arduino.  These buttons are used for added functionality around pausing tracking updates (for filming on games like DCS) and the original tare functionality.

## Housing
Housing is completely 3D printed.  Original housing was created with a QIDI Print X-Plus with PLA filament at 35% infill and supports around the usb socket (but realistically anything above 20% should work with no issues).  Almost any 3D printer should be sufficient for printing the shell.  In addition to the 3D printed parts you also need four M2 nuts and four M2 screws.  The M2 nuts were bonded to the 3D printed housing with a small amount of epoxy (I like to use JB Weld Kwik, but any epoxy should do).  After bonding the nuts you can affix the tactile switch to the top of the cover using some hot glue (or other adhesive, though I didn't need to use any since everything fit quite snug without it).

Note on the MK2 and MK1.5 variants:  On these variants I changed the parts list very slightly.  Additionally, I stopped using this he M2 nuts being bonded to the shell.  Instead, I purchased some M2 brass threaded inserts which I used a soldering iron to melt into the shell.  It provides for a cleaner looking finished product.

## Software

Example baords.txt is included now so that you can have track buddy called track buddy on your PC's USB HID.  I might package this up into a custom hardware folder that you can use in the future without modifying your boards.txt, but it is not currently a priority.

Arduino IDE 1.8.15 was used for flashing the pro micro board.  Make sure you install the IDE and all the Arduino drivers.  Install the Joystick library and the Sparkfun BNO080 library.  Select the Leonardo Board and the COM port of your board and hit upload.

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

For all other settings (output and filter), I left them as the default values (i.e. didn't touch them).  I do not plan to try to implement X,Y,Z, but I know there are ways to pseudo implement them.  Mainly, I don't like them when playing flight sims, but that's just my personal preference (feel free to fork and implement them).

After configuration put on your headset, make sure you are facing forward and click start.  If you find you are drifting (I've not have this problem yet) face the direction you want forward to be and click the button on the top of Track Buddy to internally offset for the drift.  That said, because the BNO080 has an internal compass I've found that I don't see much if any drift while playing, even for extended periods of time.

## SPECIAL IMPORTANT NOTE
BNO080 has an internal compass.  Don't put Track Buddy too close to a magnetic source (e.g. headphone speaker) or you may get undesirable behavior.  This is because the device uses the compass in order to help mitigate drift.

