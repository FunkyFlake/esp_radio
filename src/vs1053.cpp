#include "vs1053.hpp"

/**
 * @brief Initialization sequence for the VS1053 board. 
 * 
 */
void VS1053::init() {
    pinMode(XCS_PIN, OUTPUT);
    pinMode(XDCS_PIN, OUTPUT);
    pinMode(DREQ_PIN, INPUT);
    
    //Cycle control and data chip select pins (active low) to make intial state deterministic
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

    uint16_t samplerate = 44100;
    set_audioformat(samplerate, STEREO);
    
    // Set clock multiplier and update spi settings
    set_clock();
    spi_settings = spi_fast;
    testSPI();

    set_mode(SM_LINE1 | SM_DINEW | SM_STREAM);
    return;
}

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

    return;
}

void VS1053::wait4DREQ() const {
    while(!digitalRead(DREQ_PIN))
        yield();
    return;
}

void VS1053::set_audioformat(const uint16_t& samplerate, const channels_t& stereo) const {
    if(samplerate > 48000) {
        Serial.println("Samplerate must be below 48kHz.\n");
        return;
    }

    uint16_t data = samplerate | stereo;
    //write_reg(REG_AUDATA, data);
    Serial.print("VS1053 audio settings have been set to AUDATA=0x");
    Serial.println(read_reg(REG_AUDATA), HEX);
    return;
}

/**
 * @brief Set clock multiplier to 3.5, no SC_ADD allowed. Then set SPI clock to 4MHz.
 * 
 */
void VS1053::set_clock() {
    write_reg(REG_CLOCKF, SC_MULT_3_5 | SC_ADD_NO); 
    Serial.print("VS1053 clock multiplier has been set to CLOCKF=0x");
    Serial.println(read_reg(REG_CLOCKF), HEX);
    return;
}

void VS1053::set_mode(const uint16_t &mode) {
    write_reg(REG_MODE, mode);
    Serial.print("VS1053 mode has been set to MODE=0x");
    Serial.println(read_reg(REG_MODE), HEX);
    return;
}

void VS1053::send_data(uint8_t *buffer, uint16_t bufsize) {
    uint16_t packetlen;
    
    SPI.beginTransaction(spi_settings);
    digitalWrite(XDCS_PIN, LOW);

    while(bufsize > 0) {
        wait4DREQ();        
        
        packetlen = bufsize;

        if(packetlen > DATA_BLOCK_SIZE) {
            packetlen = DATA_BLOCK_SIZE;
        }

        SPI.transfer(buffer, packetlen);
        buffer += packetlen;
        bufsize -= packetlen;
    }
        
    digitalWrite(XDCS_PIN, HIGH);
    SPI.endTransaction();
    
    Serial.println("Buffer was transmitted to VS1053.");
    return;    
}