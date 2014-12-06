/******************************************************************************
 * @file:
 * @purpose: CMSIS Cortex-M3 Core Device Startup File
 * @version: V1.01
 * @date:	4. Feb. 2009
 *----------------------------------------------------------------------------
 *
 * Copyright (C) 2009 ARM Limited. All rights reserved.
 *
 * ARM Limited (ARM) is supplying this software for use with Cortex-Mx
 * processor based microcontrollers.	This file can be freely distributed
 * within development tools that are supporting such ARM based processors.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS".	NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * ARM SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 ******************************************************************************/

//
// Modified for stm32f4xx
//

//*****************************************************************************
//
// Forward declaration of the default fault handlers.
//
//*****************************************************************************
/* System exception vector handler */
void __attribute__ ((weak)) Reset_Handler(void);			/* Reset Handler */
void __attribute__ ((weak)) NMI_Handler(void);				/* NMI Handler */
void __attribute__ ((weak)) HardFault_Handler(void);		/* Hard Fault Handler */
void __attribute__ ((weak)) MemManage_Handler(void);		/* MPU Fault Handler */
void __attribute__ ((weak)) BusFault_Handler(void);			/* Bus Fault Handler */
void __attribute__ ((weak)) UsageFault_Handler(void);		/* Usage Fault Handler */
void __attribute__ ((weak)) SVC_Handler(void);				/* SVCall Handler */
void __attribute__ ((weak)) DebugMon_Handler(void);			/* Debug Monitor Handler */
void __attribute__ ((weak)) PendSV_Handler(void);			/* PendSV Handler */
void __attribute__ ((weak)) SysTick_Handler(void);			/* SysTick Handler */

