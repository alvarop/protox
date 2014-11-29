# Hardware Notes

## Remote

### STM8S003F3P6 Pinout

I haven't checked most of the pins. Here are the ones I've hooked up to the logic analyzer so far. Will update as I find out more

Pin | Function
--- | --------
 1 | 
 2 | 
 3 | 
 4 | 
 5 | 
 6 | NRST (from datasheet)
 7 | VSS (from datasheet)
 8 | 
 9 | VDD (from datasheet)
 10 | 
 11 | 
 12 | Not sure what this is for. When I press the forward/back trim buttons, a ~74kHz square wave shows up here.
 13 | MOSI/MISO - They seem to use a bidirectional pin for SPI data
 14 | 
 15 | SCK
 16 | SPI CS/SS
 17 | Not sure what this is for either - It starts high and goes low once a device is connected. Could be green LED control perhaps?
 18 | 
 19 | 
 20 | 

## Quadcopter
The data from this [forum post](http://www.rcgroups.com/forums/showpost.php?p=26871198&postcount=1269) seems to match what I saw on my ProtoX.

### Invensense MPU-3050

Seems to be a 3-axis MEMS gyro.

Datasheet - http://www.invensense.com/mems/gyro/documents/PS-MPU-3000A.pdf

### MC3210AK

Markings I could see were 3210AK, but I think it's a MC3210 accelerometer from mCube

Datasheet - http://www.mcubemems.com/wp-content/uploads/2014/10/MC3210-Datasheet-APS-048-0005v1.8.pdf

### Hubsan HBS001
This seems to be the main flight computer. [Hubsan](http://hubsan.com) seems to make the quad, so it's probably their own processor. The [forum post](http://www.rcgroups.com/forums/showpost.php?p=26871198&postcount=1269) I mentioned earlier suggests this is an ARM Cortex-M processor. They mention something similar to the Mini54Z, which is a Cortex-M0.

The [Mini54Z](http://www.nuvoton.com/hq/products/microcontrollers/arm-cortex-m0-mcus/mini51-base-series/?__locale=en) is from [Nuvoton](http://www.nuvoton.com), which also offers foundry services. It wouldn't surprise me if they just re-branded one of their Mini54's as HBS001...
