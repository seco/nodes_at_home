-- junand 22.09.2016
-- 27.09.2016 integrated some code from https://bigdanzblog.wordpress.com/2015/04/24/esp8266-nodemcu-interrupting-init-lua-during-boot/

ABORT_WAIT_TIME = 100;
STARTUP_WAIT_TIME = 5 * 1000;

STARTUP_TIMER = 0;

APPLICATION = "temperature_node";

function abortInit()

    -- initailize abort boolean flag
    print ( "[INPUT] press ENTER to abort startup" );
    
    -- if <CR> is pressed, call abortTest
    uart.on ( "data", "\r", abortStartup, 0 );

    -- start timer to execute startup function in 5 seconds
    tmr.alarm ( STARTUP_TIMER, STARTUP_WAIT_TIME, tmr.ALARM_SINGLE, startup );

end
    
function abortStartup ( data )

    tmr.unregister ( STARTUP_TIMER );   -- disable the start up timer
    uart.on ( "data" );                 -- stop capturing the uart
    print ( "[SYSTEM] startup aborted" );
    
end
    
function startup ()

    uart.on ( "data" );                 -- stop capturing the uart

    print ( "[SYSTEM] application " .. APPLICATION .. " is starting" );
    dofile ( APPLICATION .. ".lua" );

end

if ( adc.force_init_mode ( adc.INIT_VDD33 ) ) then
  node.restart ();
  return; -- don't bother continuing, the restart is scheduled
end

print ( "[SYSTEM] startup esp, waiting for application start" );
-- tmr.alarm ( 0, STARTUP_WAIT_TIME, tmr.ALARM_SINGLE, startup )
tmr.alarm ( STARTUP_TIMER, ABORT_WAIT_TIME, tmr.ALARM_SINGLE, abortInit )

