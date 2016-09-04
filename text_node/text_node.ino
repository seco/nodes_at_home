// junand 16.07.2016

// ------------------------------------------------------------------------------------------------------------------------------------------

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include <ESP8266HTTPClient.h>

#include <PrintEx.h>
#include <Ticker.h>
#include <Time.h>

#include <ArduinoJson.h>

#include "MaxMatrix.h"
#include "Sprites.h"

using namespace ios;

#include "WLAN.h"
//const char* ssid     = "your_ssid";
//const char* password = "your_password";

// ------------------------------------------------------------------------------------------------------------------------------------------

#define VERSION PSTR("V0.01 (30.08.2016)")

// ------------------------------------------------------------------------------------------------------------------------------------------

const int DATA_PIN = 14;      // grün DIN pin of MAX7219 module     -> D5
const int LOAD_PIN = 12;      // gelb CS pin of MAX7219 module      -> D6
const int CLOCK_PIN = 13;     // schwarz CLK pin of MAX7219 module  -> D7

const int MAX_IN_USE = 8;    //change this variable to set how many MAX7219's you'll use

MaxMatrix m ( DATA_PIN, LOAD_PIN, CLOCK_PIN, MAX_IN_USE ); // define module

// erste Spalte zum Anzeigen, die ersten 64 Pixelspalten werden leer gelassen 
int insert_column = 0;

int text_duration = -1; // in sec, -1 no effect, 0 is forever
long textDisplayBegin;

volatile int display_column = 0;
volatile int display_row = 0;

// ISR, wird alle 100 ms <gerufen
#define DEFAULT_DELAY 10
volatile int shift_delay = DEFAULT_DELAY;
const int TICKER_RESOLUTION = 10; // one tick every 10 ms
int counter = 0;

#define NO_DIRECTION 0
#define HORIZONTAL_DIRECTION 1
#define VERTICAL_DIRECTION 2

typedef struct CommandStruct Command;

struct CommandStruct {

    int id;
    int direction;
    int from;
    int to;
    int delay;
    int next_command_id;
    Command *next_command;

    int d;
    boolean running;

};

const int COMMAND_NUM = 20;
Command commands [COMMAND_NUM];
volatile Command *cmd;
int cmd_index = 0;

// ------------------------------------------------------------------------------------------------------------------------------------------

ESP8266WebServer server ( 80 );

Ticker ticker;

// ------------------------------------------------------------------------------------------------------------------------------------------

//static byte SNTP_server_IP[]    = { 192, 168, 2, 1 }; // ntpd@mauzi
//static byte SNTP_server_IP[]    = { 192, 168, 2, 8 }; // ntpd@glutexo

//byte SNTP_server_IP[]    = { 192, 43, 244, 18};   // time.nist.gov
byte SNTP_server_IP[]    = { 130,149,17,21};      // ntps1-0.cs.tu-berlin.de xxx
//byte SNTP_server_IP[]    = { 192,53,103,108};     // ptbtime1.ptb.de
//byte SNTP_server_IP[]    = { 64, 90, 182, 55 };   // nist1-ny.ustiming.org
//byte SNTP_server_IP[]    = { 66, 27, 60, 10 };    // ntp2d.mcc.ac.uk
//byte SNTP_server_IP[]    = { 130, 88, 200, 4 };   // ntp2c.mcc.ac.uk
//byte SNTP_server_IP[]    = { 31, 193, 9, 10 };    // clock02.mnuk01.burstnet.eu 
//byte SNTP_server_IP[]    = { 82, 68, 133, 225 };  // ntp0.borg-collective.org.uk

const unsigned long SEVENTY_YEARS = 2208988800UL; // offset between ntp and unix time
const unsigned long OFFSET_CEST = 7200L;          // offset in sec for MESZ / CEST
const unsigned long OFFSET_CET = 3600L;           // offset in sec for MEZ  / CET

const unsigned long SYNC_INTERVAL = SECS_PER_HOUR;

WiFiUDP udp;

time_t current_time, last_time;

