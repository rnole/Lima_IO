#include "EEPROM.h"

void EEPROM_write_byte(int deviceaddress, uint8_t first_byte, byte data ) 
{
  Wire.beginTransmission(deviceaddress);
  Wire.write((int)(first_byte >> 8));   // MSB
  Wire.write((int)(first_byte & 0xFF)); // LSB
  Wire.write(data);
  Wire.endTransmission();
 
  delay(5);
}
 
byte EEPROM_read_byte(int deviceaddress, unsigned int eeaddress ) 
{
  byte rdata = 0xFF;
 
  Wire.beginTransmission(deviceaddress);
  Wire.write((int)(eeaddress >> 8));   // MSB
  Wire.write((int)(eeaddress & 0xFF)); // LSB
  Wire.endTransmission();
 
  Wire.requestFrom(deviceaddress,1);
 
  if (Wire.available()) rdata = Wire.read();
 
  return rdata;
}

void EEPROM_show_content(uint8_t first_byte, uint16_t length){
  
   uint16_t i = 0;
   byte data  = 0;
   
   for(i = first_byte; i < length; i++){
     data = EEPROM_read_byte(EEPROM_ADDR, i);    
     Serial.print(data);
     Serial.print(" ");
     delay(1);
   } 
}


void EEPROM_set_block(uint8_t value, uint8_t first_byte, uint8_t length){
  
   uint16_t i = 0;   
   for(i =0; i < length; i++){
     EEPROM_write_byte(EEPROM_ADDR, first_byte + i, value);
   }  
}

void EEPROM_write_block(int deviceaddress, uint8_t first_byte, void *data, uint8_t len){
  
  uint16_t i = 0;
   for(i = 0; i < len; i++){
     EEPROM_write_byte(EEPROM_ADDR, first_byte + i, *((uint8_t *)(data+i)));
   }    
}

void EEPROM_read_block(int deviceaddress, uint8_t first_byte, void *data, uint8_t len){
  
   uint16_t i = 0;
   
   for(i = 0; i < len; i++){
     *((uint8_t *)(data + i)) = EEPROM_read_byte(EEPROM_ADDR, first_byte + i);    
     delay(1);
   } 
}

