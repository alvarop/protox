#include <stdio.h>
#include <string.h>
#include "stm32f4xx_conf.h"
#include "stm32f4xx.h"
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
#define RD_BIT					(0x40)

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

typedef struct {
	uint8_t packetType;
	uint8_t unknown1;
	uint8_t throttle;
	uint8_t unknown2;
	uint8_t yaw;
	uint8_t unknown3;
	uint8_t pitch;
	uint8_t unknown4;
	uint8_t roll;
	uint8_t flips;
	uint8_t unknown5;
	uint8_t unknown6;
	uint8_t unknown7;
	uint8_t unknown8;
	uint8_t unknown9;
} __attribute__((packed)) controlPacket_t;

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

extern volatile uint32_t tickMs;

static void msDelay(uint32_t delay) {
	uint32_t finishTime = tickMs + delay;

	while(tickMs < finishTime) {
		__WFI();
	}
}

static uint8_t protoXChecksum(uint8_t *buff, uint8_t len) {
	uint8_t checksum = 0;

	for(uint8_t x = 0; x < len; x++) {
		checksum -= buff[x];
	}

	return checksum;
}

static int32_t a7105Read(uint8_t addr, uint8_t *buff, uint8_t len) {

	// TODO - SPI RW wrBuff, rdBuff len+1

	if(buff != NULL){
		uint8_t *rxPtr = buff;

		SPI_BiDirectionalLineConfig(SPI2, SPI_Direction_Tx);

		GPIO_ResetBits(GPIOB, (1 << 11));

		SPI2->DR = addr | RD_BIT;

		// TODO - use interrupts and __WFI here
		while(!(SPI2->SR & SPI_I2S_FLAG_TXE));

		while(SPI2->SR & SPI_I2S_FLAG_BSY);

		SPI_BiDirectionalLineConfig(SPI2, SPI_Direction_Rx);

		for(int32_t byte = 0; byte < len; byte++){
			
			SPI2->DR = 0x00; // Dummy write
			while(!(SPI2->SR & SPI_I2S_FLAG_TXE));
			while(SPI2->SR & SPI_I2S_FLAG_BSY);
			*rxPtr++ = SPI2->DR;
		}

		// TODO - figure out why it breaks if there's no delay
		for(uint32_t x = 0; x < 100; x++) {
			__asm("nop");
		}

		SPI_BiDirectionalLineConfig(SPI2, SPI_Direction_Tx);

		GPIO_SetBits(GPIOB, (1 << 11));

		for(uint32_t x = 0; x < 1000; x++) {
			__asm("nop");
		}


		return 0;
	} else {
		printf("%s - buff is NULL\n", __func__);
		return -1;
	}

	return 0;
}

static int32_t a7105Write(uint8_t addr, uint8_t *buff, uint8_t len) {

	if(buff != NULL) {
		SPI_BiDirectionalLineConfig(SPI2, SPI_Direction_Tx);

		GPIO_ResetBits(GPIOB, (1 << 11));

		SPI2->DR = addr;

		// TODO - use interrupts and __WFI here
		while(!(SPI2->SR & SPI_I2S_FLAG_TXE));
		*(volatile uint16_t *)&SPI2->DR;

		for(int32_t byte = 0; byte < len; byte++){
			
			SPI2->DR = buff[byte];
			while(!(SPI2->SR & SPI_I2S_FLAG_TXE));
			*(volatile uint16_t *)&SPI2->DR;
		}

		while(SPI2->SR & SPI_I2S_FLAG_BSY);

		// TODO - figure out why it breaks if there's no delay
		for(uint32_t x = 0; x < 100; x++) {
			__asm("nop");
		}

		GPIO_SetBits(GPIOB, (1 << 11));

		for(uint32_t x = 0; x < 1000; x++) {
			__asm("nop");
		}


		return 0;
	} else {
		printf("%s - buff is NULL\n", __func__);
		return -1;
	}
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
		SPI_BiDirectionalLineConfig(SPI2, SPI_Direction_Tx);

		GPIO_ResetBits(GPIOB, (1 << 11));

		SPI2->DR = strobe;

		// TODO - use interrupts and __WFI here
		while(!(SPI2->SR & SPI_I2S_FLAG_TXE));
		*(volatile uint16_t *)&SPI2->DR;

		while(SPI2->SR & SPI_I2S_FLAG_BSY);

		// TODO - figure out why it breaks if there's no delay
		for(uint32_t x = 0; x < 100; x++) {
			__asm("nop");
		}

		GPIO_SetBits(GPIOB, (1 << 11));

		for(uint32_t x = 0; x < 1000; x++) {
			__asm("nop");
		}


	return 0;
}

