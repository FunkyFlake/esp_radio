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
    void write_reg(const uint8_t& reg, const uint16_t& data) const;
    uint16_t read_reg(const uint8_t& reg) const; 

    typedef enum {MONO = 0, STEREO = 1} channels_t;
    void set_audioformat(const uint16_t& samplerate, const channels_t& stereo) const;

private:
    uint8_t XCS_PIN;
    uint8_t XDCS_PIN;
    uint8_t DREQ_PIN;
    
    SPISettings spi_slow{200'000, MSBFIRST, SPI_MODE0};
    SPISettings spi_fast{4'000'000, MSBFIRST, SPI_MODE0};
    SPISettings spi_settings = spi_slow;

    // Internal SCI Registers 
    const uint8_t REG_MODE        = 0x00;
    const uint8_t REG_STATUS      = 0x01;
    const uint8_t REG_BASS        = 0x02;
    const uint8_t REG_CLOCKF      = 0x03;
    const uint8_t REG_DECODE_TIME = 0x04;       
    const uint8_t REG_AUDATA      = 0x05;
    const uint8_t REG_WRAM        = 0x06;
    const uint8_t REG_WRAMADDR    = 0x07;
    const uint8_t REG_HDAT0       = 0x08;
    const uint8_t REG_HDAT1       = 0x09;
    const uint8_t REG_AIADDR      = 0x0A;
    const uint8_t REG_VOL         = 0x0B;
    const uint8_t REG_AICTRL0     = 0x0C;
    const uint8_t REG_AICTRL1     = 0x0D;
    const uint8_t REG_AICTRL2     = 0x0E;
    const uint8_t REG_AICTRL3     = 0x0F;

    // SCI instructions
    const uint8_t READ_CMD  = 0b0000'0011;
    const uint8_t WRITE_CMD = 0b0000'0010;

    void testSPI() const;
    void wait4DREQ() const;
};

#endif