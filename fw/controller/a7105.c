#include <stdio.h>
#include "stm32f4xx_conf.h"
#include "stm32f4xx.h"
#include "a7105.h"

typedef struct {
	uint8_t addr;
	uint8_t val;
} regSetting_t;

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

int32_t a7105Read(uint8_t addr, uint8_t *buff, uint8_t len) {

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

int32_t a7105Write(uint8_t addr, uint8_t *buff, uint8_t len) {

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

int32_t a7105WriteReg(uint8_t reg, uint8_t val) {
	return a7105Write(reg, &val, 1);
}

uint8_t a7105ReadReg(uint8_t reg) {
	uint8_t val;
	a7105Read(reg, &val, 1);

	return val;
}

int32_t a7105Strobe(uint8_t strobe) {
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

void a7105SetChannel(uint8_t ch) {
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
void a7105Calibrate() {
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
}