// TODO - randomly generate ID?
static uint8_t deviceID[] = {0x55, 0x20, 0x10, 0x41};

static void a7105SetChannel(uint8_t ch) {
	a7105WriteReg(PLL_I, ch);
	a7105Strobe(STROBE_PLL);
}

//
// Note: Taken calibration straight from captured SPI data from ProtoX remote
// Some of the things they do don't make any sense.
// For example: 
// - writing to the IF_CAL_II register, which is read only
// - Calibrating the VCO, but checking the IF filter bank calibration result...
//
// I'm leaving these possible bugs in place until I have a setup I can test with
//
static void a7105Calibrate() {
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
	a7105SetChannel(0);

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
	a7105SetChannel(0x78);

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

static const uint8_t channels[] = {0x14, 0x1e, 0x28, 0x32, 0x3c, 0x46, 0x50, 0x5a, 0x64, 0x6e, 0x78, 0x82};
static uint32_t rssiValues[sizeof(channels)];
#define RSSI_SAMPLES (15)

static uint8_t bestChannel = 0;

int32_t a7105SetBestChannel() {
	

	a7105SetChannel(0xA0);
	a7105Strobe(STROBE_RX);

	a7105WriteReg(ADC_CTL, 0xC3);
	for(uint8_t channel = 0; channel < sizeof(channels); channel++) {
		a7105SetChannel(channels[channel]);
		a7105Strobe(STROBE_RX);

		msDelay(5);

		rssiValues[channel] = 0;

		for(uint8_t sample = 0; sample < RSSI_SAMPLES; sample++) {
			rssiValues[channel] += a7105ReadReg(RSSI_THRESHOLD);
			msDelay(2);
		}

		printf("Ch 0x%02X - RSSI total: %ld\n", channels[channel], rssiValues[channel]);

		if(rssiValues[channel] <  rssiValues[bestChannel]) {
			bestChannel = channel;
		}
	}

	printf("Best channel is: %02X, setting\n", channels[bestChannel]);
	a7105SetChannel(channels[bestChannel]);

	msDelay(5);


	return 0;
}

// TODO - add rx callback here?
void a7105Init() {
	
	SPI_InitTypeDef spiConfig;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	//
	// GPIO Config
	//

	// PB11 - CS
	// PB13 - SCK
	// PB15 - MISO/MOSI (half-duplex mode)
	GPIO_Init(GPIOB, &(GPIO_InitTypeDef){GPIO_Pin_11, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_Speed_50MHz, GPIO_PuPd_NOPULL});
	GPIO_Init(GPIOB, &(GPIO_InitTypeDef){GPIO_Pin_13, GPIO_Mode_AF, GPIO_OType_PP, GPIO_Speed_50MHz, GPIO_PuPd_NOPULL});
	GPIO_Init(GPIOB, &(GPIO_InitTypeDef){GPIO_Pin_15, GPIO_Mode_AF, GPIO_OType_PP, GPIO_Speed_50MHz, GPIO_PuPd_NOPULL});

	GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_SPI2);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_SPI2);

	GPIO_SetBits(GPIOB, (1 << 11)); // Disable PB11, since it's active low

	//
	// SPI Config
	//
	SPI_StructInit(&spiConfig);

	spiConfig.SPI_Direction = SPI_Direction_1Line_Tx;
	spiConfig.SPI_Mode = SPI_Mode_Master;
	spiConfig.SPI_NSS = SPI_NSS_Soft;
	spiConfig.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128; // (APB2PCLK == 42MHz)/128 = 328125 Hz

	SPI_I2S_DeInit(SPI2);

	SPI_Init(SPI2, &spiConfig);

	SPI_Cmd(SPI2, ENABLE);

	// Device Reset
	a7105WriteReg(MODE, 0x00);

	for(uint32_t x = 0; x < 40000; x++) {
		__asm("nop");
	}

	a7105Write(ID_DATA, deviceID, sizeof(deviceID));

	for(uint32_t reg = 0; reg < (sizeof(initialSettings)/sizeof(regSetting_t)); reg++) {
		a7105WriteReg(initialSettings[reg].addr, initialSettings[reg].val);
		for(uint32_t x = 0; x < 200; x++) {
			__asm("nop");
		}
	}

	a7105Calibrate();

	uint8_t someID[4] = {0, 0, 0, 0};
	a7105Read(ID_DATA, someID, sizeof(someID));

	printf("Read ID: %02X %02X %02X %02X\n", someID[0], someID[1], someID[2], someID[3]);

	a7105SetBestChannel();

}

