#include "per_spi.h"
#include "util_hal_map.h"

// TODO
// - fix up rest of lib so that we can add a spi_handle map to the hal map
// - Add configuration for standard spi stuff.


using namespace daisy;

static SPI_HandleTypeDef hspi1;

static void Error_Handler()
{
    asm("bkpt 255");
}

void SpiHandle::Init(Periph     periph,
                     ChipSelect chip_select,
                     BaudRate   baudrate,
                     DataSize   data_size)
{
    switch(periph)
    {
        case PERIPH_1: hspi1.Instance = SPI1; break;
        case PERIPH_3: hspi1.Instance = SPI3; break;
        case PERIPH_6: hspi1.Instance = SPI6; break;
    }

    switch(data_size)
    {
        case DATASIZE_4: hspi1.Init.DataSize = SPI_DATASIZE_4BIT; break;
        case DATASIZE_5: hspi1.Init.DataSize = SPI_DATASIZE_5BIT; break;
        case DATASIZE_6: hspi1.Init.DataSize = SPI_DATASIZE_6BIT; break;
        case DATASIZE_7: hspi1.Init.DataSize = SPI_DATASIZE_7BIT; break;
        case DATASIZE_8: hspi1.Init.DataSize = SPI_DATASIZE_8BIT; break;
        case DATASIZE_9: hspi1.Init.DataSize = SPI_DATASIZE_9BIT; break;
        case DATASIZE_10: hspi1.Init.DataSize = SPI_DATASIZE_10BIT; break;
        case DATASIZE_11: hspi1.Init.DataSize = SPI_DATASIZE_11BIT; break;
        case DATASIZE_12: hspi1.Init.DataSize = SPI_DATASIZE_12BIT; break;
        case DATASIZE_13: hspi1.Init.DataSize = SPI_DATASIZE_13BIT; break;
        case DATASIZE_14: hspi1.Init.DataSize = SPI_DATASIZE_14BIT; break;
        case DATASIZE_15: hspi1.Init.DataSize = SPI_DATASIZE_15BIT; break;
        case DATASIZE_16: hspi1.Init.DataSize = SPI_DATASIZE_16BIT; break;
        case DATASIZE_17: hspi1.Init.DataSize = SPI_DATASIZE_17BIT; break;
        case DATASIZE_18: hspi1.Init.DataSize = SPI_DATASIZE_18BIT; break;
        case DATASIZE_19: hspi1.Init.DataSize = SPI_DATASIZE_19BIT; break;
        case DATASIZE_20: hspi1.Init.DataSize = SPI_DATASIZE_20BIT; break;
        case DATASIZE_21: hspi1.Init.DataSize = SPI_DATASIZE_21BIT; break;
        case DATASIZE_22: hspi1.Init.DataSize = SPI_DATASIZE_22BIT; break;
        case DATASIZE_23: hspi1.Init.DataSize = SPI_DATASIZE_23BIT; break;
        case DATASIZE_24: hspi1.Init.DataSize = SPI_DATASIZE_24BIT; break;
        case DATASIZE_25: hspi1.Init.DataSize = SPI_DATASIZE_25BIT; break;
        case DATASIZE_26: hspi1.Init.DataSize = SPI_DATASIZE_26BIT; break;
        case DATASIZE_27: hspi1.Init.DataSize = SPI_DATASIZE_27BIT; break;
        case DATASIZE_28: hspi1.Init.DataSize = SPI_DATASIZE_28BIT; break;
        case DATASIZE_29: hspi1.Init.DataSize = SPI_DATASIZE_29BIT; break;
        case DATASIZE_30: hspi1.Init.DataSize = SPI_DATASIZE_30BIT; break;
        case DATASIZE_31: hspi1.Init.DataSize = SPI_DATASIZE_31BIT; break;
        case DATASIZE_32: hspi1.Init.DataSize = SPI_DATASIZE_32BIT; break;
    }

    switch(chip_select)
    {
        case CHIP_SELECT_SOFT: hspi1.Init.NSS = SPI_NSS_SOFT; break;
        case CHIP_SELECT_HARD_IN: hspi1.Init.NSS = SPI_NSS_HARD_INPUT; break;
        case CHIP_SELECT_HARD_OUT: hspi1.Init.NSS = SPI_NSS_HARD_OUTPUT; break;
    }

    switch(baudrate)
    {
        case BAUDRATE_2:
            hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
            break;
        case BAUDRATE_4:
            hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
            break;
        case BAUDRATE_8:
            hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
            break;
        case BAUDRATE_16:
            hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
            break;
        case BAUDRATE_32:
            hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;
            break;
        case BAUDRATE_64:
            hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_64;
            break;
        case BAUDRATE_128:
            hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_128;
            break;
        case BAUDRATE_256:
            hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
            break;
    }

    //hspi1.Instance               = SPI1;
    hspi1.Init.Mode      = SPI_MODE_MASTER;
    hspi1.Init.Direction = SPI_DIRECTION_2LINES_TXONLY;
    //    hspi1.Init.DataSize    = SPI_DATASIZE_8BIT;
    hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
    hspi1.Init.CLKPhase    = SPI_PHASE_1EDGE;
    //hspi1.Init.NSS               = SPI_NSS_HARD_OUTPUT;
    //hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
    hspi1.Init.FirstBit       = SPI_FIRSTBIT_MSB;
    hspi1.Init.TIMode         = SPI_TIMODE_DISABLE;
    hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    hspi1.Init.CRCPolynomial  = 0x0;
    hspi1.Init.NSSPMode       = SPI_NSS_PULSE_ENABLE;
    hspi1.Init.NSSPolarity    = SPI_NSS_POLARITY_LOW;
    hspi1.Init.FifoThreshold  = SPI_FIFO_THRESHOLD_01DATA;
    hspi1.Init.TxCRCInitializationPattern
        = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
    hspi1.Init.RxCRCInitializationPattern
        = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
    hspi1.Init.MasterSSIdleness        = SPI_MASTER_SS_IDLENESS_00CYCLE;
    hspi1.Init.MasterInterDataIdleness = SPI_MASTER_INTERDATA_IDLENESS_00CYCLE;
    hspi1.Init.MasterReceiverAutoSusp  = SPI_MASTER_RX_AUTOSUSP_DISABLE;
    hspi1.Init.MasterKeepIOState       = SPI_MASTER_KEEP_IO_STATE_DISABLE;
    hspi1.Init.IOSwap                  = SPI_IO_SWAP_DISABLE;
    if(HAL_SPI_Init(&hspi1) != HAL_OK)
    {
        Error_Handler();
    }
}

