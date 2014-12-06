#include <stdio.h>
#include <string.h>
#include "a7105.h"

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
#define READ_BIT				(0x40)

#define MODE_FECF		(1 << 6)
#define MODE_CRCF		(1 << 5)
#define MODE_CER		(1 << 4)
#define MODE_XER		(1 << 3)
#define MODE_PLLER		(1 << 2)
#define MODE_TRSR		(1 << 1)
#define MODE_TRER		(1 << 0)

typedef struct {
	uint8_t addr;
	uint8_t val;
} regSetting_t;

static uint8_t wrBuff[256];
static uint8_t rdBuff[256];

static const regSetting_t initialSettings[] = {
	{MODE_CTL,					0x62},
	{CAL_CTL,					0x00},
	{FIFO_I,					0x0f},
	{FIFO_II,					0xc0},
	{RC_OSC_I,					0x00},
	{RC_OSC_II,					0x00},
	{RC_OSC_III,				0x04},
	{CKO_PIN_CTL,				0x00},
	{GIO1_PIN_CTL_I,			0x01},
	{GIO2_PIN_CTL_II,			0x01},
	{CLOCK,						0x05},
	{DATA_RATE,					0x04},
	{PLL_I,						0x50},
	{PLL_II,					0x9e},
	{PLL_III,					0x4b},
	{PLL_IV,					0x00},
	{PLL_V,						0x00},
	{TX_I,						0x16},
	{TX_II,						0x2b},
	{DELAY_I,					0x12},
	{DELAY_II,					0x00},
	{RX,						0x62},
	{RX_GAIN_I,					0x80},
	{RX_GAIN_II,				0x80},
	{RX_GAIN_III,				0x00},
	{RX_GAIN_IV,				0x0a},
	{RSSI_THRESHOLD,			0x32},
	{ADC_CTL,					0xc3},
	{CODE_I,					0x07},
	{CODE_II,					0x17},
	{CODE_III,					0x00},
	{IF_CAL_I,					0x00},
	{VCO_CURRENT_CAL,			0x00},
	{VCO_SINGLE_BAND_CAL_I,		0x00},
	{VCO_SINGLE_BAND_CAL_II,	0x3b},
	{BATTERY_DETECT,			0x00},
	{TX_TEST,					0x17},
	{RX_DEM_TEST_I,				0x47},
	{RX_DEM_TEST_II,			0x80},
	{CHARGE_PUMP_CURRENT,		0x03},
	{CRYSTAL_TEST,				0x01},
	{PLL_TEST,					0x45},
	{VCO_TEST_I,				0x18},
	{VCO_TEST_II,				0x00},
	{IFAT,						0x01},
	{RSCALE,					0x0f},
};

static int32_t a7105Read(uint8_t addr, uint8_t *buff, uint8_t len) {
	wrBuff[0] = addr;

	if(len > (sizeof(wrBuff) - 1)) {
		len = sizeof(wrBuff) - 1;
	}

	memset(&wrBuff[1], 0x00, len);

	// TODO - SPI RW wrBuff, rdBuff len+1

	// Not very efficient, but caller won't have to worry about address byte
	if(buff != NULL){
		memcpy(buff, &rdBuff[1], len);
	}

	return 0;
}

static int32_t a7105Write(uint8_t addr, uint8_t *buff, uint8_t len) {
	wrBuff[0] = addr;

	if(len > (sizeof(wrBuff) - 1)) {
		len = sizeof(wrBuff) - 1;
	}

	memcpy(&wrBuff[1], buff, len);

	// TODO - SPI RW wrBuff len+1

	return 0;
}

static int32_t a7105WriteReg(uint8_t reg, uint8_t val) {
	return a7105Write(reg, &val, 1);
}

static uint8_t a7105ReadReg(uint8_t reg) {
	uint8_t val;
	a7105Read(reg, &val, 1);

	return val;
}

static int32_t a7105Strobe(uint8_t strobe) {
	return a7105Write(&strobe, NULL, 0);
}

// TODO - randomly generate ID?
static uint8_t deviceID[] = {0x01, 0x23, 0x45, 0x67};

//
// Note: Taken calibration straight from captured SPI data from ProtoX remote
// Some of the things they do don't make any sense.
// For example: 
// - writing to the IF_CAL_II register, which is read only
// - Calibrating the VCO, but checking the IF filter bank calibration result...
//
// I'm leaving these possible bugs in place until I have a setup I can test with
//
static a7105Calibrate() {
	puts("Starting A7105 Calibration");
	a7105Strobe(STROBE_STANDBY);

	puts("IF Filter Bank Calibration");
	a7105WriteReg(CAL_CTL, 0x01); // Enable IF Filter Bank Calibration
	while (a7105ReadReg(CAL_CTL) & 0x01) {
	}

	if(a7105ReadReg(IF_CAL_I) & 0x10) {
		puts("IF Filter Bank Calibration Failed");
	} else {
		puts("IF Filter Bank Calibration Successful");
	}

	a7105WriteReg(IF_CAL_I, 0x13);	// Set manual calibration value (why??)
	a7105WriteReg(IF_CAL_II, 0x3b);	// This register is read only....

	puts("IF Filter Bank Calibration 1");
	a7105WriteReg(PLL_I, 0x0);
	a7105Strobe(STROBE_PLL);

	a7105WriteReg(CAL_CTL, 0x02); // Enable VCO Bank Calibration
	while (a7105ReadReg(CAL_CTL) & 0x02) {
	}

	// I think we should be checking VCO_SINGLE_BAND_CAL_I, not IF_CAL_I...
	if(a7105ReadReg(IF_CAL_I) & 0x10) {
		puts("VCO Bank Calibration Failed");
	} else {
		puts("VCO Bank Calibration Successful");
	}

	puts("IF Filter Bank Calibration 2");
	a7105WriteReg(PLL_I, 0x78);
	a7105Strobe(STROBE_PLL);	

	a7105WriteReg(CAL_CTL, 0x02); // Enable VCO Bank Calibration (again)
	while (a7105ReadReg(CAL_CTL) & 0x02) {
	}

	// I think we should be checking VCO_SINGLE_BAND_CAL_I, not IF_CAL_I...
	if(a7105ReadReg(IF_CAL_I) & 0x10) {
		puts("VCO Bank Calibration Failed");
	} else {
		puts("VCO Bank Calibration Successful");
	}

	a7105WriteReg(VCO_SINGLE_BAND_CAL_I, 0x0B); // Set manual VCO calibration value (why? again)

	a7105Strobe(STROBE_STANDBY);

	puts("Calibration complete");
}

// TODO - add rx callback here?
void a7105Init() {
	// TODO - setup SPI

	// Device Reset
	a7105WriteReg(MODE, 0x00);

	a7105Write(ID_DATA, deviceID, sizeof(deviceID));

	for(uint32_t reg = 0; reg < (sizeof(initialSettings)/sizeof(regSetting_t)); reg++) {
		a7105WriteReg(initialSettings[reg].addr, initialSettings[reg].val);
	}

	a7105Calibrate();

}