// ------------------------------------------------------------------------------------------------------------------------------------------

const int LED_PIN = 5;

// ------------------------------------------------------------------------------------------------------------------------------------------

HTTPClient http;

// ------------------------------------------------------------------------------------------------------------------------------------------

void setup () {
    
    delay ( 2000 );

    m.init (); // module initialize
    m.setIntensity ( 0x0 ); // dot matix intensity 0-15
    clear_matrix ();

    Serial.begin ( 9600 );
    delay ( 2 );
    Serial.println ( "start" ); // first serial print!
    
    // Timer1.initialize ( 50 ); // 50 µs, prescale is set by lib
    // Timer1.attachInterrupt ( doInterrupt );
    ticker.attach_ms ( TICKER_RESOLUTION, do_interrupt );

    cmd = NULL; 

    pinMode ( LED_PIN, OUTPUT );
    digitalWrite ( LED_PIN, HIGH );

    Serial.print ( "connecting " );
    Serial.println ( ssid );
    
    clear_matrix ();
    m.init ();
    
    WiFi.mode ( WIFI_STA );
    WiFi.begin ( ssid, password );
    
    while ( WiFi.status() != WL_CONNECTED ) {
        delay ( 500 );
        matrix_append_text ( "." );
        Serial.print ( '.' );
    }
    matrix_append_text ( "connected" );
    Serial << "connected ";
    
    server.on ( "/", handle_root );
    server.on ( "/debug", handle_debug );
    server.on ( "/init", handle_init );
    server.on ( "/clear", handle_clear );
    server.on ( "/display", handle_display );
	server.onNotFound ( handle_not_found );

    digitalWrite ( LED_PIN, HIGH );
    
    matrix_append_text ( " with ip " );
    matrix_append_text ( WiFi.localIP ().toString ().c_str () );
    Serial << "with ip ";
    Serial.println ( WiFi.localIP () );
    
    udp.begin ( 8888 ); // for connect to time server
    setSyncInterval ( SYNC_INTERVAL );
    setSyncProvider ( getNtpTime );
    Serial.print ( " now: " );
    digitalClockDisplay ( now () );
    
    server.begin(); // start web server
    
    delay ( 20000 );
    clear_matrix ();
  
    digitalWrite ( LED_PIN, HIGH );

}

// ------------------------------------------------------------------------------------------------------------------------------------------

const char* http_request ( const char* url ) {
    
    const char* result = NULL;
    
    Serial << "[HTTP] Start Http Client" << endl;
    
    http.begin ( url );
    int http_code = http.GET (); // send a GET request
    Serial << "[HTTP] http_code=" << http_code << endl;
    
    if ( http_code == HTTP_CODE_OK ) {
        
        int len = http.getSize ();
        Serial << "payload size=" << len << endl;
        
        String payload = http.getString ();
        Serial.println ( payload );
        result = payload.c_str ();
                
        Serial.print ( "[HTTP] connection closed or file end.\n" );
        
    }
    else {
        Serial << "[HTTP] hhtp request with http_code=" << http_code << " (" << http.errorToString ( http_code ) << ")" << endl;
    }
    
    http.end ();
    
    return result;
    
}

// ------------------------------------------------------------------------------------------------------------------------------------------
    
void handle_root() {

    const char html_template [] = \
"<html>\
  <head>\
    <title>ESP8266 Demo</title>\
    <style>\
      body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
    </style>\
  </head>\
  <body>\
    <h1>Hello from ESP8266!</h1>\
    <p>Uptime    : %02d:%02d:%02d</p>\
    <p>Date      : %02d:%02d</p>\
    <p>Time      : %02d:%02d:%04d</p>\
    <p>IP-Address: %s</p>\
    <p>Channel   : %d</p>\
  </body>\
</html>";

    const int buf_len = 400;
	char buf [buf_len];
    
	int sec = millis() / 1000;
	int min = sec / 60;
	int hr = min / 60;
    
	snprintf ( buf, buf_len, html_template, 
               hr, min % 60, sec % 60, 
               day ( current_time ), month ( current_time ), year ( current_time ),
               hour ( current_time ), minute ( current_time ), second ( current_time ),
               WiFi.localIP ().toString ().c_str (), WiFi.channel () 
    );
	server.send ( HTTP_CODE_OK, "text/html", buf );

}

