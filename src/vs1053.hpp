#ifndef VS1053_H
#define VS1053_H

#include <cstdint>
#include <SPI.h>

class VS1053
{
public:
    VS1053(uint8_t xcs_pin, uint8_t xdcs_pin, uint8_t dreq_pin):
        XCS_PIN(xcs_pin), XDCS_PIN(xdcs_pin), DREQ_PIN(dreq_pin) { }

    void init();

private:
    uint8_t XCS_PIN;
    uint8_t XDCS_PIN;
    uint8_t DREQ_PIN;
    
    SPISettings spi_slow{200000, MSBFIRST, SPI_MODE0};
    SPISettings spi_fast{4000000, MSBFIRST, SPI_MODE0};

    // Internal SCI Registers 
    const uint8_t MODE        = 0x00;
    const uint8_t STATUS      = 0x01;
    const uint8_t BASS        = 0x02;
    const uint8_t CLOCKF      = 0x03;
    const uint8_t DECODE_TIME = 0x04;        // current decoded time in full seconds
    const uint8_t AUDATA      = 0x05;
    const uint8_t WRAM        = 0x06;
    const uint8_t WRAMADDR    = 0x07;
    const uint8_t HDAT0       = 0x08;
    const uint8_t HDAT1       = 0x09;
    const uint8_t AIADDR      = 0x0A;
    const uint8_t VOL         = 0x0B;
    const uint8_t AICTRL0     = 0x0C;
    const uint8_t AICTRL1     = 0x0D;
    const uint8_t AICTRL2     = 0x0E;
    const uint8_t AICTRL3     = 0x0F;
};

#endif