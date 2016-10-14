-- junand 05.10.2016

----------------------------------------------------------------------------------------

ON_OFF_CODE = {

    test = { on = 474410, off = 474414}, -- 3/3
    
};

----------------------------------------------------------------------------------------

RF_PIN = 7;
LED_PIN = 4;

REPEATS = 8;
PERIOD = 329;

DELAY_TIMES = {
    [0] = {1*PERIOD, 3*PERIOD, 1*PERIOD, 3*PERIOD},
    [1] = {3*PERIOD, 1*PERIOD, 3*PERIOD, 1*PERIOD};
    [2] = {1*PERIOD, 3*PERIOD, 3*PERIOD, 1*PERIOD};
};

DELAY_TIMES_SYNC = {1*PERIOD, 31*PERIOD};

require ( "bit" );
require ( "gpio" );

----------------------------------------------------------------------------------------

function appendTable ( t1, t2 )

    for i = 1, #t2 do
        t1 [#t1+1] = t2 [i]
    end
    
    return t1
    
end

function sendCode ( code, callback )

    print ( "repeats=", REPEATS, "period=", PERIOD, "code=", code );

    local codeBase4 = 0;
    for i = 1, 12 do
        local mod = code % 3;
        code = (code - mod) / 3;
        codeBase4 = bit.lshift ( codeBase4, 2 );
        codeBase4 = bit.bor ( codeBase4, mod );
        -- print ( code );
    end
    
    --      (1) put delayTimes for all trits into one array and then fire en asyncronous serout
    --      (2) resuse table from (1)
    --      (3) if so, we can use repeat from serout

    local delayTimes = {};
    local data = codeBase4;
    for i = 1, 12 do
        local trit = bit.band ( data, 2 ); -- B11
        local tritDelayTimes = DELAY_TIMES [trit];
        appendTable ( delayTimes, tritDelayTimes );
        data = bit.arshift ( data, 2 ); -- next trit
    end
    appendTable ( delayTimes , DELAY_TIMES_SYNC );

    gpio.serout ( RF_PIN, 1, delayTimes, REPEATS, callback ); -- asynchron
        
end

function sendCodeOld ( repeats, periodusec, code )

    print ( "repeats=", repeats, "period=", periodusec, "code=", code );

    local codeBase4 = 0;
    for i = 1, 12 do
        local mod = code % 3;
        code = (code - mod) / 3;
        codeBase4 = bit.lshift ( codeBase4, 2 );
        codeBase4 = bit.bor ( codeBase4, mod );
        -- print ( code );
    end
    
    
    for j = 1, repeats do
    
        local line = "";
        
        local data = codeBase4;
        
        for i = 1, 12 do
        
            local trit = bit.band ( data, 2 ); -- B11
            
            -- print ( "data=", data, "trit=", trit );
            
            -- if ( j == 1 ) then
                -- line = line .. trit .. " ";
            -- end
            
            local delayTimes = nil;
            
            if ( trit == 0 ) then -- B00
                delayTimes = {1*periodusec, 3*periodusec, 1*periodusec, 3*periodusec};
            elseif ( trit == 1 ) then -- B01
                delayTimes = {3*periodusec, 1*periodusec, 3*periodusec, 1*periodusec};
            elseif ( trit == 2 ) then -- B11
                delayTimes = {1*periodusec, 3*periodusec, 3*periodusec, 1*periodusec};
            end

            -- pin, startlevel, delays us, repeats, callback
            -- presence of callback means asynchronous call
            -- no longer then 15 ms
            -- gpio.serout ( RF_PIN, 1, {1*periodusec, 3*periodusec, 1*periodusec, 3*periodusec}, 1, function () print ( trit ) end );
            gpio.serout ( RF_PIN, 1, delayTimes ); -- synchron
            
            data = bit.arshift ( data, 2 ); -- next trit
            
        end
        
        -- send termination/synchronisation-signal. Total length: 32 periods
        -- pin, startlevel, delays us, repeats, callback
        -- gpio.serout ( RF_PIN, 1, {1*periodusec, 31*periodusec}, 1, function () print ( "------------" ) end );
        gpio.serout ( RF_PIN, 1, {1*periodusec, 31*periodusec} );
        
        -- if ( j == 1 ) then
            -- print ( line );
        -- end

    end
    
end

----------------------------------------------------------------------------------------

state = true;

function loop ()

    state = not state;
    print ( "state=", state );
    gpio.write ( LED_PIN, state and gpio.HIGH or gpio.LOW );
    sendCode ( state and ON_OFF_CODE.test.on or ON_OFF_CODE.test.off, function () print ( "+++" ) end );

end

----------------------------------------------------------------------------------------

print ( "start" );

gpio.mode ( RF_PIN, gpio.OUTPUT );
gpio.mode ( LED_PIN, gpio.OUTPUT );

tmr.alarm ( 0, 3 * 1000, tmr.ALARM_AUTO, function () loop() end ) -- timer_id, interval_ms, mode

print ( "end" );

----------------------------------------------------------------------------------------
