--------------------------------------------------------------------
--
-- nodes@home/rf_node/startup
-- author: andreas at jungierek dot de
-- LICENSE http://opensource.org/licenses/MIT
--
--------------------------------------------------------------------
-- junand 22.09.2016
-- 27.09.2016 integrated some code from https://bigdanzblog.wordpress.com/2015/04/24/esp8266-nodemcu-interrupting-init-lua-during-boot/

local ABORT_WAIT_TIME = 100;
local STARTUP_WAIT_TIME = 5 * 1000;

local STARTUP_TIMER = 0;

require ( "espNode" );

function abortInit()

    -- initialize abort boolean flag
    print ( "[STARTUP] press ENTER to abort" );
    
    -- if <CR> is pressed, call abortStartup
    uart.on ( "data", "\r", abortStartup, 0 );

    -- start timer to execute startup function in 5 seconds
    tmr.alarm ( STARTUP_TIMER, STARTUP_WAIT_TIME, tmr.ALARM_SINGLE, startup );

end
    
function abortStartup ( data )

    tmr.unregister ( STARTUP_TIMER );   -- disable the start up timer
    uart.on ( "data" );                 -- stop capturing the uart
    print ( "[STARTUP] aborted" );
    
end
    
function startup ()

    uart.on ( "data" );                 -- stop capturing the uart

    print ( "[STARTUP] application " .. espNode.config.app .. " is starting" );
    dofile ( espNode.config.app .. ".lua" );

end

if ( adc.force_init_mode ( adc.INIT_VDD33 ) ) then
  node.restart ();
  return; -- don't bother continuing, the restart is scheduled
end

print ( "[STARTUP] waiting for application start" );
tmr.alarm ( STARTUP_TIMER, ABORT_WAIT_TIME, tmr.ALARM_SINGLE, abortInit )

