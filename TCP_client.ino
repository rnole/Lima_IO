#include "Definitions.h"

err_t Send_http_post_request(char *param_str){
  
 // if you get a connection, report back via serial:
  if (client.connect(IPAddress(104, 131, 35,249), 80) == 1) {
    
      Serial.println("-> Connected");
      // Make a HTTP request:
      client.print(F("POST /api/mediciones?")); 
      client.print(param_str);
      client.println(F(" HTTP/1.1"));
      client.print(F("Host: "));
      client.println(server);
      client.println(F("Connection: close"));
      client.println(F("Content-Type: application/x-www-form-urlencoded"));
      client.println(F("Content-Length: 1"));
      client.println();
      client.print("");
      client.println();   
      Serial.println("-> Terminado de enviar!");
    
    
      int connectLoop = 0;
      int inChar;
      while(client.connected())
      {
        while(client.available())
        {
          inChar = client.read();
          Serial.write(inChar);
          connectLoop = 0;
        }
    
        delay(1);
        connectLoop++;
        if(connectLoop > 10000)
        {
          Serial.println();
          Serial.println(F("Timeout"));
          client.flush();
          client.stop();
        }
      }
  
      Serial.println();
      Serial.println(F("disconnecting."));
      client.stop(); 
      return ERR_OK;
    }
    else {
      // you didn't get a connection to the server:
      Serial.println(F("connection failed/n"));
      return ERR_CONN;
    }
}