uint8_t packetBuff[256];
void protoXSendPacket(uint8_t *buff, uint8_t len) {
	memcpy(packetBuff, buff, len);
	packetBuff[len] = protoXChecksum(packetBuff, len);

	a7105Strobe(STROBE_STANDBY);
	a7105Strobe(STROBE_FIFO_WR_RST);
	a7105Write(FIFO_DATA, packetBuff, len + 1);
	a7105Strobe(STROBE_TX);

	while (a7105ReadReg(MODE) & MODE_TRER);
}

uint8_t magicalPacket[] = {0x01, 0x82, 0xC0, 0xAC, 0xD8, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

controlPacket_t controlPacket = {0x20, 0x00, 0x00, 0x00, 0x80, 0x00, 0x6C, 0x00, 0x80, 0x06, 0x19, 0x00, 0x00, 0x00, 0x00};

typedef enum {COUNT1, COUNT2, COUNT3} connectState_t;
static connectState_t state = COUNT1;

int32_t processPacket(uint8_t *packet) {
	int32_t rval = 0;

	if(protoXChecksum(packet, 16) == 0) {
		switch(state) {
			case COUNT1: {
				if(packet[0] == 4) {
					magicalPacket[0] = 1;

					// Set the new ID
					a7105Write(ID_DATA, &magicalPacket[2], 4);

					state = COUNT2;
				} else {
					magicalPacket[0] = packet[0] + 1;
				}
				break;
			}

			case COUNT2: {
				magicalPacket[0] = 0x9;
				magicalPacket[2] = 0x00;
				state = COUNT3;
				break;
			}

			case COUNT3: {
				if(packet[1] == 9) {
					magicalPacket[0] = 1;	
					state = COUNT1;

					// Start encrypting data?
					a7105WriteReg(CODE_I, 0x0F);
					rval = 1;
				} else {
					magicalPacket[2] = packet[1];
				}
				break;
			}
		}
		
	} else {
		printf("Bad packet! [");
		for(uint8_t x = 0; x < 16; x++) {
			printf("%02X ", packet[x]);
		}
		printf("]\n");
		rval = 1;
	}

	return rval;
}

void protoXRemote() {
	uint32_t timeout;
	uint32_t timeToCHSwitch;
	uint32_t channelRefresh = 0;
	uint32_t done = 0;
	printf("Starting remote\n");
	do {
		protoXSendPacket(magicalPacket, 15);

		a7105Strobe(STROBE_RX);

		timeout = tickMs + 14;

		while ((a7105ReadReg(MODE) & MODE_TRER) && (timeout > tickMs));

		// Heard it!
		if (timeout > tickMs) {
			a7105Strobe(STROBE_FIFO_RD_RST);
			a7105Read(FIFO_DATA, packetBuff, 16);
			
			done = processPacket(packetBuff);
		}

		for(uint32_t x = 0; x < 200; x++) {
			__asm("nop");
		}
	} while(!done);

	timeToCHSwitch = tickMs + 5;

	puts("idle");
	for(uint32_t vroom = 0; vroom < 64; vroom++){
	
		if(channelRefresh) {
			channelRefresh = 0;
			a7105SetChannel(channels[bestChannel]);
			timeToCHSwitch = tickMs + 5;
		} else if(tickMs > timeToCHSwitch) {
			a7105SetChannel(0xA5);
			channelRefresh = 1;
		}

		timeout = tickMs + 10;
		controlPacket.throttle = 0;
		protoXSendPacket((uint8_t *)&controlPacket, 15);

		while(timeout > tickMs);
	}

	puts("rampup");
	for(uint32_t vroom = 0; vroom < 64; vroom++){
		
		if(channelRefresh) {
			channelRefresh = 0;
			a7105SetChannel(channels[bestChannel]);
			timeToCHSwitch = tickMs + 5;
		} else if(tickMs > timeToCHSwitch) {
			a7105SetChannel(0xA5);
			channelRefresh = 1;
		}

		timeout = tickMs + 10;
		controlPacket.throttle = vroom * 2;
		protoXSendPacket((uint8_t *)&controlPacket, 15);

		while(timeout > tickMs);
	}

	puts("rampdown");

	for(uint32_t vroom = 64; vroom > 0; vroom--){
		
		if(channelRefresh) {
			channelRefresh = 0;
			a7105SetChannel(channels[bestChannel]);
			timeToCHSwitch = tickMs + 5;
		} else if(tickMs > timeToCHSwitch) {
			a7105SetChannel(0xA5);
			channelRefresh = 1;
		}

		timeout = tickMs + 10;
		controlPacket.throttle = vroom * 2;
		protoXSendPacket((uint8_t *)&controlPacket, 15);

		while(timeout > tickMs);
	}

	puts("done");

		
}
