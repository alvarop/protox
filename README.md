Trying to reverse engineer the Proto-X radio protocol to build my own controllers (and maybe mess with others...)

Using a python script to process Logic captured SPI data (from CSV) to more manageable chunks
Since the CS signal wasn't capture properly, relying on time between bytes to determine if byte is part of current or new 'packet'
This works *most* of the time, but we do get some split packets from time to time.
I also added duplicate packet detection so we don't print the same thing over and over

Input file looks like this:
1.752640320000000,,0x19,
1.752681920000000,,0x40,
1.752716640000000,,0x19,
1.752758320000000,,0x40,
1.752793040000000,,0x19,
1.752834640000000,,0x40,
1.752869360000000,,0x19,
1.752911040000000,,0x40,
1.752945760000000,,0x19,

Output looks like:
40 19  (Repeated 78 times)
40 
19 
40 19  (Repeated 25 times)
a0 
e0 05 01 82 96 d5 18 f6 00 00 00 00 00 00 00 00 00 04 d0 
40 1b  (Repeated 16 times)
40 
1b 
40 1b  (Repeated 8 times)
40 1a 
c0 
40 19  (Repeated 68 times)
