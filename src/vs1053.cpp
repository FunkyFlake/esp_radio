#include "vs1053.hpp"

/**
 * @brief Initialization sequence for the VS1053 board:
 * - Configures hardware pins
 * - Cycles the chip select XCS and data chip select to ensure correct levels 
 * - Checks whether DREQ is HIGH to check the hardware 
 * - Starts SPI communication with slow speed (200kHz)
 * - Rudimentary test of SPI communication 
 * - Ensures the VS1053 is in MP3 mode and not MIDI mode (includes soft reset)
 * - Sets audioformat i.e. samplerate
 * - Sets the internal clock multiplier to 3.5x 
 * - Switch to fast SPI speed (4MHz)
 * - Rudimentary test of SPI communication
 * - Sets initial SCI mode
 */
void VS1053::init(uint16_t &samplerate) {
    pinMode(XCS_PIN, OUTPUT);
    pinMode(XDCS_PIN, OUTPUT);
    pinMode(DREQ_PIN, INPUT);
    
    Serial.println("Initializing VS1053");
    digitalWrite(XCS_PIN, HIGH);
    digitalWrite(XDCS_PIN, HIGH);
    delay(150);
    digitalWrite(XCS_PIN, LOW);
    digitalWrite(XDCS_PIN, LOW);
    delay(150);
    digitalWrite(XCS_PIN, HIGH);
    digitalWrite(XDCS_PIN, HIGH);
    delay(150);

    // If everything is connected correctly then DREQ should be high after toggling the cs pins
    if(digitalRead(DREQ_PIN))
        Serial.println("DREQ HIGH");
    else
        Serial.println("DREQ LOW. Something is wrong -> Check connections and restart.");
    
    SPI.begin();
    testSPI();

    set_mp3_mode();

    if(samplerate > 48000) 
        samplerate = 44100;
    
    set_audioformat(samplerate, STEREO);
    
    set_clock();
    spi_settings = spi_fast;
    testSPI();

    set_mode(sci_mode);
}

/**
 * @brief Writes data to the specified VS1053 SCI register. Waits for DREQ to be HIGH again
 * before returning.
 * 
 * @param reg is the SCI register address that is accessed
 * @param data is the data that is written to the register
 */
void VS1053::write_reg(const uint8_t& reg, const uint16_t& data) const {
    if(!digitalRead(DREQ_PIN)) {
        Serial.println("VS1053 SCI is not ready for new data.");
        return;
    }

    SPI.beginTransaction(spi_settings);
    digitalWrite(XCS_PIN, LOW);
    
    SPI.write(WRITE_CMD);
    SPI.write(reg);
    SPI.write16(data);
    
    wait4DREQ();        
    
    digitalWrite(XCS_PIN, HIGH);
    SPI.endTransaction();
    
    return;
}


/**
 * @brief Reads a SCI register by first sending the READ instruction and register address 
 * and receiving of the 16 bit data afterwards. Waits for DREQ to be HIGH again before 
 * returning.
 * 
 * @param reg is the SCI register address that is accessed
 * @return uint16_t register data
 */
uint16_t VS1053::read_reg(const uint8_t& reg) const {
    if(reg > 0x0F || !digitalRead(DREQ_PIN)) {
        Serial.println("VS1053 SCI is not ready for next command.");
        return 0xFFFF;
    } 
    
    SPI.beginTransaction(spi_settings);
    digitalWrite(XCS_PIN, LOW);
    
    SPI.write(READ_CMD);
    SPI.write(reg);
    uint16_t data = SPI.transfer16(0xFFFF);

    wait4DREQ();

    digitalWrite(XCS_PIN, HIGH);
    SPI.endTransaction();

    return data;
}


/**
 * @brief Simple SPI test function that checks whether the written value matches the
 * read register value. At the moment only the volume register is used for testing.
 * 
 */
void VS1053::testSPI() const {
    Serial.println("Testing SPI:");
    
 
    Serial.println("Writing to register...");
    write_reg(REG_VOL, 0xCAFE);
 
    Serial.println("Reading from register...");
    uint16_t vol = read_reg(REG_VOL);

    if(vol == 0xCAFE)
        Serial.println("SPI is functional.\n");
    else
    {
        Serial.println("SPI is not functional. Check hardware connection.\n");
    }

    write_reg(REG_VOL, 0x0000); 
}


/**
 * @brief Waits for the data request pin DREQ to be HIGH. This is a blocking function.
 * 
 */
void VS1053::wait4DREQ() const {
    while(!digitalRead(DREQ_PIN))
        yield();
}

