// junand 02.10.20116

// ------------------------------------------------------------------------------------------------------------------------------------------

#define HOME_WLAN
// #define DEV_WLAN
// #define XPERIA_WLAN
// #define IPHONE_WLAN

// ------------------------------------------------------------------------------------------------------------------------------------------

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include <ESP8266HTTPClient.h>

#include <PrintEx.h>
using namespace ios;

// #include <ArduinoJson.h>
#include <PubSubClient.h>

#include "RemoteSwitch.h"

#include "WLAN.h"
//const char* ssid     = "your_ssid";
//const char* password = "your_password";

// ------------------------------------------------------------------------------------------------------------------------------------------

#define VERSION "V0.20 (" __DATE__ ", " __TIME__ ")"

// ------------------------------------------------------------------------------------------------------------------------------------------

PrintEx printExSerial = Serial;
#define MY_SERIAL printExSerial
// #define MY_SERIAL Serial

// ------------------------------------------------------------------------------------------------------------------------------------------

const int LED_PIN = 5; // D1 / GIO5
const int RF_PIN = 13; // D7 / GIO13

const int PERIOD = 329;

struct SwitchCode {
  const char *name;
  unsigned long onCode;
  unsigned long offCode;
};

#define NUM_DEVICES 12

SwitchCode switchCode [NUM_DEVICES] = {
  // Fernbedienung 1 - Küche
  // DIP 123!4!5
  { "lampKitchenLongStrip",     18218, 18222 },     // 1-A
  { "lampKitchenShortStrip",    19190, 19194 },     // 1-B 
  { "noName1C",                 19514, 19518 },     // 1-C
  { "noName1D",                 19622, 19626 },     // 1-D

  // Fernbedienung 2 - Wohnzimmer
  // DIP 1!2!3!45
  { "lampStudyPillar",          171308, 171312 },   // 2-A
  { "lampLoungeWhiteBall",      172280, 172284 },   // 2-B
  { "lampLoungeBlackBowl",      172604, 172608 },   // 2-C
  { "lampLoungeReading",        172712, 172716 },   // 2-D

  // Fernbedienung 3 - Terrarium
  // DIP !1!2345
  { "noName3A",                 473114, 473118 },   // 3-A 
  { "noName3B",                 474086, 474090 },   // 3-B
  { "lamp1",                    474410, 474414 },   // 3-C
  { "noName3D",                 474518, 474522 },   // 3-D

  // Fernbedienung 4 - Nina
  // DIP !1234!5

};

// ------------------------------------------------------------------------------------------------------------------------------------------

WiFiClient wifiClient;

// ------------------------------------------------------------------------------------------------------------------------------------------

const int MQTT_PORT = 1883;
const char* MQTT_CLIENT_ID = "ESP8266_rf_node";

#if defined HOME_WLAN
const IPAddress MQTT_SERVER ( 192, 168, 2, 117 );
#elif defined DEV_WLAN
const IPAddress MQTT_SERVER ( 192, 168, 2, 101 );
#elif defined XPERIA_WLAN
const IPAddress MQTT_SERVER ( 192, 168, 43, 78 );
#elif defined IPHONE_WLAN
const IPAddress MQTT_SERVER ( 172, 20, 10, 2 );
#endif

PubSubClient mqttClient ( wifiClient );
long lastMqttReconnect;
const long MQTT_RECONNECT_PERIOD = 5000L;

#define NODE_CLASS "switch"
#define NODE_TYPE "rfhub"
#define NODE_LOCATION "first"

#define MQTT_TOPIC_NODE "nodes@home/" NODE_CLASS "/" NODE_TYPE "/" NODE_LOCATION
#define MQTT_TOPIC_VOLTAGE MQTT_TOPIC_NODE    "/voltage"

const int MQTT_QOS = 0;
// const int MQTT_RETAIN = 1;

// ------------------------------------------------------------------------------------------------------------------------------------------

void setup () {
    
    delay ( 100 );
    
    // --- init MY_SERIAL --------------------------------------------------------
    Serial.begin ( 9600 );
    delay ( 2 );
    MY_SERIAL.print ( "\n[SETUP] start version=" );
    MY_SERIAL.println ( VERSION );
    
    // --- init led pin -------------------------------------------------------
    pinMode ( LED_PIN, OUTPUT );
    digitalWrite ( LED_PIN, LOW );

    // --- init rf pin -------------------------------------------------------
    pinMode ( RF_PIN, OUTPUT );
    digitalWrite ( RF_PIN, LOW );
    
    // --- init wifi ----------------------------------------------------------
    MY_SERIAL.print ( "[SETUP] connecting " );
    MY_SERIAL.print ( ssid );
    
    WiFi.mode ( WIFI_STA );
    WiFi.begin ( ssid, password );
    while ( WiFi.status () != WL_CONNECTED ) {
        delay ( 500 );
        MY_SERIAL.print ( '.' );
    }

    MY_SERIAL << endl << "[SETUP] connected with ip ";
    MY_SERIAL.println ( WiFi.localIP () );

    // --- init mqtt -----------------------------------------------------------
    MY_SERIAL << "[MQTT] mqtt: server=" << MQTT_SERVER << " port=" << MQTT_PORT << " version=" << MQTT_VERSION << endl;
    mqttClient.setServer ( MQTT_SERVER, MQTT_PORT );
    mqttClient.setCallback ( mqttCallback );
    // mqttClient.setClient ( wifiClient );
    
}

// ------------------------------------------------------------------------------------------------------------------------------------------