void handle_not_found () {

	String message = "File Not Found\n\n";
	message += "URI: ";
	message += server.uri ();
	message += "\nMethod: ";
	message += ( server.method() == HTTP_GET ) ? "GET" : "POST";
	message += "\nArguments: ";
	message += server.args ();
	message += "\n";

	for ( uint8_t i = 0; i < server.args (); i++ ) {
		message += " " + server.argName ( i ) + ": " + server.arg ( i ) + "\n";
	}

	server.send ( HTTP_CODE_NOT_FOUND, "text/plain", message );

}

void handle_debug () {

    const int buf_len = 400;
	char buf [buf_len];
    const char* buf_template = \
"debug info\n\
    insert_column=%d\n\
    display_column=%d\n\
    display_row=%d\n\
    shift_delay=%d\n\
    text_duration=%d\n\
";

	snprintf ( buf, buf_len, buf_template, insert_column, display_column, display_row, shift_delay, text_duration );
    
    String tag = "xy";
    if ( server.args () > 0 ) {
        if ( server.hasArg ( "tag" ) ) {
            tag = server.arg ( "tag" );
        }
    }
    Serial.printf ( "[DEBUG] tag=%s\n", tag.c_str () );
    
    // const char* httpResponse = http_request ( "http://192.168.2.8/website/weather_test/current_weather_test.html" );
    const char* httpResponse = "\
{\
    \"coord\":{\"lon\":13.54,\"lat\":52.65},\
    \"weather\":[{\"id\":800,\"main\":\"Clear\",\"description\":\"clear sky\",\"icon\":\"01d\"}],\
    \"base\":\"stations\",\
    \"main\":{\"temp\":296.48,\"pressure\":1017,\"humidity\":52,\"temp_min\":294.15,\"temp_max\":303.15},\
    \"visibility\":10000,\
    \"wind\":{\"speed\":4.1,\"deg\":290},\
    \"clouds\":{\"all\":0},\
    \"dt\":1472891622,\
    \"sys\":{\"type\":1,\"id\":4892,\"message\":0.0347,\"country\":\"DE\",\"sunrise\":1472876456,\"sunset\":1472924870},\
    \"id\":2804759,\
    \"name\":\"Zepernick\",\
    \"cod\":200\
}\
";
    DynamicJsonBuffer jsonBuffer;
    // StaticJsonBuffer<2000> jsonBuffer;
    JsonObject& jsonRoot = jsonBuffer.parseObject ( httpResponse );
    if ( jsonRoot.success () ) {
        const char* data = jsonRoot [tag];
        Serial.printf ( "[DEBUG] %s=%s\n", tag.c_str (), data );
        snprintf ( buf, buf_len, "%s\njson: %s=%s\n", buf, tag.c_str (), data );
    }
    else {
        Serial.println ( "parseObject () failed") ;
    }

    server.send ( HTTP_CODE_OK, "text/plain", buf );

}

void handle_init () {
    
    m.init ();
    clear_matrix ();
    server.send ( HTTP_CODE_OK, "text/plain", "matrix initialized" );
    
}

void handle_clear () {
    
    clear_matrix ();
    server.send ( HTTP_CODE_OK, "text/plain", "text cleared" );
    
}

