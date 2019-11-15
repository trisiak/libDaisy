#include <stm32h7xx_hal.h>
#include "fatfs.h"
#include "dsy_sdmmc.h"
//    PC12     ------> SDMMC1_CK
//    PC11     ------> SDMMC1_D3
//    PC10     ------> SDMMC1_D2
//    PD2     ------> SDMMC1_CMD
//    PC9     ------> SDMMC1_D1
//    PC8     ------> SDMMC1_D0


SD_HandleTypeDef hsd1;

void dsy_sdmmc_init() 
{
	// Used to MX Init function from cube
	hsd1.Instance = SDMMC1;
	hsd1.Init.ClockEdge			  = SDMMC_CLOCK_EDGE_RISING;
	hsd1.Init.ClockPowerSave	  = SDMMC_CLOCK_POWER_SAVE_DISABLE;
	hsd1.Init.BusWide			  = SDMMC_BUS_WIDE_4B;
	hsd1.Init.HardwareFlowControl = SDMMC_HARDWARE_FLOW_CONTROL_DISABLE;
	hsd1.Init.ClockDiv			  = 62; // TODO: tweak for max speed.
}
void dsy_sdmmc_init_fatfs() 
{
	
}


void HAL_SD_MspInit(SD_HandleTypeDef* sdHandle)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	if(sdHandle->Instance == SDMMC1)
	{
		__HAL_RCC_SDMMC1_CLK_ENABLE();
		__HAL_RCC_GPIOC_CLK_ENABLE();
		__HAL_RCC_GPIOD_CLK_ENABLE();
		GPIO_InitStruct.Pin
			= GPIO_PIN_12 | GPIO_PIN_11 | GPIO_PIN_10 | GPIO_PIN_9 | GPIO_PIN_8;
		GPIO_InitStruct.Mode	  = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull	  = GPIO_NOPULL;
		GPIO_InitStruct.Speed	 = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF12_SDIO1;
		HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

		GPIO_InitStruct.Pin		  = GPIO_PIN_2;
		GPIO_InitStruct.Mode	  = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull	  = GPIO_NOPULL;
		GPIO_InitStruct.Speed	 = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF12_SDIO1;
		HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
	}
}

void HAL_SD_MspDeInit(SD_HandleTypeDef* sdHandle)
{
	if(sdHandle->Instance == SDMMC1)
	{
		__HAL_RCC_SDMMC1_CLK_DISABLE();
		HAL_GPIO_DeInit(GPIOC,
						GPIO_PIN_12 | GPIO_PIN_11 | GPIO_PIN_10 | GPIO_PIN_9
							| GPIO_PIN_8);

		HAL_GPIO_DeInit(GPIOD, GPIO_PIN_2);
	}
}
