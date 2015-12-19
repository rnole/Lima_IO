#ifndef EEPROM_H
#define EEPROM_H

void EEPROM_write_byte(int deviceaddress, uint8_t first_byte, byte data);
void EEPROM_write_block(int deviceaddress, uint8_t first_byte, void *data, uint8_t len);
void EEPROM_read_block(int deviceaddress, uint8_t first_byte, void *data, uint8_t len);
byte EEPROM_read_byte(int deviceaddress, unsigned int eeaddress);
void EEPROM_show_content(uint8_t first_byte, uint16_t length); 


#endif  /* EEPROM_H */