void handle_display () {
    
    if ( server.args () > 0 ) {
        if ( server.hasArg ( "text" ) ) {
            String text = server.arg ( "text" );
            Serial << "received text=" << text << endl;
            clear_matrix ();
            matrix_append_text ( text.c_str () );
            text_duration = -1;
        }
        if ( server.hasArg ( "duration" ) ) {
            String duration_str = server.arg ( "duration" );
            if ( duration_str == "forever" ) {
                text_duration = 0;
            }
            else {
                text_duration = atoi ( duration_str.c_str () );
            }
                
        }
        if ( server.hasArg ( "ticks" ) ) {
            String ticks_str = server.arg ( "ticks" );
            int ticks = atoi ( ticks_str.c_str () );
            Serial << "received ticks=" << ticks_str << endl;
            shift_delay = ticks;
        }
        if ( server.hasArg ( "brightness" ) ) {
            String brightness_str = server.arg ( "brightness" );
            int intensity = atoi ( brightness_str.c_str () );
            Serial << "received intensity=" << brightness_str << endl;
            m.setIntensity ( intensity );
        }

        server.send ( HTTP_CODE_OK, "text/plain", "display handled" );

    }
    else {
        server.send ( HTTP_CODE_BAD_REQUEST, "text/plain", "no text argument" );
    }
    
}

// ------------------------------------------------------------------------------------------------------------------------------------------

void clear_matrix () {

    m.clear ();
    // insert_column = NUM_DISPLAY_COLS;
    insert_column = 0;
    display_column = 0;
    display_row = 0;

}

// ------------------------------------------------------------------------------------------------------------------------------------------

void clear_command ( int i ) {

    commands [i].id = -1;
    commands [i].direction = NO_DIRECTION;
    commands [i].from = -1;
    commands [i].to = -1;
    commands [i].delay = -1;
    commands [i].next_command_id = -1;
    commands [i].next_command = NULL;
    commands [i].d = 0;
    commands [i].running = false;

}

void clear_all_commands () {

    cmd_index = 0;
    
    for ( int i = 0; i < COMMAND_NUM; i++ ) {
        clear_command ( i );
    }
    
}

struct CommandStruct *find_command ( int id ) {

    Command *result = NULL;

    for ( int i = 0; i < COMMAND_NUM; i++ ) {
        if ( commands [i].id == id ) {
            result = &commands [i];
        }
    }

    return result;

}

// ------------------------------------------------------------------------------------------------------------------------------------------

void do_interrupt () {

    volatile int* val;
    
    counter++;
    if ( counter > shift_delay ) {

        counter = 0;
        
        m.shiftOutMatrix ( display_column, display_row );

        if ( cmd != NULL ) {

            if ( cmd->direction == HORIZONTAL_DIRECTION ) val = &display_column;
            else if ( cmd->direction == VERTICAL_DIRECTION ) val = &display_row;

            if ( cmd->running ) {
                if ( *val == cmd->to ) {
                    set_next_command ();
                }
                else {
                    *val += cmd->d;
                }
            }
            else { // !running
                cmd->running = true;
                *val = cmd->from;
            }

        }
        
    }

}

void init_command ( int id, int direction, int from, int to, int next_id ) {

        commands [cmd_index].id = id;
        commands [cmd_index].direction = direction;
        commands [cmd_index].from = from;
        commands [cmd_index].to = to;
        commands [cmd_index].delay = DEFAULT_DELAY;
        if ( commands [cmd_index].from < commands [cmd_index].to ) {
            commands [cmd_index].d = 1;
        }
        else {
            commands [cmd_index].d = -1;
        }
        commands [cmd_index].next_command_id = next_id;
        
        cmd_index++;

}

void start_command ( int id ) {
    
    Command *command = find_command ( id );
    command->running = false;
    cmd = command;

}

void set_next_command () {

    cmd->running = false;
    
    if ( cmd->next_command_id > -1 ) {
        if ( cmd->next_command == NULL ) {
            cmd->next_command = find_command ( cmd->next_command_id );
        }
        cmd = cmd->next_command;
        if ( cmd != NULL ) cmd->running = false; // damit sich from initialisiert
    }

}

// ------------------------------------------------------------------------------------------------------------------------------------------

void matrix_append_text ( const String str ) {
    
    matrix_append_text ( str.c_str () );
    
}

/*

    UTF-8 Umalute
    
    Ä       C384
    Ö       C396
    Ü       C39C
    ä       C3A4
    ö       C3B6
    ü       C3BC
    ß       C39F
    
*/