/* External interrupt vector handler */
void __attribute__ ((weak)) WWDG_IRQHandler(void);
void __attribute__ ((weak)) PVD_IRQHandler(void);
void __attribute__ ((weak)) TAMP_STAMP_IRQHandler(void);
void __attribute__ ((weak)) RTC_WKUP_IRQHandler(void);
void __attribute__ ((weak)) FLASH_IRQHandler(void);
void __attribute__ ((weak)) RCC_IRQHandler(void);
void __attribute__ ((weak)) EXTI0_IRQHandler(void);
void __attribute__ ((weak)) EXTI1_IRQHandler(void);
void __attribute__ ((weak)) EXTI2_IRQHandler(void);
void __attribute__ ((weak)) EXTI3_IRQHandler(void);
void __attribute__ ((weak)) EXTI4_IRQHandler(void);
void __attribute__ ((weak)) DMA1_Stream0_IRQHandler(void);
void __attribute__ ((weak)) DMA1_Stream1_IRQHandler(void);
void __attribute__ ((weak)) DMA1_Stream2_IRQHandler(void);
void __attribute__ ((weak)) DMA1_Stream3_IRQHandler(void);
void __attribute__ ((weak)) DMA1_Stream4_IRQHandler(void);
void __attribute__ ((weak)) DMA1_Stream5_IRQHandler(void);
void __attribute__ ((weak)) DMA1_Stream6_IRQHandler(void);
void __attribute__ ((weak)) ADC_IRQHandler(void);
void __attribute__ ((weak)) CAN1_TX_IRQHandler(void);
void __attribute__ ((weak)) CAN1_RX0_IRQHandler(void);
void __attribute__ ((weak)) CAN1_RX1_IRQHandler(void);
void __attribute__ ((weak)) CAN1_SCE_IRQHandler(void);
void __attribute__ ((weak)) EXTI9_5_IRQHandler(void);
void __attribute__ ((weak)) TIM1_BRK_TIM9_IRQHandler(void);
void __attribute__ ((weak)) TIM1_UP_TIM10_IRQHandler(void);
void __attribute__ ((weak)) TIM1_TRG_COM_TIM11_IRQHandler(void);
void __attribute__ ((weak)) TIM1_CC_IRQHandler(void);
void __attribute__ ((weak)) TIM2_IRQHandler(void);
void __attribute__ ((weak)) TIM3_IRQHandler(void);
void __attribute__ ((weak)) TIM4_IRQHandler(void);
void __attribute__ ((weak)) I2C1_EV_IRQHandler(void);
void __attribute__ ((weak)) I2C1_ER_IRQHandler(void);
void __attribute__ ((weak)) I2C2_EV_IRQHandler(void);
void __attribute__ ((weak)) I2C2_ER_IRQHandler(void);
void __attribute__ ((weak)) SPI1_IRQHandler(void);
void __attribute__ ((weak)) SPI2_IRQHandler(void);
void __attribute__ ((weak)) USART1_IRQHandler(void);
void __attribute__ ((weak)) USART2_IRQHandler(void);
void __attribute__ ((weak)) USART3_IRQHandler(void);
void __attribute__ ((weak)) EXTI15_10_IRQHandler(void);
void __attribute__ ((weak)) RTC_Alarm_IRQHandler(void);
void __attribute__ ((weak)) OTG_FS_WKUP_IRQHandler(void);
void __attribute__ ((weak)) TIM8_BRK_TIM12_IRQHandler(void);
void __attribute__ ((weak)) TIM8_UP_TIM13_IRQHandler(void);
void __attribute__ ((weak)) TIM8_TRG_COM_TIM14_IRQHandler(void);
void __attribute__ ((weak)) TIM8_CC_IRQHandler(void);
void __attribute__ ((weak)) DMA1_Stream7_IRQHandler(void);
void __attribute__ ((weak)) FSMC_IRQHandler(void);
void __attribute__ ((weak)) SDIO_IRQHandler(void);
void __attribute__ ((weak)) TIM5_IRQHandler(void);
void __attribute__ ((weak)) SPI3_IRQHandler(void);
void __attribute__ ((weak)) UART4_IRQHandler(void);
void __attribute__ ((weak)) UART5_IRQHandler(void);
void __attribute__ ((weak)) TIM6_DAC_IRQHandler(void);
void __attribute__ ((weak)) TIM7_IRQHandler(void);
void __attribute__ ((weak)) DMA2_Stream0_IRQHandler(void);
void __attribute__ ((weak)) DMA2_Stream1_IRQHandler(void);
void __attribute__ ((weak)) DMA2_Stream2_IRQHandler(void);
void __attribute__ ((weak)) DMA2_Stream3_IRQHandler(void);
void __attribute__ ((weak)) DMA2_Stream4_IRQHandler(void);
void __attribute__ ((weak)) ETH_IRQHandler(void);
void __attribute__ ((weak)) ETH_WKUP_IRQHandler(void);
void __attribute__ ((weak)) CAN2_TX_IRQHandler(void);
void __attribute__ ((weak)) CAN2_RX0_IRQHandler(void);
void __attribute__ ((weak)) CAN2_RX1_IRQHandler(void);
void __attribute__ ((weak)) CAN2_SCE_IRQHandler(void);
void __attribute__ ((weak)) OTG_FS_IRQHandler(void);
void __attribute__ ((weak)) DMA2_Stream5_IRQHandler(void);
void __attribute__ ((weak)) DMA2_Stream6_IRQHandler(void);
void __attribute__ ((weak)) DMA2_Stream7_IRQHandler(void);
void __attribute__ ((weak)) USART6_IRQHandler(void);
void __attribute__ ((weak)) I2C3_EV_IRQHandler(void);
void __attribute__ ((weak)) I2C3_ER_IRQHandler(void);
void __attribute__ ((weak)) OTG_HS_EP1_OUT_IRQHandler(void);
void __attribute__ ((weak)) OTG_HS_EP1_IN_IRQHandler(void);
void __attribute__ ((weak)) OTG_HS_WKUP_IRQHandler(void);
void __attribute__ ((weak)) OTG_HS_IRQHandler(void);
void __attribute__ ((weak)) DCMI_IRQHandler(void);
void __attribute__ ((weak)) CRYP_IRQHandler(void);
void __attribute__ ((weak)) HASH_RNG_IRQHandler(void);
void __attribute__ ((weak)) FPU_IRQHandler(void);

/* Exported types --------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
extern unsigned long __etext;
extern unsigned long __sidata;			/* start address for the initialization values of the .data section. defined in linker script */
extern unsigned long __data_start__;	/* start address for the .data section. defined in linker script */
extern unsigned long __data_end__;		/* end address for the .data section. defined in linker script */

