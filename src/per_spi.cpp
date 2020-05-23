#include "per_spi.h"
#include "util_hal_map.h"

// TODO
// - fix up rest of lib so that we can add a spi_handle map to the hal map
// - Add configuration for standard spi stuff.

using namespace daisy;

struct SpiInstance
{
    SPI_HandleTypeDef hspi;
    SpiConfig         config;
};

void InitSpiPins(SpiInstance* spi);

static SpiInstance instance[3];

static void Error_Handler()
{
    asm("bkpt 255");
}

void SpiHandle::Init(SpiConfig config)
{
    // create pointer to periph index of private config struct
    SpiInstance* instance_p;
    instance_p = &instance[config.periph];

    // assign passed config struct to private version
    instance_p->config = config;

    switch(config.periph)
    {
        case SpiConfig::PERIPH_1: instance_p->hspi.Instance = SPI1; break;
        case SpiConfig::PERIPH_3: instance_p->hspi.Instance = SPI3; break;
        case SpiConfig::PERIPH_6: instance_p->hspi.Instance = SPI6; break;
        default: instance_p->hspi.Instance = SPI1; break;
    }

    if(config.data_size - 1 > SPI_DATASIZE_4BIT
       && config.data_size - 1 < SPI_DATASIZE_32BIT)
        instance_p->hspi.Init.DataSize = config.data_size - 1;
    else
        instance_p->hspi.Init.DataSize = SPI_DATASIZE_8BIT;

    switch(config.chip_select)
    {
        case SpiConfig::CS_SOFT:
            instance_p->hspi.Init.NSS = SPI_NSS_SOFT;
            break;
        case SpiConfig::CS_HARD_IN:
            instance_p->hspi.Init.NSS = SPI_NSS_HARD_INPUT;
            break;
        case SpiConfig::CS_HARD_OUT:
            instance_p->hspi.Init.NSS = SPI_NSS_HARD_OUTPUT;
            break;
        default: instance_p->hspi.Init.NSS = SPI_NSS_HARD_OUTPUT; break;
    }

    switch(config.clock_divide)
    {
        case SpiConfig::CLOCK_DIVIDE_2:
            instance_p->hspi.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
            break;
        case SpiConfig::CLOCK_DIVIDE_4:
            instance_p->hspi.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
            break;
        case SpiConfig::CLOCK_DIVIDE_8:
            instance_p->hspi.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
            break;
        case SpiConfig::CLOCK_DIVIDE_16:
            instance_p->hspi.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
            break;
        case SpiConfig::CLOCK_DIVIDE_32:
            instance_p->hspi.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;
            break;
        case SpiConfig::CLOCK_DIVIDE_64:
            instance_p->hspi.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_64;
            break;
        case SpiConfig::CLOCK_DIVIDE_128:
            instance_p->hspi.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_128;
            break;
        case SpiConfig::CLOCK_DIVIDE_256:
            instance_p->hspi.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
            break;
        default:
            instance_p->hspi.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
            break;
    }

    //hspi1.Instance               = SPI1;
    instance_p->hspi.Init.Mode      = SPI_MODE_MASTER;
    instance_p->hspi.Init.Direction = SPI_DIRECTION_2LINES_TXONLY;
    //    hspi1.Init.DataSize    = SPI_DATASIZE_8BIT;
    instance_p->hspi.Init.CLKPolarity = SPI_POLARITY_LOW;
    instance_p->hspi.Init.CLKPhase    = SPI_PHASE_1EDGE;
    //hspi1.Init.NSS               = SPI_NSS_HARD_OUTPUT;
    //hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
    instance_p->hspi.Init.FirstBit       = SPI_FIRSTBIT_MSB;
    instance_p->hspi.Init.TIMode         = SPI_TIMODE_DISABLE;
    instance_p->hspi.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    instance_p->hspi.Init.CRCPolynomial  = 0x0;
    instance_p->hspi.Init.NSSPMode       = SPI_NSS_PULSE_ENABLE;
    instance_p->hspi.Init.NSSPolarity    = SPI_NSS_POLARITY_LOW;
    instance_p->hspi.Init.FifoThreshold  = SPI_FIFO_THRESHOLD_01DATA;
    instance_p->hspi.Init.TxCRCInitializationPattern
        = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
    instance_p->hspi.Init.RxCRCInitializationPattern
        = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
    instance_p->hspi.Init.MasterSSIdleness = SPI_MASTER_SS_IDLENESS_00CYCLE;
    instance_p->hspi.Init.MasterInterDataIdleness
        = SPI_MASTER_INTERDATA_IDLENESS_00CYCLE;
    instance_p->hspi.Init.MasterReceiverAutoSusp
        = SPI_MASTER_RX_AUTOSUSP_DISABLE;
    instance_p->hspi.Init.MasterKeepIOState = SPI_MASTER_KEEP_IO_STATE_DISABLE;
    instance_p->hspi.Init.IOSwap            = SPI_IO_SWAP_DISABLE;
    if(HAL_SPI_Init(&instance_p->hspi) != HAL_OK)
    {
        Error_Handler();
    }
}

void SpiHandle::BlockingTransmit(uint8_t* buff, size_t size)
{
    //HAL_SPI_Transmit(&hspi1, buff, size, 100);
}

void HAL_SPI_MspInit(SPI_HandleTypeDef* spiHandle)
{
    if(spiHandle->Instance == SPI1)
    {
        __HAL_RCC_GPIOB_CLK_ENABLE();
        __HAL_RCC_GPIOG_CLK_ENABLE();
        InitSpiPins(&instance[SpiConfig::PERIPH_1]);
        __HAL_RCC_SPI1_CLK_ENABLE();
    }
    else if(spiHandle->Instance == SPI3)
    {
        __HAL_RCC_GPIOC_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();
        InitSpiPins(&instance[SpiConfig::PERIPH_3]);
        __HAL_RCC_SPI3_CLK_ENABLE();
    }
    else if(spiHandle->Instance == SPI6)
    {
        InitSpiPins(&instance[SpiConfig::PERIPH_6]);
        __HAL_RCC_SPI6_CLK_ENABLE();
    }
}

void HAL_SPI_MspDeInit(SPI_HandleTypeDef* spiHandle)
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

void InitSpiPins(SpiInstance* spi)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_TypeDef*    port;

    for(uint8_t i = 0; i < SpiConfig::PIN_LAST; i++)
    {
        port                = dsy_hal_map_get_port(&spi->config.pins[i]);
        GPIO_InitStruct.Pin = dsy_hal_map_get_pin(&spi->config.pins[i]);

        GPIO_InitStruct.Mode  = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull  = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        // all AF pins get defined to the same address
        // consider removing switch statement and just assigning to GPIO_AF5_SPI1
        // regardless of periph
        switch(spi[i].config.periph)
        {
            case SpiConfig::PERIPH_1:
                GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
                break;
            case SpiConfig::PERIPH_3:
                GPIO_InitStruct.Alternate = GPIO_AF5_SPI3;
                break;
            case SpiConfig::PERIPH_6:
                GPIO_InitStruct.Alternate = GPIO_AF5_SPI6;
                break;
            default: GPIO_InitStruct.Alternate = GPIO_AF5_SPI1; break;
        }

        HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);
    }

    /**SPI1 GPIO Configuration    
    PB5     ------> SPI1_MOSI
    PB4 (NJTRST)     ------> SPI1_MISO
    PG11     ------> SPI1_SCK
    PG10     ------> SPI1_NSS 
    */
    switch(spi->hspi.Init.Direction)
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
}