void matrix_append_text ( const char* text ) {
    
    cmd = NULL;
    
    while ( *text ) {

        if ( *text == 0xC3 ) { // special handling for german umlaute
        
            switch ( *++text ) {
                case 0x84: // Ä     sprite index 95
                    insert_column += m.printChar ( 32 + 95, insert_column );
                    break;
                case 0x96: // Ö     sprite index 96
                    insert_column += m.printChar ( 32 + 96, insert_column );
                    break;
                case 0x9C: // Ü     sprite index 97
                    insert_column += m.printChar ( 32 + 97, insert_column );
                    break;
                case 0xA4: // ä     sprite index 98
                    insert_column += m.printChar ( 32 + 98, insert_column );
                    break;
                case 0xB6: // ö     sprite index 99
                    insert_column += m.printChar ( 32 + 99, insert_column );
                    break;
                case 0xBC: // ü     sprite index 100
                    insert_column += m.printChar ( 32 + 100, insert_column );
                    break;
                case 0x9F: // ü     sprite index 101
                    insert_column += m.printChar ( 32 + 101, insert_column );
                    break;
                default:
                    break;
            }
            
        }
        else {
            
            insert_column += m.printChar ( *text, insert_column );
            
        }

        text++;

    }
    
    if ( insert_column > NUM_DISPLAY_COLS ) {
        
        int col1 = 0;
        int col2 = insert_column - NUM_DISPLAY_COLS;
        init_command ( 1, HORIZONTAL_DIRECTION, col1, col2, 2 );
        init_command ( 2, HORIZONTAL_DIRECTION, col2, col1, 1 );
        
        start_command ( 1 );
        
        cmd_index++;

    }
    
    textDisplayBegin = millis ();

}

void show_time_date_char ( char c ) {

    if ( c == '1' ) insert_column += m.printEmptyCol ( insert_column );
    if ( c == ' ' ) {
        insert_column += m.printEmptyCol ( insert_column );
        insert_column += m.printEmptyCol ( insert_column );
        insert_column += m.printEmptyCol ( insert_column );
    }
    else {
        insert_column += m.printChar ( c, insert_column );
    }

}

void matrix_show_time ( time_t time ) {
    
    cmd = NULL;

    int len = 30;
    char buf [len];
    
    sprintf ( buf, "%02d%c%02d", hour ( time ), second ( time ) % 2 == 0 ? ':' : ' ', minute ( time ) );
    // Serial.println ( buf );

    insert_column = 21;
    
    for ( int i = 0; i < len; i++ ) {
        if ( buf [i] == 0x00 ) break;
        show_time_date_char ( buf [i] );
    }
    
}

void matrix_show_date ( time_t time ) {
    
    cmd = NULL;

    int len = 30;
    char buf [len];
    
    sprintf ( buf, "%02d.%02d.%04d", day ( time ), month ( time ), year ( time ) );
    // Serial.println ( buf );

    insert_column = 9;
    
    for ( int i = 0; i < len; i++ ) {
        if ( buf [i] == 0x00 ) break;
        show_time_date_char ( buf [i] );
    }
    
}

// ------------------------------------------------------------------------------------------------------------------------------------------
// time

boolean isCEST ( unsigned long t ) {
  
  boolean isCest = false;

  int m = month ( t );
  switch ( m ) {

    // CET
    case 1:       
    case 2:       
    case 11:       
    case 12:       
      isCest = false; 
      break;
      
    // CEST
    case 4:
    case 5:
    case 6: 
    case 7: 
    case 8: 
    case 9:   
      isCest = true; 
      break;
      
    // CEST -> CES / CES -> CEST
    case 10:
    case 3:
      isCest = (m==10); // Anfang Oktober ist Sommerzeit / Anfang MÃ¤rz ist Winterzeit
      if ( day ( t ) > 24 ) { // wir sind in der letzten Woche des Monats inkl. letztem Sonntag
        if ( weekday ( t ) + 31 - day ( t ) < 8 ) { // wir sind am So oder danach; 8 ist der kleinste Wert
          isCest = !isCest; // fast schon Winterzeit / Sommerzeit
          if ( weekday ( t ) == 1 && hour ( t ) < 1 ) { // es ist Sonntag und noch nicht 2 Uhr Ortszeit!
              isCest = !isCest; // aber doch noch ein bischen Sommerzeit / Winterzeit
          }
        }
      }
      break;
    
    default:
      break;
    
  }
  
  return isCest;
  
}