void SpiHandle::BlockingTransmit(uint8_t* buff, size_t size)
{
    HAL_SPI_Transmit(&hspi1, buff, size, 100);
}

void HAL_SPI_MspInit(SPI_HandleTypeDef* spiHandle)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    if(spiHandle->Instance == SPI1)
    {
        /* USER CODE BEGIN SPI1_MspInit 0 */

        /* USER CODE END SPI1_MspInit 0 */
        /* SPI1 clock enable */
        __HAL_RCC_SPI1_CLK_ENABLE();

        __HAL_RCC_GPIOB_CLK_ENABLE();
        __HAL_RCC_GPIOG_CLK_ENABLE();
        /**SPI1 GPIO Configuration    
    PB5     ------> SPI1_MOSI
    PB4 (NJTRST)     ------> SPI1_MISO
    PG11     ------> SPI1_SCK
    PG10     ------> SPI1_NSS 
    */
        //        GPIO_InitStruct.Pin       = GPIO_PIN_5 | GPIO_PIN_4;
        switch(spiHandle->Init.Direction)
        {
            case SPI_DIRECTION_2LINES_TXONLY:
                GPIO_InitStruct.Pin = GPIO_PIN_5;
                break;
            case SPI_DIRECTION_2LINES_RXONLY:
                GPIO_InitStruct.Pin = GPIO_PIN_4;
                break;
            default: GPIO_InitStruct.Pin = GPIO_PIN_4 | GPIO_PIN_5; break;
        }
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        // Sck and CS
        GPIO_InitStruct.Pin = GPIO_PIN_11;
        if(spiHandle->Init.NSS != SPI_NSS_SOFT)
        {
            GPIO_InitStruct.Pin |= GPIO_PIN_10;
        }
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
        HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

        /* USER CODE BEGIN SPI1_MspInit 1 */

        /* USER CODE END SPI1_MspInit 1 */
    }
}

void HAL_SPI_MspDeInit(SPI_HandleTypeDef* spiHandle)
{
    if(spiHandle->Instance == SPI1)
    {
        /* USER CODE BEGIN SPI1_MspDeInit 0 */

        /* USER CODE END SPI1_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_SPI1_CLK_DISABLE();

        /**SPI1 GPIO Configuration    
    PB5     ------> SPI1_MOSI
    PB4 (NJTRST)     ------> SPI1_MISO
    PG11     ------> SPI1_SCK
    PG10     ------> SPI1_NSS 
    */
        HAL_GPIO_DeInit(GPIOB, GPIO_PIN_5 | GPIO_PIN_4);

        HAL_GPIO_DeInit(GPIOG, GPIO_PIN_11 | GPIO_PIN_10);

        /* USER CODE BEGIN SPI1_MspDeInit 1 */

        /* USER CODE END SPI1_MspDeInit 1 */
    }
}