void mqttCallback ( char* topic, byte* payload, unsigned int payloadLength ) {
    
    MY_SERIAL.print ( "[MQTT] payloadLength=" );
    MY_SERIAL.println ( payloadLength );
    
    MY_SERIAL.print ( "[MQTT] Message arrived [" );
    MY_SERIAL.print ( topic );
    MY_SERIAL.print ( "] >" );
    for ( int i = 0; i < payloadLength; i++ ) MY_SERIAL.print ( (char) payload [i] );
    MY_SERIAL.println( "<" );

    char buf [200] = { 0 };
    int len = payloadLength < sizeof ( buf ) ? payloadLength : sizeof ( buf ) - 1;
    strncpy ( buf, (const char*) payload, len );
    buf [len] = '\0';
        
#ifdef WITH_JSON
    StaticJsonBuffer<200> jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject ( buf );
#endif    

    if ( strncmp ( topic, MQTT_TOPIC_NODE, strlen ( MQTT_TOPIC_NODE ) ) == 0 ) {

        MY_SERIAL << "[MQTT] node topic received " << topic << endl;
        
        char topicState [strlen ( topic ) + 10];
        strcpy ( topicState, topic );
        strcat ( topicState, "/state" );
        MY_SERIAL << "[MQTT] topicState=" << &topicState [0] << endl;

        // find last token
        int i = 0;
        char deviceName [30];
        char* token = strtok ( topic, "/" );
        while ( token ) {
            // MY_SERIAL.printf ( "[MQTT] token=%s\n", token );
            if ( i == 4 ) {
                strncpy ( deviceName, token, sizeof ( deviceName ) );
            }
            token = strtok ( NULL, "/" );
            i++;
        }
        MY_SERIAL << "[MQTT} device detected: " << deviceName << endl;

        boolean state = false;
        boolean action = false;

#ifdef WITH_JSON
        if  ( root.success () ) {
            const char* stateText = root ["state"];
            MY_SERIAL << "[MQTT] state in json detected: " << stateText << endl;
            if ( strcmp ( "on", stateText ) == 0 ) {
                state = true;
            }
            MY_SERIAL << "state is <" << stateText << ">, state=" << state << endl; 
        }
        else {
            MY_SERIAL.printf ( "[MQTT] parsing failed payload=%s\n", payload );
        }
#else            
        // if ( strcmp ( "ON", (const char*) payload ) == 0  ) {
        if ( payloadLength == 2 && payload [0] == 'O' && payload [1] == 'N' ) {
            action = true;
            state = true;
        }
        else if ( payloadLength == 3 && payload [0] == 'O' && payload [1] == 'F' && payload [2] == 'F' ) {
            action = true;
            state = false;
        }
        if ( action == true ) {
            MY_SERIAL << "[MQTT] switching device <" << deviceName << "> to state=" << state << endl; 
            switchPower ( deviceName, state );
            digitalWrite ( LED_PIN, state );
            // mqttClient.publish ( topicState, state ? "ON" : "OFF", true ); // retained flag on
            mqttClient.publish ( topicState, state ? "ON" : "OFF", false ); // retained flag off
        }
        else {
            MY_SERIAL << "[MQTT] no switching" << endl; 
        }

#endif            
        
    }

}

void mqttReconnect () {
    
    // Loop until we're reconnected
    if ( !mqttClient.connected () && (lastMqttReconnect + MQTT_RECONNECT_PERIOD) < millis () ) {
      
        MY_SERIAL.print ( "[MQTT] Attempting MQTT connecting ... " );

        lastMqttReconnect = millis ();

        // Attempt to connect
        if ( mqttClient.connect ( MQTT_CLIENT_ID ) ) {
        
            MY_SERIAL.println ( "connected" );
            // Once connected, publish an announcement...
            mqttClient.publish ( MQTT_TOPIC_NODE, NODE_CLASS " " NODE_TYPE " " VERSION );
            // ... and resubscribe
            mqttClient.subscribe ( MQTT_TOPIC_NODE "/+" );
      
        } 
        else {
        
            MY_SERIAL.print ( "failed, rc=" );
            MY_SERIAL.print ( mqttClient.state () );
            MY_SERIAL.println ( " try again in 5 seconds" );

        }
    }
}
    
// ------------------------------------------------------------------------------------------------------------------------------------------

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

void switchPower ( int device, boolean state ) {
  
  unsigned long code = 0UL;
  if ( state ) {
    code = switchCode [device].onCode;
  }
  else {
    code = switchCode [device].offCode;
  }
  if ( code != 0 ) sendCode ( code, PERIOD );
  
  Serial.print ( "[SWITCH] dev=" ); Serial.println ( device );
  Serial.print ( "[SWITCH] code=" ); Serial.println ( code );
  
}

void switchPower ( const char* deviceName, boolean state ) {
    
    int device = -1;
    
    for ( int i = 0; i < NUM_DEVICES; i++ ) {
            if ( strcmp ( switchCode [i].name, deviceName ) == 0 ) {
                device = i;
                break;
            }
    }
    
    Serial.printf ( "[SWITCH] device %d found\n", device );
    
    if ( device > -1 ) {
        switchPower ( device, state );
    }
    
}

// ------------------------------------------------------------------------------------------------------------------------------------------

long lastSwitch = 0L;
const unsigned long switchDelay = 5000L;
boolean state = false;

void loop () {
    
    mqttReconnect ();
    mqttClient.loop ();
    
    // if ( ( lastSwitch + switchDelay ) < millis () ) {
        // lastSwitch = millis ();
        // state = !state;
        // digitalWrite ( LED_PIN, state ? HIGH : LOW );
        // // switchPower ( 10, state );
        // switchPower ( "lamp1", state );
    // }
    
}

// ------------------------------------------------------------------------------------------------------------------------------------------

