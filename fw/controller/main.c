#include <stdio.h>
#include <stdint.h>

#include "stm32f4xx_conf.h"
#include "stm32f4xx.h"

#include "usbd_core.h"
#include "usbd_usr.h"
#include "usbd_desc.h"
#include "usbd_cdc_vcp.h"

#include "console.h"

#define BLINK_DELAY_MS	(500)

volatile uint32_t tickMs = 0;
__ALIGN_BEGIN USB_OTG_CORE_HANDLE  USB_OTG_dev __ALIGN_END;

// Private function prototypes
void Delay(volatile uint32_t nCount);
void init();

int main(void) {
	uint32_t nextBlink;
	uint32_t blinkState = 0;
	init();

	// Disable line buffering on stdout
	setbuf(stdout, NULL);

	nextBlink = tickMs + BLINK_DELAY_MS;
	for(;;) {

		consoleProcess();

		if(tickMs > nextBlink) {
			nextBlink = tickMs + BLINK_DELAY_MS;
			if(blinkState) {
				GPIO_SetBits(GPIOD, GPIO_Pin_12);
			} else {
				GPIO_ResetBits(GPIOD, GPIO_Pin_12);
			}
			blinkState ^= 1;
		}

		__WFI();
		
	}

	return 0;
}

void init() {

	// ---------- SysTick timer -------- //
	if (SysTick_Config(SystemCoreClock / 1000)) {
		// Capture error
		while (1){};
	}

	// ---------- GPIO -------- //
	// GPIOD Periph clock enable
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	GPIO_Init(GPIOD, &(GPIO_InitTypeDef){GPIO_Pin_12, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_Speed_2MHz, GPIO_PuPd_NOPULL});

	USBD_Init(&USB_OTG_dev,
				USB_OTG_FS_CORE_ID,
				&USR_desc,
				&USBD_CDC_cb,
				&USR_cb);
}

void SysTick_Handler(void)
{
	tickMs++;
}
