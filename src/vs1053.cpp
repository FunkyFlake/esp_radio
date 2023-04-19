#include "vs1053.hpp"

/**
 * @brief Initialization sequence for the VS1053 board.
 * 
 */
void VS1053::init()
{
    pinMode(XCS_PIN, OUTPUT);
    pinMode(XDCS_PIN, OUTPUT);
    pinMode(DREQ_PIN, INPUT);
    
    //Cycle control and data chip select pins (active low)
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

    if(digitalRead(DREQ_PIN))
        Serial.println("DREQ HIGH");
    else
        Serial.println("DREQ LOW. Something is wrong -> Check connections and restart.");
    SPI.begin();
    SPI.beginTransaction(spi_settings);
    uint16_t vol = read_ctrl(REG_VOL);
    Serial.println("Volume " + String(vol));
    Serial.println("Writing 0xCAFE");
    write_ctrl(REG_VOL, 0xCAFE);
    vol = read_ctrl(REG_VOL);
    Serial.println("Volume " + String(vol));
}

void VS1053::write_ctrl(const uint8_t reg, const uint16_t data)
{
    if(digitalRead(DREQ_PIN))
    {
        digitalWrite(XCS_PIN, LOW);
        
        SPI.write(WRITE_CMD);
        SPI.write(reg);
        SPI.write16(data);
        
        digitalWrite(XCS_PIN, HIGH);
    }
    else
    {
        Serial.println("VS1053 is not ready for new data.");
    }
}

uint16_t VS1053::read_ctrl(const uint8_t reg)
{
    if(reg > 0x0F) 
        return 0xFFFF;

    digitalWrite(XCS_PIN, LOW);
    
    SPI.write(READ_CMD);
    SPI.write(reg);
    uint16_t data = SPI.transfer16(0xFFFF);
    
    digitalWrite(XCS_PIN, HIGH);
    
    return data;
}