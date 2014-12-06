# Control

The ProtoX quadcopter comes with a small 4 channel remote. It also works with the larget HUBSAN remote (made for the X4 H107 Quad)

The larger remote is able to enable/disable expert/stunt mode as well as enable/disable flips.

![Controllers](/images/IMG_2722.jpg)

## Expert/stunt mode
This mode makes the controls much more sensitive, which allows for quicker manouvers. The small remote doesn't seem to have a way to enable this but the larger one does it by clicking on the right stick.

Looking at the packet captures, expert mode effectively doubles the magnitude of all control signals (minus the throttle).

## Flips
Flips are enabled/disabled on the larger remote by clicking on the left stick. As expected, the smaller remote doesn't enable this feature.

When flips are enabled, moving the right stick all the way in any direction and back quickly will make the quad do a flip. This only works with the controls on expert mode.

Bit 4 on byte 9 on the control packet (counting from 0) has the flips enabled/disabled setting. Thanks to [goebish on the deviationtx.com forum](http://www.deviationtx.com/forum/protocol-development/1848-new-hubsan-upgraded-version-on-the-way#12552) for figuring this out.

To see a sample capture with expert mode and flips enabled, see [this one.](processeddata/quad-start-to-expertmode2.decoded.txt)

## Expert Mode/Flips on Small Remote
From what I've seen so far, there's nothing preventing the little remote from entering expert mode and enabling/disabling flips. The remote itself has buttons under both sticks. A quick firmware update should be able to provide the new functionality. Unfortunately, I've never used STM8 devices and have no programmer/debugger for them :-(

## Packet Format

This is what I know so far about the control packet format

Byte | Description
---- | -----------
0x00 | Packet Type(?) - 0x20
0x01 | 
0x02 | Throttle (0x00 - 0xFA)
0x03 | 
0x04 | Yaw Left (0x80-0x34) - Yaw Right (0x80-0xCC)
0x05 | 
0x06 | Pitch Forward (0x80-0x34) - Pitch Backward (0x80-0xCC)
0x07 | 
0x08 | Roll Left (0x80-0xCC) - Roll Right (0x80-0x34)
0x09 | Enable Flips if bit 3 is set (0x08)
0x0A | 
0x0B | 
0x0C | 
0x0D | 
0x0E | 
0x0F | Checksum
