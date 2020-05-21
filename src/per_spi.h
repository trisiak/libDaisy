#pragma once
#ifndef DSY_SPI_H
#define DSY_SPI_H

#include "daisy_core.h"

// # SPI
// TODO:
// - Add documentation
// - Add configuration
// - Add reception
// - Add IT
// - Add DMA

namespace daisy
{
class SpiHandle
{
    enum SpiPeriph
    {
        SPI_PERIPH_1,
        SPI_PERIPH_3,
        SPI_PERIPH_6,
    };

    enum SpiPin
    {
        SPI_PIN_CS,
        SPI_PIN_SCK,
        SPI_PIN_MOSI,
        SPI_PIN_MISO,
    };

    enum SpiDataSize
    {
        SPI_DATASIZE_4,
        SPI_DATASIZE_5,
        SPI_DATASIZE_6,
        SPI_DATASIZE_7,
        SPI_DATASIZE_8,
        SPI_DATASIZE_9,
        SPI_DATASIZE_10,
        SPI_DATASIZE_11,
        SPI_DATASIZE_12,
        SPI_DATASIZE_13,
        SPI_DATASIZE_14,
        SPI_DATASIZE_15,
        SPI_DATASIZE_16,
        SPI_DATASIZE_17,
        SPI_DATASIZE_18,
        SPI_DATASIZE_19,
        SPI_DATASIZE_20,
        SPI_DATASIZE_21,
        SPI_DATASIZE_22,
        SPI_DATASIZE_23,
        SPI_DATASIZE_24,
        SPI_DATASIZE_25,
        SPI_DATASIZE_26,
        SPI_DATASIZE_27,
        SPI_DATASIZE_28,
        SPI_DATASIZE_29,
        SPI_DATASIZE_30,
        SPI_DATASIZE_31,
        SPI_DATASIZE_32,
    };

    enum SpiNss
    {
        SPI_NSS_SOFT,
        SPI_NSS_HARD_IN,
        SPI_NSS_HARD_OUT,
    };

    enum SpiBaudRate
    {
        SPI_BAUDRATE_2,
        SPI_BAUDRATE_4,
        SPI_BAUDRATE_8,
        SPI_BAUDRATE_16,
        SPI_BAUDRATE_32,
        SPI_BAUDRATE_64,
        SPI_BAUDRATE_128,
        SPI_BAUDRATE_256,
    };

  public:
    SpiHandle() {}
    ~SpiHandle() {}

    void Init(SpiPeriph   periph,
              SpiNss      nss,
              SpiBaudRate baudrate,
              SpiDataSize data_size);

    void BlockingTransmit(uint8_t *buff, size_t size);

  private:
};

} // namespace daisy

#endif
