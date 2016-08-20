// junand 20.08.2016
// Übernahme aus altem Program für text_node
// Zielplattform Arduino Pro Mini 5V/18MHz

// TODO: sprites nach oben begrenzen

#include "MaxMatrix.h"
#include <avr/pgmspace.h>

#include <Streaming.h>
#include <TimerOne.h>

#include "Sprites.h"
#include "MySerial.h"

const int data = 11;      // grün DIN pin of MAX7219 module
const int load = 12;      // gelb CS pin of MAX7219 module
const int clock = 13;     // schwarz CLK pin of MAX7219 module

const int maxInUse = 8;    //change this variable to set how many MAX7219's you'll use

MaxMatrix m ( data, load, clock, maxInUse ); // define module

// erste Spalte zum Anzeigen, die ersten 64 Pixelspalten werden leer gelassen 
int insertCol = NUM_DISPLAY_COLS;

volatile int displayCol = 0;
volatile int displayRow = 0;

// ISR, wird alle 50 µs gerufen, nach 20.000 Ticks ist dann eine Sekunde vorbei
#define DEFAULT_DELAY 3000 // 20.000 ist 1sec, 2.000 ist 1/10 sec
volatile int shift_delay = DEFAULT_DELAY;
int counter = 0; // hier werden die Ticks gezählt
//The ASCII buffer for recieving from the serial:
#define MAX_CMD_SIZE 96
#define BUFSIZE 4

static char cmdbuffer [BUFSIZE][MAX_CMD_SIZE];
static int bufindr = 0;
static int bufindw = 0;
static int buflen = 0;
static char serial_char;
static int serial_count = 0;
static boolean comment_mode = false;
static char *strchr_pointer; // just a pointer to find chars in the cmd string like X, Y, Z, E, etc

//==============================================================

extern "C"{

    extern unsigned int __bss_end;
    extern unsigned int __heap_start;
    extern void *__brkval;

    int freeMemory () {

        int free_memory;
        if ( (int)__brkval == 0 )
            free_memory = ((int) &free_memory) - ((int) &__bss_end);
        else
            free_memory = ((int) &free_memory) - ((int) __brkval);

        return free_memory;
        
    }

}

//==============================================================

void clearMatrix () {

    m.clear ();
    insertCol = NUM_DISPLAY_COLS;
    displayCol = 0;
    displayRow = 0;

}

//==============================================================

enum ENUM_DIR { NONE, HOR, VER };
typedef ENUM_DIR DIR;

typedef struct _Command Command;

struct _Command {

    int id;
    DIR direction;
    int from;
    int to;
    int delay;
    int nextCommandId;
    Command *nextCommand;

    int d;
    boolean running;

};

const int COMMAND_NUM = 20;
Command commands [COMMAND_NUM];
Command *cmd;
int cmdIndex = 0;

void clearCommand ( int i ){

    commands [i].id = -1;
    commands [i].direction = NONE;
    commands [i].from = -1;
    commands [i].to = -1;
    commands [i].delay = -1;
    commands [i].nextCommandId = -1;
    commands [i].nextCommand = NULL;
    commands [i].d = 0;
    commands [i].running = false;

}

struct _Command *findCommand ( int id ) {

    Command *result = NULL;

    for ( int i = 0; i < COMMAND_NUM; i++ ) {
        if ( commands [i].id == id ) {
            result = &commands [i];
        }
    }

    return result;

}

//==============================================================

