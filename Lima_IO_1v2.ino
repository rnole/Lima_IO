/**
Author: Richard Nole
Description: A weather station that sends collected data to a web server.

Components:
- ADC
- ENC28J60(SPI)
- DS1307 RTC(I2C)
- EEPROM(I2C)
- Pressure Sensor(I2C)
- Humidity Sensor(I2C)
*/

#include <Wire.h>
#include <UIPEthernet.h>
#include <RTClib.h>
#include <HTU21D.h>
#include <EnableInterrupt.h>
#include <SFE_BMP180.h>
#include "Lima_IO.h"
//#define DEBUG

/****************************************/
/** Globals                             */
/****************************************/
EthernetClient client;
IPAddress ip(192, 168, 0, 57);
IPAddress dns_addr(8, 8, 8, 8);
IPAddress gateway_addr(192, 168, 0, 1);
IPAddress subnet_mask(255, 255, 255, 0);

const char server[]     = "radioclima.limaio.com";
uint8_t  anem_counter   = 0;
uint8_t  err_conn_flag  = 0;
uint32_t last_wind_time = 0;
uint32_t last_rain_time = 0;
uint32_t current_time   = 0;


int uv2_int_part       ,  uv2_frac_part;
int vane_int_part      ,  vane_frac_part;
int anem_int_part      ,  anem_frac_part;
int rain_gauge_int_part,  rain_gauge_frac_part;
int humidity_int_part  ,  humidity_frac_part;
int temp_int_part      ,  temp_frac_part;
int pressure_int_part  ,  pressure_frac_part;

RTC_DS1307       RTC;
SFE_BMP180       Pressure_sensor;
HTU21D           Temp_sensor;
struct_weather   weather_data;

void setup() {
  Wire.begin();
  Ports_init();
  Serial.begin(9600);
  #ifndef DEBUG
  RTC_init();
  Temp_sensor_init();
  Pressure_sensor_init();
  RainG_init();
  #endif
  ENC28J60_init();
  sei();
  
}

void loop() {
  
  err_t    rc_status                 =  0;
  int      cursor_str                =  0;
  uint32_t last_time_request         =  0;
  char     param_str[MAX_QUERY_SIZE] = {0};

  #ifndef DEBUG
  DateTime now = RTC.now();
  
  snprintf(weather_data.day_str,  6, "%.2d-%.2d", now.day(),  now.month());
  snprintf(weather_data.time_str, 6, "%.2d:%.2d", now.hour(), now.minute());
  
  Serial.println(F("Leyendo sensores... "));
  weather_data.uv          = UV_sensor_read();                       //Units: mW/cm2
  weather_data.vane        = Vane_read();                            //Units: degrees 0° - 360°
  weather_data.anem        = Anem_get_wind_speed();                  //Units: Km/h
  weather_data.rain_gauge  = RainG_read();                           //Units: mm
  weather_data.humidity    = Temp_sensor.readHumidity();
  weather_data.temp        = Temp_sensor.readTemperature();          //Units: C°
  weather_data.pressure    = Pressure_sensor_read();                 //Units: mb
  
  split_float_number(weather_data.uv,         &uv2_int_part,        &uv2_frac_part);
  split_float_number(weather_data.vane,       &vane_int_part,       &vane_frac_part);
  split_float_number(weather_data.anem,       &anem_int_part,       &anem_frac_part);
  split_float_number(weather_data.rain_gauge, &rain_gauge_int_part, &rain_gauge_frac_part);
  split_float_number(weather_data.humidity,   &humidity_int_part,   &humidity_frac_part);
  split_float_number(weather_data.temp,       &temp_int_part,       &temp_frac_part);
  split_float_number(weather_data.pressure,   &pressure_int_part,   &pressure_frac_part);
  #endif
  
  #ifndef DEBUG  
  
  cursor_str =  snprintf(param_str, MAX_QUERY_SIZE, "precipit=%d.%02d&dir_viento=%d.%02d&id=03288&",
                        rain_gauge_int_part, rain_gauge_frac_part, vane_int_part, vane_frac_part);
	  
  cursor_str += snprintf(param_str + cursor_str, MAX_QUERY_SIZE - cursor_str, "vel_viento=%d.%02d&temp_aire=%d.%02d&",
			 anem_int_part, anem_frac_part, temp_int_part, temp_frac_part);
	
  cursor_str += snprintf(param_str + cursor_str, MAX_QUERY_SIZE - cursor_str, "hum_rel=%d.%02d&presion_atm=%d.%02d&",
			 humidity_int_part, humidity_frac_part, pressure_int_part, pressure_frac_part);

   cursor_str += snprintf(param_str + cursor_str, MAX_QUERY_SIZE - cursor_str, "time=%s_%s&uv=%d.%02d",
			  weather_data.day_str, weather_data.time_str, uv2_int_part, uv2_frac_part);
   
  #else
    strcpy(param_str, "precipit=100&dir_viento=5.2O&id=00001&vel_viento=2.4&temp_aire=20.4&hum_rel=80.2&presion_atm=100.1&uv=0.03");
  #endif
  
  Serial.print(F("\nA enviar: "));
  Serial.println(param_str);
  
  rc_status = Send_http_post_request(param_str);
  
  if(rc_status == ERR_CONN){
      Serial.print(F("Error de conexion con el server!"));
      err_conn_flag = 1;
  }
  
  //Delay to send another HTTP POST request
  last_time_request = millis();
  while((millis() - last_time_request) < HTTP_REQUEST_INTERVAL_MS);
  
}

void Ports_init(void) {
  DDRC &= ~((1 << UV_SENSOR) | (1 << VANE));
  DDRD &= ~((1 << INT0));
  DDRD |= (1 << 7);

  pinMode(ANEN, INPUT_PULLUP);
  pinMode(RAIN_G, INPUT_PULLUP);
  pinMode(10, OUTPUT);
  disableInterrupt(ANEN);
  disableInterrupt(RAIN_G);
}

void RainG_init(void) {
  enableInterrupt(RAIN_G, RainG_interruptFunction, FALLING);
}

void ENC28J60_init(void) {
  
  static byte    mymac[] = { 0x74, 0x69, 0x69, 0x2D, 0x30, 0x31};
  Ethernet.begin(mymac, ip, dns_addr, gateway_addr, subnet_mask);
  
  Serial.print(F("IP Address: "));
  Serial.println(Ethernet.localIP());
  Serial.print(F("Default Gateway IP: "));
  Serial.println(Ethernet.gatewayIP());
  
}

void Temp_sensor_init(void) {
   Temp_sensor.begin();
}

float UV_sensor_read(void) {
  uint16_t adc_reading = 0;
  float uv_voltage, uv_intensity = 0;

  adc_reading  = analogRead_average(UV_SENSOR);
  uv_voltage   = (adc_reading * 3.3) / 1023.0;
  
  uv_intensity = (15.0 - 0.0) * (uv_voltage - 0.99) / (2.9 - 0.99);
  
  if(uv_intensity < 0)
    uv_intensity = 0;
    
  return uv_intensity;
}

void Save_weather_parameters(struct_weather *weather_ptr){
  
  //First clear EEPROM   
  EEPROM_set_block(0x00, 0, sizeof(struct_weather));
  
  EEPROM_write_block(EEPROM_ADDR, 0, (void *)weather_ptr, sizeof(struct_weather));
}