extern unsigned long __bss_start__;		/* start address for the .bss section. defined in linker script */
extern unsigned long __bss_end__;		/* end address for the .bss section. defined in linker script */

extern unsigned long *_estack;				/* init value for the stack pointer. defined in linker script */

extern void SystemInit(void);

/* Private typedef -----------------------------------------------------------*/
/* function prototypes ------------------------------------------------------*/
void Reset_Handler(void) __attribute__((__interrupt__));
extern int main(void);
extern void _CPUregTestPOST (void);

/******************************************************************************
*
* The minimal vector table for a Cortex M3.	Note that the proper constructs
* must be placed on this to ensure that it ends up at physical address
* 0x0000.0000.
*
******************************************************************************/

__attribute__ ((section(".isr_vector")))
void (* const g_pfnVectors[])(void) =
{
		(void *)&_estack,			 		/* The initial stack pointer */
		Reset_Handler,						/* Reset Handler */
		NMI_Handler,						/* NMI Handler */
		HardFault_Handler,					/* Hard Fault Handler */
		MemManage_Handler,					/* MPU Fault Handler */
		BusFault_Handler,					/* Bus Fault Handler */
		UsageFault_Handler,					/* Usage Fault Handler */
		0,									/* Reserved */
		0,									/* Reserved */
		0,									/* Reserved */
		0,									/* Reserved */
		SVC_Handler,						/* SVCall Handler */
		DebugMon_Handler,					/* Debug Monitor Handler */
		0,									/* Reserved */
		PendSV_Handler,						/* PendSV Handler */
		SysTick_Handler,					/* SysTick Handler */

		// External Interrupts
		WWDG_IRQHandler,                   /* Window WatchDog              */
		PVD_IRQHandler,                    /* PVD through EXTI Line detection */
		TAMP_STAMP_IRQHandler,             /* Tamper and TimeStamps through the EXTI line */
		RTC_WKUP_IRQHandler,               /* RTC Wakeup through the EXTI line */
		FLASH_IRQHandler,                  /* FLASH                        */
		RCC_IRQHandler,                    /* RCC                          */
		EXTI0_IRQHandler,                  /* EXTI Line0                   */
		EXTI1_IRQHandler,                  /* EXTI Line1                   */
		EXTI2_IRQHandler,                  /* EXTI Line2                   */
		EXTI3_IRQHandler,                  /* EXTI Line3                   */
		EXTI4_IRQHandler,                  /* EXTI Line4                   */
		DMA1_Stream0_IRQHandler,           /* DMA1 Stream 0                */
		DMA1_Stream1_IRQHandler,           /* DMA1 Stream 1                */
		DMA1_Stream2_IRQHandler,           /* DMA1 Stream 2                */
		DMA1_Stream3_IRQHandler,           /* DMA1 Stream 3                */
		DMA1_Stream4_IRQHandler,           /* DMA1 Stream 4                */
		DMA1_Stream5_IRQHandler,           /* DMA1 Stream 5                */
		DMA1_Stream6_IRQHandler,           /* DMA1 Stream 6                */
		ADC_IRQHandler,                    /* ADC1, ADC2 and ADC3s         */
		CAN1_TX_IRQHandler,                /* CAN1 TX                      */
		CAN1_RX0_IRQHandler,               /* CAN1 RX0                     */
		CAN1_RX1_IRQHandler,               /* CAN1 RX1                     */
		CAN1_SCE_IRQHandler,               /* CAN1 SCE                     */
		EXTI9_5_IRQHandler,                /* External Line[9:5]s          */
		TIM1_BRK_TIM9_IRQHandler,          /* TIM1 Break and TIM9          */
		TIM1_UP_TIM10_IRQHandler,          /* TIM1 Update and TIM10        */
		TIM1_TRG_COM_TIM11_IRQHandler,     /* TIM1 Trigger and Commutation and TIM11 */
		TIM1_CC_IRQHandler,                /* TIM1 Capture Compare         */
		TIM2_IRQHandler,                   /* TIM2                         */
		TIM3_IRQHandler,                   /* TIM3                         */
		TIM4_IRQHandler,                   /* TIM4                         */
		I2C1_EV_IRQHandler,                /* I2C1 Event                   */
		I2C1_ER_IRQHandler,                /* I2C1 Error                   */
		I2C2_EV_IRQHandler,                /* I2C2 Event                   */
		I2C2_ER_IRQHandler,                /* I2C2 Error                   */
		SPI1_IRQHandler,                   /* SPI1                         */
		SPI2_IRQHandler,                   /* SPI2                         */
		USART1_IRQHandler,                 /* USART1                       */
		USART2_IRQHandler,                 /* USART2                       */
		USART3_IRQHandler,                 /* USART3                       */
		EXTI15_10_IRQHandler,              /* External Line[15:10]s        */
		RTC_Alarm_IRQHandler,              /* RTC Alarm (A and B) through EXTI Line */
		OTG_FS_WKUP_IRQHandler,            /* USB OTG FS Wakeup through EXTI line */
		TIM8_BRK_TIM12_IRQHandler,         /* TIM8 Break and TIM12         */
		TIM8_UP_TIM13_IRQHandler,          /* TIM8 Update and TIM13        */
		TIM8_TRG_COM_TIM14_IRQHandler,     /* TIM8 Trigger and Commutation and TIM14 */
		TIM8_CC_IRQHandler,                /* TIM8 Capture Compare         */
		DMA1_Stream7_IRQHandler,           /* DMA1 Stream7                 */
		FSMC_IRQHandler,                   /* FSMC                         */
		SDIO_IRQHandler,                   /* SDIO                         */
		TIM5_IRQHandler,                   /* TIM5                         */
		SPI3_IRQHandler,                   /* SPI3                         */
		UART4_IRQHandler,                  /* UART4                        */
		UART5_IRQHandler,                  /* UART5                        */
		TIM6_DAC_IRQHandler,               /* TIM6 and DAC1&2 underrun errors */
		TIM7_IRQHandler,                   /* TIM7                         */
		DMA2_Stream0_IRQHandler,           /* DMA2 Stream 0                */
		DMA2_Stream1_IRQHandler,           /* DMA2 Stream 1                */
		DMA2_Stream2_IRQHandler,           /* DMA2 Stream 2                */
		DMA2_Stream3_IRQHandler,           /* DMA2 Stream 3                */
		DMA2_Stream4_IRQHandler,           /* DMA2 Stream 4                */
		ETH_IRQHandler,                    /* Ethernet                     */
		ETH_WKUP_IRQHandler,               /* Ethernet Wakeup through EXTI line */
		CAN2_TX_IRQHandler,                /* CAN2 TX                      */
		CAN2_RX0_IRQHandler,               /* CAN2 RX0                     */
		CAN2_RX1_IRQHandler,               /* CAN2 RX1                     */
		CAN2_SCE_IRQHandler,               /* CAN2 SCE                     */
		OTG_FS_IRQHandler,                 /* USB OTG FS                   */
		DMA2_Stream5_IRQHandler,           /* DMA2 Stream 5                */
		DMA2_Stream6_IRQHandler,           /* DMA2 Stream 6                */
		DMA2_Stream7_IRQHandler,           /* DMA2 Stream 7                */
		USART6_IRQHandler,                 /* USART6                       */
		I2C3_EV_IRQHandler,                /* I2C3 event                   */
		I2C3_ER_IRQHandler,                /* I2C3 error                   */
		OTG_HS_EP1_OUT_IRQHandler,         /* USB OTG HS End Point 1 Out   */
		OTG_HS_EP1_IN_IRQHandler,          /* USB OTG HS End Point 1 In    */
		OTG_HS_WKUP_IRQHandler,            /* USB OTG HS Wakeup through EXTI */
		OTG_HS_IRQHandler,                 /* USB OTG HS                   */
		DCMI_IRQHandler,                   /* DCMI                         */
		CRYP_IRQHandler,                   /* CRYP crypto                  */
		HASH_RNG_IRQHandler,               /* Hash and Rng                 */
		FPU_IRQHandler,                    /* FPU                          */
};