/**
 * @brief Sets the samplerate and channel to either MONO or STEREO.
 * 
 * @param samplerate is the new samplerate, must be smaller than 48kHz
 * @param stereo is MONO or STEREO
 */
void VS1053::set_audioformat(const uint16_t& samplerate, const channels_t& stereo) const {
    if(samplerate > 48000) {
        Serial.println("Samplerate must be below 48kHz.\n");
        return;
    }

    uint16_t data = samplerate | stereo;
    write_reg(REG_AUDATA, data);
    Serial.print("VS1053 audio settings have been set to AUDATA=0x");
    Serial.println(read_reg(REG_AUDATA), HEX);
    return;
}

/**
 * @brief Set clock multiplier to 3.5 with no SC_ADD allowed. 
 * 
 */
void VS1053::set_clock() const {
    write_reg(REG_CLOCKF, SC_MULT_3 | SC_ADD_NO); 
    Serial.print("VS1053 clock multiplier has been set to CLOCKF=0x");
    Serial.println(read_reg(REG_CLOCKF), HEX);
}


/**
 * @brief Writes the specified mode to the SCI_MODE register for configuration.
 * 
 * @param mode is the new mode
 */
void VS1053::set_mode(const uint16_t &mode) {
    write_reg(REG_MODE, mode);
    sci_mode = read_reg(REG_MODE);
    Serial.print("VS1053 mode has been set to MODE=0x");
    Serial.println(sci_mode, HEX);
}

/**
 * @brief Sends data to the VS1053 in 32 byte blocks until the buffer is empty.
 * 
 * @param buffer is a pointer to the data buffer
 * @param bufsize is the size of the buffer
 */
void VS1053::send_data(const uint8_t *buffer, uint16_t bufsize) const {
    uint16_t packetlen;
    
    SPI.beginTransaction(spi_settings);
    digitalWrite(XDCS_PIN, LOW);

    while(bufsize > 0) {
        wait4DREQ();        
        
        packetlen = bufsize;

        if(packetlen > DATA_BLOCK_SIZE) {
            packetlen = DATA_BLOCK_SIZE;
        }

        for(uint16_t i = 0; i < packetlen; i++) {
            SPI.transfer(*buffer++);
        }

        bufsize -= packetlen;
    }
        
    digitalWrite(XDCS_PIN, HIGH);
    SPI.endTransaction();
    
    Serial.println("Buffer was transmitted to VS1053.");
}

/**
 * @brief Writes to the WRAM of the VS1053 which is used for user applications.
 * 
 * @param address is the wram address
 * @param data is the data that is written into the wram
 */
void VS1053::write_wram(const uint16_t &address, const uint16_t &data) const {
    write_reg(REG_WRAMADDR, address);
    write_reg(REG_WRAM, data);
}


/**
 * @brief Some boards start in MIDI mode, this reverses the problem without requiring
 * hardware changes.
 * See http://bajdi.com/lcsoft-vs1053-mp3-module/#comment-33773
 * and https://github.com/baldram/ESP_VS1053_Library/blob/master/src/VS1053.cpp
 * for more detail.
 */
void VS1053::set_mp3_mode() const {
    write_wram(GPIO_DDR, 3); 
    write_wram(GPIO_ODATA, 0);
    delay(150);
    software_reset();
}


/**
 * @brief Performs a software reset of the VS1053.
 * 
 */
void VS1053::software_reset() const {
    write_reg(REG_MODE, sci_mode | SM_RESET);
    delay(50);
    wait4DREQ(); // DREQ will be high after reset is done
    Serial.println("Software reset of VS1053 is complete.");
}

/**
 * @brief Sends the buffer to the VS1053 via the SDI for playback. 
 * 
 * @param buffer is a pointer to the buffer with MP3 data.
 * @param bufsize is the size of the buffer.
 */
void VS1053::playback(const uint8_t *buffer, uint16_t bufsize) const {
    send_data(buffer, bufsize);
}

/**
 * @brief Sets the volume in the range 0 to 100%.
 * 
 * @param vol 0 to 100 where 100 is the max. volume.
 */
void VS1053::set_volume(uint8_t vol) {
    if(vol >= 100) {
        volume = 100;
        vol = 0x00;
    }
    else {
        volume = vol;
        vol = map(vol, 0, 100, 0xFE, 0x00);
    }
    write_reg(REG_VOL, (vol << 8) | vol);
    //Serial.print("VS1053 volume has been set to VOL=0x");
    //Serial.println(read_reg(REG_VOL), HEX);
}

/**
 * @brief Function to read the current volume.
 * 
 * @return * uint8_t volume in range 0 to 100
 */
uint8_t VS1053::get_volume() const {
    return volume;
}