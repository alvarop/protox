# ProtoX Reversing Project

I'm trying to reverse engineer the [ProtoX](http://www.protoquad.com) radio protocol to build my own controllers (and maybe mess with others...) Similar things have been done in the past, but I figured it would be a fun learning experience. This repo has my current findings.

This project was inspired by [@scanlime](https://twitter.com/scanlime)'s awesome [Coastermelt](https://vimeo.com/channels/coastermelt) videos. Check them out if you haven't already! The ProtoX project is much simpler, but I have to start somewhere...

![Remote+Quad](/images/IMG_2328.jpg)

### Current Status
I am able to capture SPI data going between the STM8 microcontroller and the A7105 with my Saleae Logic analyzer. I export the SPI capture data to CSV and use a [python script](quadcsv.py) to process it into 'packets'. You can see an example processed capture [here](processeddata/connect2.processed.txt).

After what seemed an eternity, I was able to solder some wires to sniff the SPI data on the quadcopter itself. The microscope came in handy while trying to solder tiny wires on to pins on a QFN device. This allows me to capture both radio streams simultaneously and get a complete picture of what exactly goes on during pairing and operation. A processed packet capture with both the remote and quad data can be found [here](processeddata/dual-capture.decoded.txt).

I've added the [a7105.py](a7105.py) module which allows for plaintext decoding of the A7105 radio packets. This makes reading captures much simpler, since I no longer need to look up what each register means... You can see an example decoded capture [here](processeddata/connect2.decoded.txt).

So far, I have a decent understanding of the [startup](startup.md) sequence and some idea about how the remote and quadcopter [pair/sync](sync.md) together. I think I've figured out which bytes on the radio packet are used for the basic controls.

All radio packets (for both pairing and control) have an 8-bit checksum. It's a fairly simple computation. I used another [python script](checksum.py) to test and verify it.

Next step is to figure out the channel selection criteria and better understand the device pairing. I've ordered some radio modules to play with. Once those arrive, I should be able to build my own controller (or use my computer to do it.)

See the following files for much more detailed information on each topic:


### Files

See [startup.md](startup.md) for initial configuration and channel selection for both the remote and the quad

See [sync.md](sync.md) for notes on device sync/pairing protocol

See [notes.md](notes.md) for miscellaneous project notes and intermediate results.

See [hardware.md](hardware.md) for hardware notes (pinouts, etc...)


See [a7105.py](a7105.py) for python module that decodes A7105 SPI packets into readable text

See [checksum.py](checksum.py) for packet checksum computation and explanation.

See [quadcsv.py](quadcsv.py) for script to process Saleae Logic SPI CSV data into more manageable chunks