/*******************************************************************************
* Function Name	: Reset_Handler
* Description	: This is the code that gets called when the processor first starts execution
*				following a reset event.	Only the absolutely necessary set is performed,
*				after which the application supplied main() routine is called.
* Input			:
* Output		:
* Return		:
*******************************************************************************/
void Reset_Handler(void)
{
	unsigned long *pulSrc, *pulDest;

	/*
	* This used for cleaning AHBRAM0 section
	*/
#if 0
	for (pulDest = ((unsigned long *)AHBRAM0_BASE); \
					pulDest < ((unsigned long *)(AHBRAM0_BASE + AHBRAM0_SIZE)); \
					pulDest++){
		*(pulDest++) = 0;
	}
#endif

	/*
	* This used for cleaning AHBRAM1 section
	*/
#if 0
	for (pulDest = ((unsigned long *)AHBRAM1_BASE); \
					pulDest < ((unsigned long *)(AHBRAM1_BASE + AHBRAM1_SIZE)); \
					pulDest++){
		*(pulDest++) = 0;
	}
#endif

	//
	// Copy the data segment initializers from flash to SRAM in ROM mode
	//
#if (__RAM_MODE__==0)
	pulSrc = &__sidata;
	for(pulDest = &__data_start__; pulDest < &__data_end__; )
	{
		*(pulDest++) = *(pulSrc++);
	}
#endif


	//
	// Zero fill the bss segment.
	//
	for(pulDest = &__bss_start__; pulDest < &__bss_end__; )
	{
		*(pulDest++) = 0;
	}

	//
	//	Call IEC60335 CPU register tests POST
	//
//	__ASM volatile ("bl _CPUregTestPOST \t\n");

	SystemInit();

	//
	// Call the application's entry point.
	//
	main();
}

