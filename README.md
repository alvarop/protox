Trying to reverse engineer the Proto-X radio protocol to build my own controllers (and maybe mess with others...)

Using a python script to process Logic captured SPI data (from CSV) to more manageable chunks

See notes.md for project notes and intermediate results.
See checksum.py for packet checksum computation and explanation.
See sync.md for notes on device sync protocol

~~Since the CS signal wasn't captured properly, relying on time between bytes to determine if byte is part of current or new 'packet'~~

This works *most* of the time, but we do get some split packets from time to time.
I also added duplicate packet detection so we don't print the same thing over and over

Input file looks like this:
```
4.406020080000000,41055,0x40,
4.406054800000000,41055,0x19,
4.406096400000000,41056,0x40,
4.406135600000000,41056,0x19,
4.406178720000000,41057,0xA0,
4.406216720000000,41058,0xE0,
4.406253680000000,41059,0x05,
4.406290560000000,41059,0x01,
4.406327600000000,41059,0x46,
4.406364720000000,41059,0xE4,
4.406401760000000,41059,0x59,
4.406438880000000,41059,0x10,
4.406476000000000,41059,0x3F,
4.406513040000000,41059,0x00,
4.406550160000000,41059,0x00,
4.406587280000000,41059,0x00,
4.406624320000000,41059,0x00,
4.406661440000000,41059,0x00,
4.406698560000000,41059,0x00,
4.406735680000000,41059,0x00,
4.406772720000000,41059,0x00,
4.406809840000000,41059,0x00,
4.406847040000000,41059,0x2D,
4.406884320000000,41060,0xD0,
4.406921760000000,41061,0x40,
4.406956480000000,41061,0x1B,
4.406995680000000,41062,0x40,
4.407030400000000,41062,0x1B,
```

Output looks like:
```
40 19  (Repeated 148 times)
a0 
e0 
05 01 46 e4 59 10 3f 00 00 00 00 00 00 00 00 00 2d 
d0 
40 1b  (Repeated 26 times)
40 1a 
c0 
40 19  (Repeated 148 times)
```