time_t getNtpTime () {
  
  sendNTPpacket ( SNTP_server_IP );

  delay ( 1000 );
  
  unsigned long time = recvNtpTime ();
  if ( time != 0L ) {
    time -= SEVENTY_YEARS;
//    time += (isCEST ( time ) ? OFFSET_CEST : OFFSET_CET); // erst hier ist time initial gesetzt
    if ( isCEST ( time ) ) {
      time += OFFSET_CEST;
    }
    else {
      time += OFFSET_CET;
    }
  }
      
  return time;

}

const int NTP_PACKET_SIZE = 48;                   // NTP time stamp is in first 48 bytes of message
byte packetBuffer [NTP_PACKET_SIZE];              // buffer to hold incoming(outgoing packets

void sendNTPpacket ( byte *address ) {

  memset ( packetBuffer, 0, NTP_PACKET_SIZE );
  
  // Init for NTP Request
  packetBuffer [0] = B11100011; // LI, Version, Mode 0xE3
  packetBuffer [1] = 0; // Stratum
  packetBuffer [2] = 6; // max intervall between messages in sec
  packetBuffer [3] = 0xEC; // clock precision
  // bytes 4 - 11 are for root delay ad dispersion and were set to 0 by memset
  packetBuffer [12] = 49; // four byte reference id
  packetBuffer [13] = 0x4E;
  packetBuffer [14] = 49;
  packetBuffer [15] = 52;
  
  // send the packet requesting a timestamp
  // port 123
  udp.beginPacket ( address, 123 );
  udp.write ( packetBuffer, NTP_PACKET_SIZE );
  udp.endPacket ();
  
}

unsigned long recvNtpTime () {
    
  if ( udp.parsePacket () ) {
    
    udp.read ( packetBuffer, NTP_PACKET_SIZE );
    
    // the time starts at byte 40, convert four bytes into long
    unsigned long hi = word ( packetBuffer [40], packetBuffer [41] );
    unsigned long lo = word ( packetBuffer [42], packetBuffer [43] );
    
    // this is NTP time (seconds since Jan 1 1900
    unsigned long secsSince1900 = hi << 16 | lo;
    
    return secsSince1900;
    
  }

  return 0L; // return 0 if unable to get the time

}

void digitalClockDisplay ( time_t time ){
    
  // digital clock display of the time
  Serial.print ( hour ( time ) );
  printDigits ( minute ( time ) );
  printDigits ( second ( time ) );
  Serial.print ( " " );
  Serial.print ( day ( time ));
  Serial.print ( "." );
  Serial.print ( month ( time ) );
  Serial.print ( "." );
  Serial.print ( year ( time ) ); 
  Serial.println (); 
  
}



void printDigits ( int digits ) {
    
  // utility for digital clock display: prints preceding colon and leading 0
  Serial.print ( ":" );
  if ( digits < 10 ) Serial.print ( '0' );
  Serial.print ( digits );
  
}

// ------------------------------------------------------------------------------------------------------------------------------------------

static char currentWeatherTextCache [200] = { 0 };
long lastWeatherHttpRequest = 0L;
const long WEATHER_REQUEST_PERIOD = 1L * 60L * 60L * 1000L;

