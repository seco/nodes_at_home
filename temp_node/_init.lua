-- junand 22.09.2016

STARTUP_WAIT_TIME = 5 * 1000;
APPLICATION = "temperature_node";

print ( "startup esp, waiting for application start" );
tmr.alarm ( 0, STARTUP_WAIT_TIME, tmr.ALARM_SINGLE, 
    function ()
        print ( "application is starting" );
        dofile ( APPLICATION .. ".lua" );
    end
)

