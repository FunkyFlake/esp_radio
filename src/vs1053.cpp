#include "vs1053.hpp"

void VS1053::init()
{
    //Chip initialization
    pinMode(XCS_PIN, OUTPUT);
    pinMode(XDCS_PIN, OUTPUT);
    pinMode(DREQ_PIN, INPUT);
    

    SPI.beginTransaction(spi_slow);

}