char* getCurrentWeatherText () {
    
    if ( strlen ( currentWeatherTextCache ) == 0 || lastWeatherHttpRequest + WEATHER_REQUEST_PERIOD < millis () ) {
        
        Serial.println ( "request new current weather report" );
        
        // const char* httpResponse = http_request ( "http://192.168.2.8/website/weather_test/current_weather_test.html" );
        const char* httpResponse = http_request ( 
                "http://api.openweathermap.org/data/2.5/weather?id=2804759&appid=29ab3e72b07b4dd4c3aca6db6ff3290d&lang=de&units=metric" );
        const char* httpResponseTest = "\
    {\
        \"coord\":{\"lon\":13.54,\"lat\":52.65},\
        \"weather\":[{\"id\":800,\"main\":\"Clear\",\"description\":\"clear sky\",\"icon\":\"01d\"}],\
        \"base\":\"stations\",\
        \"main\":{\"temp\":296.48,\"pressure\":1017,\"humidity\":52,\"temp_min\":294.15,\"temp_max\":303.15},\
        \"visibility\":10000,\
        \"wind\":{\"speed\":4.1,\"deg\":290},\
        \"clouds\":{\"all\":0},\
        \"dt\":1472891622,\
        \"sys\":{\"type\":1,\"id\":4892,\"message\":0.0347,\"country\":\"DE\",\"sunrise\":1472876456,\"sunset\":1472924870},\
        \"id\":2804759,\
        \"name\":\"Zepernick\",\
        \"cod\":200\
    }\
    ";
        // DynamicJsonBuffer jsonBuffer;
        StaticJsonBuffer<2000> jsonBuffer;
        JsonObject& jsonRoot = jsonBuffer.parseObject ( httpResponse );
        if ( jsonRoot.success () ) {
            const char* temperature = jsonRoot ["main"]["temp"];
            const char* humidity = jsonRoot ["main"]["humidity"];
            const char* pressure = jsonRoot ["main"]["pressure"];
            snprintf ( currentWeatherTextCache, sizeof ( currentWeatherTextCache ), "%sC %s%% %shPa", temperature, humidity, pressure );
            Serial.printf ( "[WEATHER] temperature=%s humidity=%s pressure=%s\n", temperature, humidity, pressure );
        }
        else {
            snprintf ( currentWeatherTextCache, sizeof ( currentWeatherTextCache ), "error parsing json response" );
            Serial.println ( "[WEATHER] parseObject () failed") ;
        }
        
        lastWeatherHttpRequest = millis ();
        
    }
    
    return currentWeatherTextCache;

}

// ------------------------------------------------------------------------------------------------------------------------------------------

boolean led_state;

const int DAY_TIME_PERIOD = 10;
int display_counter = 100;
int display_type = 0;
const int MAX_DISPLAY_TYPE = 3;

boolean show_time = false;

void loop() {
    
    server.handleClient();

    current_time = now ();
    // tick every second
    if ( second ( current_time ) != second ( last_time ) ) {

        led_state = !led_state;
        digitalWrite ( LED_PIN, led_state );
        
        if ( text_duration < 0 || text_duration > 0 && textDisplayBegin + text_duration * 1000L < millis () ) {
            
            if ( show_time ) matrix_show_time ( current_time ); // toggle the colon
        
            if ( ++display_counter >= DAY_TIME_PERIOD ) {
                display_counter = 0;
                display_type++;
                if ( display_type > MAX_DISPLAY_TYPE ) display_type = 0;
                clear_matrix ();
                m.init ();
                show_time = false;
                switch ( display_type ) {
                    case 0: // time
                        matrix_show_time ( current_time );
                        show_time = true;
                        break;
                    case 1: // date
                        matrix_show_date ( current_time );
                        break;
                    case 2: // day of week
                        // matrix_show_day_of_week ( current_time );
                        insert_column = 13;
                        matrix_append_text ( dayStr ( weekday ( current_time ) ) );
                        break;
                    case 3: // current weather
                        insert_column = 0;
                        matrix_append_text ( getCurrentWeatherText () );
                        text_duration = -1;
                        break;
                    default:
                        display_counter = 0;
                        break;
                }

            }
            
        }
        
        last_time = current_time;
        
    }
      
}

// ------------------------------------------------------------------------------------------------------------------------------------------

