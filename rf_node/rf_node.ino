// junand 02.10.20116

#include "RemoteSwitch.h"

// Led Pin D4
// Rf Pin D7

const int LED_PIN = 2; // D4 / GIO2
const int RF_PIN = 13; // D7 / GIO13

struct SwitchCode {
  unsigned long onCode;
  unsigned long offCode;
};

#define NUM_RC 3
#define NUM_DEV 4

SwitchCode switchCode [NUM_RC] [NUM_DEV] = {
  // Fernbedienung 1 - Küche
  // DIP 123!4!5
  { { 18218, 18222}, { 19190, 19194 }, { 19514, 19518}, { 19622, 19626 } },

  // Fernbedienung 2 - Wohnzimmer
  // DIP 1!2!3!45
  { { 171308, 171312}, { 172280, 172284 }, { 172604, 172608}, { 172712, 172716 } },

  // Fernbedienung 3 - Terrarium
  // DIP !1!2345
  { { 473114, 473118}, { 474086, 474090 }, { 474410, 474414}, { 474518, 474522 } },

  // Fernbedienung 4 - Nina
  // DIP !1234!5

};

void setup () {
    
    Serial.begin ( 9600 );
    Serial.println ( "start setup" );
    
    pinMode ( RF_PIN, OUTPUT );
    
    pinMode ( LED_PIN, OUTPUT );
    digitalWrite ( LED_PIN, HIGH );
    
}

void sendCode ( unsigned long code, int period  ) {

  // Format
  // pppppppp|prrrdddd|dddddddd|dddddddd (32 bit)
  // p - Geschw. 9 bit
  // r - Wiederholungen als 2log - r=3 entspricht 8 (2^3)
  // d - Code
  
  code |= (unsigned long) period << 23;
  code |= 4L << 20;
  
  RemoteSwitch::sendTelegram ( code, RF_PIN );

}

void switchPower ( int remoteControl, int device, boolean state ) {
  
  unsigned long code = 0UL;
  if ( state ) {
    code = switchCode [remoteControl] [device].onCode;
  }
  else {
    code = switchCode [remoteControl] [device].offCode;
  }
  if ( code != 0 ) sendCode ( code, 329 );
  
  Serial.print ( "rc=" ); Serial.println ( remoteControl );
  Serial.print ( "dev=" ); Serial.println ( device );
  Serial.print ( "code=" ); Serial.println ( code );
  
}

long lastSwitch = 0L;
const unsigned long switchDelay = 5000L;
boolean state = false;

void loop () {
    
    if ( ( lastSwitch + switchDelay ) < millis () ) {
        lastSwitch = millis ();
        state = !state;
        digitalWrite ( LED_PIN, state ? HIGH : LOW );
        switchPower ( 2, 2, state );
    }
    
}