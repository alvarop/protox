I'm trying to reverse engineer the [Proto-X](http://www.protoquad.com) radio protocol to build my own controllers (and maybe mess with others...) Similar things have been done in the past, but I figured it would be a fun learning experience. This repo has my current findings.


### Current Status
I am able to capture SPI data going between the STM8 microcontroller and the A7105 with my Saleae Logic analyzer. I expor the SPI capture data to CSV and use a [python script](quadcsv.py) to process it into 'packets'

So far, I have a decent understanding of the [startup](startup.md) sequence and some idea about how the remote and quadcopter [pair/sync][sync.md] together. I think I've figured out which bytes on the radio packet are used for the basic controls.

All radio packets (for both pairing and control) have an 8-bit checksum. It's a fairly simple computation. I used another [python script](checksum.py) to test and verify it.

Next step is to figure out the channel selection criteria and better understand the device pairing. I've ordered some radio modules to play with. Once those arrive, I should be able to build my own controller (or use my computer to do it.)

See the following files for much more detailed information on each topic:


### Files

See [startup.md](startup.md) for initial configuration and channel selection

See [sync.md](sync.md) for notes on device sync/pairing protocol

See [notes.md](notes.md) for miscellaneous project notes and intermediate results.

See [checksum.py](checksum.py) for packet checksum computation and explanation.

See [quadcsv.py](quadcsv.py) for script to process Saleae Logic SPI CSV data into more manageable chunks
