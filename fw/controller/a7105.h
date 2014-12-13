#ifndef __A7105_H__
#define __A7105_H__

#include <stdint.h>

// 
// Strobe definitions
// 

#define STROBE_SLEEP			(0x80)
#define STROBE_IDLE				(0x90)
#define STROBE_STANDBY			(0xA0)
#define STROBE_PLL				(0xB0)
#define STROBE_RX				(0xC0)
#define STROBE_TX				(0xD0)
#define STROBE_FIFO_WR_RST		(0xE0)
#define STROBE_FIFO_RD_RST		(0xF0)

//
// Register definitions
//
#define MODE					(0x00)
#define MODE_CTL				(0x01)
#define CAL_CTL					(0x02)
#define FIFO_I					(0x03)
#define FIFO_II					(0x04)
#define FIFO_DATA				(0x05)
#define ID_DATA					(0x06)
#define RC_OSC_I				(0x07)
#define RC_OSC_II				(0x08)
#define RC_OSC_III				(0x09)
#define CKO_PIN_CTL				(0x0A)
#define GIO1_PIN_CTL_I			(0x0B)
#define GIO2_PIN_CTL_II			(0x0C)
#define CLOCK					(0x0D)
#define DATA_RATE				(0x0E)
#define PLL_I					(0x0F)
#define PLL_II					(0x10)
#define PLL_III					(0x11)
#define PLL_IV					(0x12)
#define PLL_V					(0x13)
#define TX_I					(0x14)
#define TX_II					(0x15)
#define DELAY_I					(0x16)
#define DELAY_II				(0x17)
#define RX						(0x18)
#define RX_GAIN_I				(0x19)
#define RX_GAIN_II				(0x1A)
#define RX_GAIN_III				(0x1B)
#define RX_GAIN_IV				(0x1C)
#define RSSI_THRESHOLD			(0x1D)
#define ADC_CTL					(0x1E)
#define CODE_I					(0x1F)
#define CODE_II					(0x20)
#define CODE_III				(0x21)
#define IF_CAL_I				(0x22)
#define IF_CAL_II				(0x23)
#define VCO_CURRENT_CAL			(0x24)
#define VCO_SINGLE_BAND_CAL_I	(0x25)
#define VCO_SINGLE_BAND_CAL_II	(0x26)
#define BATTERY_DETECT			(0x27)
#define TX_TEST					(0x28)
#define RX_DEM_TEST_I			(0x29)
#define RX_DEM_TEST_II			(0x2A)
#define CHARGE_PUMP_CURRENT		(0x2B)
#define CRYSTAL_TEST			(0x2C)
#define PLL_TEST				(0x2D)
#define VCO_TEST_I				(0x2E)
#define VCO_TEST_II				(0x2F)
#define IFAT					(0x30)
#define RSCALE					(0x31)
#define FILTER_TEST				(0x32)

#define STROBE_BIT				(0x80)
#define RD_BIT					(0x40)

#define MODE_FECF		(1 << 6)
#define MODE_CRCF		(1 << 5)
#define MODE_CER		(1 << 4)
#define MODE_XER		(1 << 3)
#define MODE_PLLER		(1 << 2)
#define MODE_TRSR		(1 << 1)
#define MODE_TRER		(1 << 0)

void a7105Init();

int32_t a7105Read(uint8_t addr, uint8_t *buff, uint8_t len);
int32_t a7105Write(uint8_t addr, uint8_t *buff, uint8_t len);
int32_t a7105WriteReg(uint8_t reg, uint8_t val);
uint8_t a7105ReadReg(uint8_t reg);
int32_t a7105Strobe(uint8_t strobe);
void a7105SetChannel(uint8_t ch);
void a7105Calibrate();
uint8_t a7105GetBestChannel();

#endif
