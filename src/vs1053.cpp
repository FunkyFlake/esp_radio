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
        Serial.println("DREQ LOW - something is wrong...");
    //SPI.beginTransaction(spi_settings);
}

void write_ctrl(const uint16_t data)
{
    //    SPI.beginTransaction(spi_settings);

}

/*uint16_t read_ctrl(const uint16_t data)
{

}*/