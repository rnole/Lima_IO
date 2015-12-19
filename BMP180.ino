#include "Definitions.h"
#include "BMP180.h"

void Pressure_sensor_init(void) {
  if (Pressure_sensor.begin())
    return;
  else
  {
    Serial.println(F("BMP180 init fail\n\n"));
    while (1); // Pause forever.
  }
}

float Pressure_sensor_read(void) {

  char status;
  double T, P;

  // If request is unsuccessful, 0 is returned.
  status = Pressure_sensor.startTemperature();
  if (status != 0) {
    delay(status);

    // Function returns 1 if successful, 0 if failure.
    status = Pressure_sensor.getTemperature(T);
    if (status != 0) {
      // The parameter is the oversampling setting, from 0 to 3 (highest res, longest wait).
      // If request is unsuccessful, 0 is returned.
      status = Pressure_sensor.startPressure(3);

      if (status != 0) {
        delay(status);
        status = Pressure_sensor.getPressure(P, T); //returns absolute pressure in mb

        if (status != 0) {
          // Print out the measurement:
          /*
          Serial.print("absolute pressure: ");
          Serial.print(P, 2);
          */
          return P;
        }
        else return 0.0;
      }
      else return 0.0;
    }
    else return 0.0; 
  }
  else return 0.0;   
}
