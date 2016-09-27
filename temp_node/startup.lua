-- junand 22.09.2016
-- 27.09.2016 integrated some code from https://bigdanzblog.wordpress.com/2015/04/24/esp8266-nodemcu-interrupting-init-lua-during-boot/

ABORT_WAIT_TIME = 1 * 1000;
STARTUP_WAIT_TIME = 5 * 1000;

STARTUP_TIMER = 0;

APPLICATION = "temperature_node";

function abortInit()

    -- initailize abort boolean flag
    print ( "press ENTER to abort startup" );
    
    -- if <CR> is pressed, call abortTest
    uart.on ( "data", "\r", abortStartup, 0 );

    -- start timer to execute startup function in 5 seconds
    tmr.alarm ( STARTUP_TIMER, STARTUP_WAIT_TIME, tmr.ALARM_SINGLE, startup );

end
    
function abortStartup ( data )

    tmr.unregister ( STARTUP_TIMER );   -- disable the start up timer
    uart.on ( "data" );                 -- stop capturing the uart
    print ( "startup aborted" );
    
end
    
function startup ()

    uart.on ( "data" );                 -- stop capturing the uart

    print ( "application is starting" );
    dofile ( APPLICATION .. ".lua" );

end

print ( "startup esp, waiting for application start" );
-- tmr.alarm ( 0, STARTUP_WAIT_TIME, tmr.ALARM_SINGLE, startup )
tmr.alarm ( STARTUP_TIMER, ABORT_WAIT_TIME, tmr.ALARM_SINGLE, abortInit )