//*****************************************************************************
//
// Provide weak aliases for each Exception handler to the Default_Handler.
// As they are weak aliases, any function with the same name will override
// this definition.
//
//*****************************************************************************
#pragma weak MemManage_Handler = Default_Handler			/* MPU Fault Handler */
#pragma weak BusFault_Handler = Default_Handler			/* Bus Fault Handler */
#pragma weak UsageFault_Handler = Default_Handler		/* Usage Fault Handler */
#pragma weak SVC_Handler = Default_Handler				/* SVCall Handler */
#pragma weak DebugMon_Handler = Default_Handler			/* Debug Monitor Handler */
#pragma weak PendSV_Handler = Default_Handler			/* PendSV Handler */
#pragma weak SysTick_Handler = Default_Handler			/* SysTick Handler */

/* External interrupt vector handler */
#pragma weak WWDG_IRQHandler = Default_Handler
#pragma weak PVD_IRQHandler = Default_Handler
#pragma weak TAMP_STAMP_IRQHandler = Default_Handler
#pragma weak RTC_WKUP_IRQHandler = Default_Handler
#pragma weak FLASH_IRQHandler = Default_Handler
#pragma weak RCC_IRQHandler = Default_Handler
#pragma weak EXTI0_IRQHandler = Default_Handler
#pragma weak EXTI1_IRQHandler = Default_Handler
#pragma weak EXTI2_IRQHandler = Default_Handler
#pragma weak EXTI3_IRQHandler = Default_Handler
#pragma weak EXTI4_IRQHandler = Default_Handler
#pragma weak DMA1_Stream0_IRQHandler = Default_Handler
#pragma weak DMA1_Stream1_IRQHandler = Default_Handler
#pragma weak DMA1_Stream2_IRQHandler = Default_Handler
#pragma weak DMA1_Stream3_IRQHandler = Default_Handler
#pragma weak DMA1_Stream4_IRQHandler = Default_Handler
#pragma weak DMA1_Stream5_IRQHandler = Default_Handler
#pragma weak DMA1_Stream6_IRQHandler = Default_Handler
#pragma weak ADC_IRQHandler = Default_Handler
#pragma weak CAN1_TX_IRQHandler = Default_Handler
#pragma weak CAN1_RX0_IRQHandler = Default_Handler
#pragma weak CAN1_RX1_IRQHandler = Default_Handler
#pragma weak CAN1_SCE_IRQHandler = Default_Handler
#pragma weak EXTI9_5_IRQHandler = Default_Handler
#pragma weak TIM1_BRK_TIM9_IRQHandler = Default_Handler
#pragma weak TIM1_UP_TIM10_IRQHandler = Default_Handler
#pragma weak TIM1_TRG_COM_TIM11_IRQHandler = Default_Handler
#pragma weak TIM1_CC_IRQHandler = Default_Handler
#pragma weak TIM2_IRQHandler = Default_Handler
#pragma weak TIM3_IRQHandler = Default_Handler
#pragma weak TIM4_IRQHandler = Default_Handler
#pragma weak I2C1_EV_IRQHandler = Default_Handler
#pragma weak I2C1_ER_IRQHandler = Default_Handler
#pragma weak I2C2_EV_IRQHandler = Default_Handler
#pragma weak I2C2_ER_IRQHandler = Default_Handler
#pragma weak SPI1_IRQHandler = Default_Handler
#pragma weak SPI2_IRQHandler = Default_Handler
#pragma weak USART1_IRQHandler = Default_Handler
#pragma weak USART2_IRQHandler = Default_Handler
#pragma weak USART3_IRQHandler = Default_Handler
#pragma weak EXTI15_10_IRQHandler = Default_Handler
#pragma weak RTC_Alarm_IRQHandler = Default_Handler
#pragma weak OTG_FS_WKUP_IRQHandler = Default_Handler
#pragma weak TIM8_BRK_TIM12_IRQHandler = Default_Handler
#pragma weak TIM8_UP_TIM13_IRQHandler = Default_Handler
#pragma weak TIM8_TRG_COM_TIM14_IRQHandler = Default_Handler
#pragma weak TIM8_CC_IRQHandler = Default_Handler
#pragma weak DMA1_Stream7_IRQHandler = Default_Handler
#pragma weak FSMC_IRQHandler = Default_Handler
#pragma weak SDIO_IRQHandler = Default_Handler
#pragma weak TIM5_IRQHandler = Default_Handler
#pragma weak SPI3_IRQHandler = Default_Handler
#pragma weak UART4_IRQHandler = Default_Handler
#pragma weak UART5_IRQHandler = Default_Handler
#pragma weak TIM6_DAC_IRQHandler = Default_Handler
#pragma weak TIM7_IRQHandler = Default_Handler
#pragma weak DMA2_Stream0_IRQHandler = Default_Handler
#pragma weak DMA2_Stream1_IRQHandler = Default_Handler
#pragma weak DMA2_Stream2_IRQHandler = Default_Handler
#pragma weak DMA2_Stream3_IRQHandler = Default_Handler
#pragma weak DMA2_Stream4_IRQHandler = Default_Handler
#pragma weak ETH_IRQHandler = Default_Handler
#pragma weak ETH_WKUP_IRQHandler = Default_Handler
#pragma weak CAN2_TX_IRQHandler = Default_Handler
#pragma weak CAN2_RX0_IRQHandler = Default_Handler
#pragma weak CAN2_RX1_IRQHandler = Default_Handler
#pragma weak CAN2_SCE_IRQHandler = Default_Handler
#pragma weak OTG_FS_IRQHandler = Default_Handler
#pragma weak DMA2_Stream5_IRQHandler = Default_Handler
#pragma weak DMA2_Stream6_IRQHandler = Default_Handler
#pragma weak DMA2_Stream7_IRQHandler = Default_Handler
#pragma weak USART6_IRQHandler = Default_Handler
#pragma weak I2C3_EV_IRQHandler = Default_Handler
#pragma weak I2C3_ER_IRQHandler = Default_Handler
#pragma weak OTG_HS_EP1_OUT_IRQHandler = Default_Handler
#pragma weak OTG_HS_EP1_IN_IRQHandler = Default_Handler
#pragma weak OTG_HS_WKUP_IRQHandler = Default_Handler
#pragma weak OTG_HS_IRQHandler = Default_Handler
#pragma weak DCMI_IRQHandler = Default_Handler
#pragma weak CRYP_IRQHandler = Default_Handler
#pragma weak HASH_RNG_IRQHandler = Default_Handler
#pragma weak FPU_IRQHandler = Default_Handler

//*****************************************************************************
//
// This is the code that gets called when the processor receives an unexpected
// interrupt.	This simply enters an infinite loop, preserving the system state
// for examination by a debugger.
//
//*****************************************************************************
void Default_Handler(void) {
	// Go into an infinite loop.
	//
	while (1) {
	}
}
