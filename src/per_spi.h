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
    enum Periph
    {
        PERIPH_1,
        PERIPH_3,
        PERIPH_6,
    };

    enum Pin
    {
        PIN_CS,
        PIN_SCK,
        PIN_MOSI,
        PIN_MISO,
    };

    enum DataSize
    {
        DATASIZE_4,
        DATASIZE_5,
        DATASIZE_6,
        DATASIZE_7,
        DATASIZE_8,
        DATASIZE_9,
        DATASIZE_10,
        DATASIZE_11,
        DATASIZE_12,
        DATASIZE_13,
        DATASIZE_14,
        DATASIZE_15,
        DATASIZE_16,
        DATASIZE_17,
        DATASIZE_18,
        DATASIZE_19,
        DATASIZE_20,
        DATASIZE_21,
        DATASIZE_22,
        DATASIZE_23,
        DATASIZE_24,
        DATASIZE_25,
        DATASIZE_26,
        DATASIZE_27,
        DATASIZE_28,
        DATASIZE_29,
        DATASIZE_30,
        DATASIZE_31,
        DATASIZE_32,
    };

    enum ChipSelect
    {
        CHIP_SELECT_SOFT,
        CHIP_SELECT_HARD_IN,
        CHIP_SELECT_HARD_OUT,
    };

    enum BaudRate
    {
        BAUDRATE_2,
        BAUDRATE_4,
        BAUDRATE_8,
        BAUDRATE_16,
        BAUDRATE_32,
        BAUDRATE_64,
        BAUDRATE_128,
        BAUDRATE_256,
    };

  public:
    SpiHandle() {}
    ~SpiHandle() {}

    void Init(Periph     periph,
              ChipSelect chip_select,
              BaudRate   baudrate,
              DataSize   data_size);

    void BlockingTransmit(uint8_t *buff, size_t size);

  private:
};

} // namespace daisy

#endif