void doInterrupt () {

    volatile int* val;

    counter++;
    if ( counter > shift_delay ) {

        counter = 0;

        m.shiftOutMatrix ( displayCol, displayRow );

        if ( cmd != NULL ) {
            
            if ( cmd->direction == HOR ) val = &displayCol;
            else if ( cmd->direction == VER ) val = &displayRow;

            if ( cmd->running ) {
                if ( *val == cmd->to ) {
                    setNextCommand ();
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

void setNextCommand () {

    cmd->running = false;
    // displayCol = 0;
    // displayRow = 0;
    
    if ( cmd->nextCommandId > -1 ) {
        if ( cmd->nextCommand == NULL ) {
            cmd->nextCommand = findCommand ( cmd->nextCommandId );
        }
        cmd = cmd->nextCommand;
        cmd->running = false; // damit sich from initialisiert
    }

}

//==============================================================

void setup () {

    m.init (); // module initialize
    m.setIntensity ( 0x0 ); // dot matix intensity 0-15
    clearMatrix ();

    MYSERIAL.begin ( BAUDRATE );

    SERIAL_PROTOCOLLNPGM ( "start" );

    if ( true ) {
        SERIAL_ECHO_START;
        SERIAL_ECHOPGM ( "MSG_FREE_MEMORY " );
        SERIAL_ECHOLN ( freeMemory () );
    }
    
    Timer1.initialize ( 50 ); // 50 µs, prescale is set by lib
    Timer1.attachInterrupt ( doInterrupt );

    cmd = NULL;

}

//==============================================================

void loop () {

    if ( buflen < (BUFSIZE - 1) ) {

        get_command();

        if ( buflen ) {
            process_commands ();
            buflen = buflen - 1;
            bufindr = (bufindr + 1) % BUFSIZE;
        }
        
    }

}

//==============================================================

void get_command () {

    while ( MYSERIAL.available () > 0  && buflen < BUFSIZE ) {

        serial_char = MYSERIAL.read ();

        if ( serial_char == '\n' || serial_char == '\r' ||
             (serial_char == ':' && comment_mode == false) ||
             serial_count >= (MAX_CMD_SIZE - 1) 
        ) {

            if ( !serial_count ) { //if empty line
                comment_mode = false; //for new command
                return;
            }

            cmdbuffer [bufindw][serial_count] = 0; //terminate string

            if ( !comment_mode ) {
                bufindw = (bufindw + 1) % BUFSIZE;
                buflen += 1;
            }
            serial_count = 0; //clear buffer

        }
        else {

            if ( serial_char == ';' ) comment_mode = true;
            if ( !comment_mode ) cmdbuffer [bufindw][serial_count++] = serial_char;

        }

    }

}

float code_value () {

    return strtod ( &cmdbuffer [bufindr][strchr_pointer - cmdbuffer [bufindr] + 1], NULL );

}

long code_value_long () {

    return strtol ( &cmdbuffer [bufindr][strchr_pointer - cmdbuffer [bufindr] + 1], NULL, 10 );

}

bool code_seen ( char code ) {

    strchr_pointer = strchr ( cmdbuffer [bufindr], code );
    return strchr_pointer != NULL;  //Return True if a character was found

}

byte buffer [10];

void process_commands () {

unsigned long codenum; //throw away variable
char *starpos = NULL;

if ( code_seen ( 'X' ) ) {

    switch ( (int) code_value () ) {

        case 0: // X0 Text, set/reset text at given insert column
            if ( code_seen ( 'T' ) ) {
                while ( char c = *++strchr_pointer ) { // up to end of line
                    if ( insertCol < NUM_BUF_COLS ) { // etwas ungenau
                        if ( c >= 32 ) {
                            c -= 32;
                            if ( c < 95 ) {
                                memcpy_P ( buffer, SPRITE + 7*c, 7);
                                m.printSprite ( insertCol, buffer );
                                insertCol += buffer [0];
                                m.printEmptyCol ( insertCol++ );
                            }
                        }
                    }
                }

                m.shiftOutMatrix ( displayCol, displayRow );

            }
            else {
                // no 'T' means reset of displayed text
                clearMatrix ();
            }
            break;

        case 1: // X1 Insert Row Column, set variables
        
            cmd = NULL;
            
            if ( code_seen ( 'I' ) ) {
                insertCol = code_value ();
            }
            if ( code_seen ( 'C' ) ) {
                displayCol = code_value ();
            }
            if ( code_seen ( 'R' ) ) {
                displayRow = code_value ();
            }
            break;

        case 10: // X10 Id Next From To Delay -> Horizontale Move
            clearCommand ( cmdIndex );
            commands [cmdIndex].direction = HOR;
            commands [cmdIndex].from = 0;
            commands [cmdIndex].to = NUM_BUF_COLS;
            commands [cmdIndex].delay = DEFAULT_DELAY;

            if ( code_seen ( 'I' ) ) {
                commands [cmdIndex].id = code_value ();
            }
            if ( code_seen ( 'N' ) ) {
                commands [cmdIndex].nextCommandId = code_value ();
            }
            if ( code_seen ( 'F' ) ) {
                commands [cmdIndex].from = code_value ();
            }
            if ( code_seen ( 'T' ) ) {
                commands [cmdIndex].to = code_value ();
            }
            if ( code_seen ( 'D' ) ) {
                commands [cmdIndex].delay = code_value ();
            }
            if ( commands [cmdIndex].from < commands [cmdIndex].to ) {
                commands [cmdIndex].d = 1;
            }
            else {
                commands [cmdIndex].d = -1;
            }
            cmdIndex++;
            break;

        case 20: // X20 Id Next From To -> Verticale Move
            clearCommand ( cmdIndex );
            commands [cmdIndex].direction = VER;
            commands [cmdIndex].from = 0;
            commands [cmdIndex].to = 8;
            commands [cmdIndex].delay = DEFAULT_DELAY;

            if ( code_seen ( 'I' ) ) {
                commands [cmdIndex].id = code_value ();
            }
            if ( code_seen ( 'N' ) ) {
                commands [cmdIndex].nextCommandId = code_value ();
            }
            if ( code_seen ( 'F' ) ) {
                commands [cmdIndex].from = code_value ();
            }
            if ( code_seen ( 'T' ) ) {
                commands [cmdIndex].to = code_value ();
            }
            if ( code_seen ( 'D' ) ) {
                commands [cmdIndex].delay = code_value ();
            }
            if ( commands [cmdIndex].from < commands [cmdIndex].to ) {
                commands [cmdIndex].d = 1;
            }
            else {
                commands [cmdIndex].d = -1;
            }
            cmdIndex++;
            break;

        case 80: // X80 Id Start Command with Id
            if ( code_seen ( 'I' ) ) {
                int id = code_value ();
                cmd = findCommand ( id );
                cmd->running = false;
            }
            else {
                cmd = NULL;
                delay ( 1 );
            }
            break;

        case 81: // X81 Clear Command Buffer
            cmd = NULL;
            delay ( 1 );
            cmdIndex = 0;
            for ( int i = 0; i < COMMAND_NUM; i++ ) {
                clearCommand ( i );
            }
            break;

        case 90: // X90 Brightness Delay
            if ( code_seen ( 'B' ) ) {
                m.setIntensity ( (int) code_value () & 0x0F );
            }
            if ( code_seen ( 'D' ) ) {
                shift_delay = (int) code_value ();
            }
            break;

        case 99: // X99 display some vars

            Serial << "i=" << insertCol << " c=" << displayCol << " r=" << displayRow << " iCmd=" << cmdIndex << endl;
            
            for ( int i = 0; i < COMMAND_NUM; i++ ) {
                _Command c = commands [i];
                Serial << "command id=" << c.id << " direction=" << c.direction << " from=" << c.from << " to=" << c.to << " delay=" << c.delay << " nextCmd=" << c.nextCommandId << endl;
            }
            
            break;

            default:
            break;

    }

}
else {
    SERIAL_ECHO_START;
    SERIAL_ECHOPGM ( "MSG_UNKNOWN_COMMAND" );
    SERIAL_ECHO ( cmdbuffer [bufindr] );
    SERIAL_ECHOLNPGM ( "\"" );
}

}
