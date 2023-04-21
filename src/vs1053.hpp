#ifndef VS1053_H
#define VS1053_H

#include <cstdint>
#include <SPI.h>

class VS1053
{
public:
    VS1053(uint8_t xcs_pin, uint8_t xdcs_pin, uint8_t dreq_pin):
        XCS_PIN(xcs_pin), XDCS_PIN(xdcs_pin), DREQ_PIN(dreq_pin) { }

    void init(uint16_t &samplerate);

    typedef enum {MONO = 0, STEREO = 1} channels_t;

    void playback(uint8_t *buffer, uint16_t bufsize) const;


private:
    uint8_t XCS_PIN;
    uint8_t XDCS_PIN;
    uint8_t DREQ_PIN;
    
    SPISettings spi_slow{200'000, MSBFIRST, SPI_MODE0};
    SPISettings spi_fast{4'000'000, MSBFIRST, SPI_MODE0};
    SPISettings spi_settings = spi_slow;

    uint16_t sci_mode = SM_DINEW | SM_STREAM;

    // Internal SCI Registers 
    static constexpr uint8_t REG_MODE        = 0x00;
    static constexpr uint8_t REG_STATUS      = 0x01;
    static constexpr uint8_t REG_BASS        = 0x02;
    static constexpr uint8_t REG_CLOCKF      = 0x03;
    static constexpr uint8_t REG_DECODE_TIME = 0x04;       
    static constexpr uint8_t REG_AUDATA      = 0x05;
    static constexpr uint8_t REG_WRAM        = 0x06;
    static constexpr uint8_t REG_WRAMADDR    = 0x07;
    static constexpr uint8_t REG_HDAT0       = 0x08;
    static constexpr uint8_t REG_HDAT1       = 0x09;
    static constexpr uint8_t REG_AIADDR      = 0x0A;
    static constexpr uint8_t REG_VOL         = 0x0B;
    static constexpr uint8_t REG_AICTRL0     = 0x0C;
    static constexpr uint8_t REG_AICTRL1     = 0x0D;
    static constexpr uint8_t REG_AICTRL2     = 0x0E;
    static constexpr uint8_t REG_AICTRL3     = 0x0F;

    // SCI instructions
    static constexpr uint8_t READ_CMD  = 0b0000'0011;
    static constexpr uint8_t WRITE_CMD = 0b0000'0010;

    // CLOCKF register bitmasks
    static constexpr uint16_t SC_MULT_1   = 0x0000;
    static constexpr uint16_t SC_MULT_2   = 0x2000;
    static constexpr uint16_t SC_MULT_3   = 0x6000;
    static constexpr uint16_t SC_MULT_2_5 = 0x4000;
    static constexpr uint16_t SC_MULT_3_5 = 0x8000;
    static constexpr uint16_t SC_MULT_4   = 0xA000;
    static constexpr uint16_t SC_MULT_4_5 = 0xC000;
    static constexpr uint16_t SC_MULT_5   = 0xE000;
    
    static constexpr uint16_t SC_ADD_NO  = 0x0000;
    static constexpr uint16_t SC_ADD_1   = 0x0800;
    static constexpr uint16_t SC_ADD_1_5 = 0x1000;
    static constexpr uint16_t SC_ADD_2   = 0x1800;
    
    // MODE register bitmasks
    static constexpr uint16_t SM_DIFF          = 0x0001;
    static constexpr uint16_t SM_LAYER12       = 0x0002;
    static constexpr uint16_t SM_RESET         = 0x0004;
    static constexpr uint16_t SM_CANCEL        = 0x0008;
    static constexpr uint16_t SM_EARSPEAKER_LO = 0x0010;
    static constexpr uint16_t SM_TESTS         = 0x0020;
    static constexpr uint16_t SM_STREAM        = 0x0040;
    static constexpr uint16_t SM_EARSPEAKER_HI = 0x0080;
    static constexpr uint16_t SM_DACT          = 0x0100;
    static constexpr uint16_t SM_DIORD         = 0x0200;
    static constexpr uint16_t SM_DISHARE       = 0x0400;
    static constexpr uint16_t SM_DINEW         = 0x0800;
    static constexpr uint16_t SM_ADPCM         = 0x1000;
    static constexpr uint16_t SM_LINE1         = 0x4000;
    static constexpr uint16_t SM_CLK_RANGE     = 0x8000;

    // SDI Constants
    static constexpr uint16_t DATA_BLOCK_SIZE = 32;

    // GPIO register constants (necessary for switch form midi to mp3)
    static constexpr uint16_t GPIO_DDR   = 0xC017;
    static constexpr uint16_t GPIO_IDATA = 0xC018;
    static constexpr uint16_t GPIO_ODATA = 0xC019;
    
    void testSPI() const;
    void wait4DREQ() const;
    void software_reset() const;
    void send_data(uint8_t *buffer, uint16_t bufsize) const;
    void set_clock() const;
    void set_mode(const uint16_t &mode);
    void write_wram(const uint16_t &address, const uint16_t &data) const;
    void set_mp3_mode() const;
    void write_reg(const uint8_t& reg, const uint16_t& data) const;
    uint16_t read_reg(const uint8_t& reg) const; 
    void set_audioformat(const uint16_t& samplerate, const channels_t& stereo) const;
};

#endif