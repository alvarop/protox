# ProtoX Reversing Project

I'm trying to reverse engineer the [ProtoX](http://www.protoquad.com) radio protocol to build my own controllers (and maybe mess with others...) Similar things have been done in the past, but I figured it would be a fun learning experience. This repo has my current findings.

This project was inspired by [@scanlime](https://twitter.com/scanlime)'s awesome [Coastermelt](https://vimeo.com/channels/coastermelt) videos. Check them out if you haven't already! The ProtoX project is much simpler, but I have to start somewhere...

**NOTE:** While there's some basic information about the project results on this page, most of the details are in other files/pages. 

![Remote+Quad](/images/IMG_2328.jpg)

### Current Status
#### Super Current
I now have a fully functional sniffer. At the moment, scans all channels (like the quad) until it hears the remote. It then follows the pairing with the quad and decodes control packets live.

As far as hijacking another quad, I'm making progress, but not enough. I am able to do a simple Denial-of-service by just flooding the channel with packets. This causes the quad to stop receiving commands which can be entertaining. An important note is that this only works with the little remotes. The large hubsan one seems to have better antennas or something, because it refuses to go down.

The goal is to be able to not only prevent the quad from receiving commands from the remote, but inject a different command in order to take control. So far I haven't had much luck. Sometimes I'm able to intermittently get malicious commands through, but not reliably. To start, I'm trying to turn off the remote and 'resume' control on my own. For some reason the quad just gets CRC errors and never actually receives my commands. 

#### Used to be Current
I screwed up and thought they used encryption. They do write to the data whitening/encryption register during pairing, but only to enable CRC on the radio! This means listening in (and possibly jumping in) is much easier than I previously thought.

I've added a bunch of packet sniffing features to the firmware. It will now scan all channels until it finds a remote transmitting. It won't be able to follow the entire conversation unless it has a matching device ID, so there's a command to set that. There's also one to enable/disable CRC on the radio, so it can listen before and after pairing.

When I get a chance, I should be able to set the ID from the conversation it's sniffing and automatically follow the pairing process and change settings accordingly. Another fun feature to add will be capturing an already started conversation and figure out what the ID is to properly set it.

#### Not So Current
We have control!!! I gave up waiting for the radio modules to arrive, so instead I removed the STM8 microcontroller from one of the remotes and connected an STM32F4 to it instead. Well, just the SPI lines that go to the radio... After a day of fighting with the half-duplex SPI code, I managed to get pairing working. Turns out the remote changes its own ID to match the quad's and enables ~~encryption~~ radio CRC after pairing. 

![Hijacked Remote](/images/IMG_2354.jpg)

I haven't had time to play with it much, but I managed to get a quick throttle ramp up/down working. You can watch the video [on youtube.](http://www.youtube.com/watch?v=5Op-pvrtTcU)

I wrote a quick app to send commands to the remote. Since I was in a rush, I re-purposed an app I used to test [my laser turret](http://alvarop.com/2014/11/laser-turret-project-mechanical/) over the summer, so it wasn't ideal. I controlled the throttle with a slider bar and pitch/roll with by holding and dragging the mouse across the window. You can watch a [video of that here](http://youtu.be/NUGXJX7mko0).

I spent some more time with the app to make it more useable. The throttle and yaw/rotation are controlled with the keyboard (standard WASD controls) and pitch/roll is still controlled by the mouse. While I was able to fly the quadcopter, I wouldn't recommend using a mouse and keyboard setup to do it. You can [see the app in action here](https://www.youtube.com/watch?v=BfARBlWldN0).

#### Old News
I am able to capture SPI data going between the STM8 microcontroller and the A7105 with my Saleae Logic analyzer. I export the SPI capture data to CSV and use a [python script](quadcsv.py) to process it into 'packets'. You can see an example processed capture [here](processeddata/connect2.processed.txt).

After what seemed an eternity, I was able to solder some wires to sniff the SPI data on the quadcopter itself. The microscope came in handy while trying to solder tiny wires on to pins on a QFN device. This allows me to capture both radio streams simultaneously and get a complete picture of what exactly goes on during pairing and operation. A processed packet capture with both the remote and quad data can be found [here](processeddata/dual-capture.decoded.txt).

I've added the [a7105.py](a7105.py) module which allows for plaintext decoding of the A7105 radio packets. This makes reading captures much simpler, since I no longer need to look up what each register means... You can see an example decoded capture [here](processeddata/connect2.decoded.txt).

So far, I have a decent understanding of the [startup](startup.md) sequence and some idea about how the remote and quadcopter [pair/sync](sync.md) together. I think I've figured out which bytes on the radio packet are used for the basic controls. See [control.md](control.md) for more info.

All radio packets (for both pairing and control) have an 8-bit checksum. It's a fairly simple computation. I used another [python script](checksum.py) to test and verify it.

Next step is to figure out the channel selection criteria and better understand the device pairing. I've ordered some radio modules to play with. Once those arrive, I should be able to build my own controller (or use my computer to do it.)

See the following files for much more detailed information on each topic:


### Files

See [startup.md](startup.md) for initial configuration and channel selection for both the remote and the quad

See [sync.md](sync.md) for notes on device sync/pairing protocol

See [control.md](control.md) for notes on radios, control packets, and control modes

See [notes.md](notes.md) for miscellaneous project notes and intermediate results.

See [hardware.md](hardware.md) for hardware notes (pinouts, etc...)

See [a7105.py](a7105.py) for python module that decodes A7105 SPI packets into readable text

See [checksum.py](checksum.py) for packet checksum computation and explanation.

See [quadcsv.py](quadcsv.py) for script to process Saleae Logic SPI CSV data into more manageable chunks

See [snifftest.py](snifftest.py) for python script that runs the sniffer and displays information about packets in the air. It can also enable the DOS and 'hijack' features.

See the [fw/controller](fw/controller) directory for the STM32F4 firmware I'm using to bridge between the radio and computer (via USB)

See (or don't, it's terrible) [sw/ProtoXRemote](sw/ProtoXRemote) for an OSX app that translates mouse and keyboard movement to controls for the quadcopter.