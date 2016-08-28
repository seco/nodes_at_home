// junand 16.07.2016

// ------------------------------------------------------------------------------------------------------------------------------------------

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <PrintEx.h>
#include <Ticker.h>
#include <Time.h>

#include "MaxMatrix.h"
// #include <avr/pgmspace.h>
#include "Sprites.h"

using namespace ios;

#define AT_HOME
#include "WLAN.h"
//const char* ssid     = "your_ssid";
//const char* password = "your_password";

#include "http_code.h"

// ------------------------------------------------------------------------------------------------------------------------------------------

const int DATA_PIN = 14;      // grün DIN pin of MAX7219 module     -> D5
const int LOAD_PIN = 12;      // gelb CS pin of MAX7219 module      -> D6
const int CLOCK_PIN = 13;     // schwarz CLK pin of MAX7219 module  -> D7

const int MAX_IN_USE = 8;    //change this variable to set how many MAX7219's you'll use

MaxMatrix m ( DATA_PIN, LOAD_PIN, CLOCK_PIN, MAX_IN_USE ); // define module

// erste Spalte zum Anzeigen, die ersten 64 Pixelspalten werden leer gelassen 
int insert_column = 0;

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

const int LED_PIN = 5;

void setup () {

    m.init (); // module initialize
    m.setIntensity ( 0x0 ); // dot matix intensity 0-15
    clear_matrix ();

    Serial.begin ( 9600 );
    delay ( 2 );
    Serial.println ( "start" ); // first serial print!
    
    Serial.print ( "ä=" );
    Serial.println ( 'ä', HEX );
    Serial.print ( "ö=" );
    Serial.println ( 'ö', HEX );
    Serial.print ( "ü=" );
    Serial.println ( 'ü', HEX );
    
    // Timer1.initialize ( 50 ); // 50 µs, prescale is set by lib
    // Timer1.attachInterrupt ( doInterrupt );
    ticker.attach_ms ( TICKER_RESOLUTION, do_interrupt );

    cmd = NULL; 

    pinMode ( LED_PIN, OUTPUT );
    digitalWrite ( LED_PIN, HIGH );

    WiFi.mode ( WIFI_STA );
    WiFi.begin(ssid, password);
    
    while ( WiFi.status() != WL_CONNECTED ) {
        delay ( 500 );
        matrix_append_text ( "." );
        Serial.print ( '.' );
    }
    matrix_append_text ( "connected" );
    Serial << "connected" << endl;
    
    server.on ( "/", handle_root );
    server.on ( "/init", handle_init );
    server.on ( "/clear", handle_clear );
    server.on ( "/display", handle_display );
	server.onNotFound ( handle_not_found );

    digitalWrite ( LED_PIN, HIGH );
    
    // clear_matrix ();
    // String ip = WiFi.localIP ().toString ();
    // String msg = " and server started with ip " + ip ;
    matrix_append_text ( " with ip " );
    matrix_append_text ( WiFi.localIP ().toString ().c_str () );
    // Serial << " with ip " << WiFi.localIP ().toString () << endl;
    Serial << " with ip ";
    Serial.println ( WiFi.localIP () );
    Serial << "inserCol=" << insert_column << endl;
    
    server.begin();
  
    digitalWrite ( LED_PIN, HIGH );

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
    <p>IP-Address: %s</p>\
    <p>Channel   : %d</p>\
  </body>\
</html>";

    const int buf_len = 400;
	char buf [buf_len];
    
	int sec = millis() / 1000;
	int min = sec / 60;
	int hr = min / 60;
    
	snprintf ( buf, buf_len, html_template, hr, min % 60, sec % 60, WiFi.localIP ().toString ().c_str (), WiFi.channel () );
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
            
            const char *str = text.c_str ();
            while ( *str ) {
                Serial.print ( *str );
                Serial.print ( " -> " );
                Serial.print ( *str, HEX );
                Serial.println ();
                str++;
            }
        }
        else if ( server.hasArg ( "ticks" ) ) {
            String ticks_str = server.arg ( "ticks" );
            int ticks = atoi ( ticks_str.c_str () );
            Serial << "received ticks=" << ticks_str << endl;
            shift_delay = ticks;
        }
        else if ( server.hasArg ( "brightness" ) ) {
            String brightness_str = server.arg ( "brightness" );
            int intensity = atoi ( brightness_str.c_str () );
            Serial << "received intensity=" << brightness_str << endl;
            m.setIntensity ( intensity );
        }

        server.send ( HTTP_CODE_OK, "text/plain", "text displayed" );

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
        
        clear_all_commands ();
        int col1 = 0;
        int col2 = insert_column - NUM_DISPLAY_COLS;
        init_command ( 1, HORIZONTAL_DIRECTION, col1, col2, 2 );
        init_command ( 2, HORIZONTAL_DIRECTION, col2, col1, 1 );
        
        start_command ( 1 );
        
        cmd_index++;

    }

}

// ------------------------------------------------------------------------------------------------------------------------------------------

const int PAUSE1 = 1000;
const int PAUSE2 = PAUSE1 + 1000;
long last_timestamp;

void loop() {
    
    server.handleClient();

    long timestamp = millis ();
    if ( timestamp > last_timestamp + PAUSE2 ) {
        digitalWrite ( LED_PIN, LOW );
        last_timestamp = timestamp;
    }
    else if ( timestamp > last_timestamp + PAUSE1 ) {
        digitalWrite ( LED_PIN, HIGH );
    }
    
}

// ------------------------------------------------------------------------------------------------------------------------------------------

