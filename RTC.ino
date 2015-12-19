#include "Definitions.h"
#include "RTC.h"

void RTC_init(void) {

  RTC.begin();
//  RTC.adjust(DateTime(F(__DATE__), F(__TIME__)));  
  if (!RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
    RTC.adjust(DateTime(F(__DATE__), F(__TIME__)));  
  }
}
