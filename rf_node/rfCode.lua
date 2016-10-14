--------------------------------------------------------------------
--
-- nodes@home/rf_node/rfCode
-- author: andreas at jungierek dot de
-- LICENSE http://opensource.org/licenses/MIT
--
--------------------------------------------------------------------
-- junand 14.10.2016

local moduleName = ...;
local M = {};
_G [moduleName] = M;

require ( "bit" );
require ( "gpio" );

--------------------------------------------------------------------
-- vars

--------------------------------------------------------------------
-- private

function appendTable ( t1, t2 )

    for i = 1, #t2 do
        t1 [#t1+1] = t2 [i]
    end
    
    return t1
    
end

function sendCode ( code )

    print ( "[RF] repeats=", M.repeats, "period=", M.period, "code=", code );

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

    local period = M.period;
    local tritDelayTimes = {
        [0] = {1*period, 3*period, 1*period, 3*period},
        [1] = {3*period, 1*period, 3*period, 1*period};
        [2] = {1*period, 3*period, 3*period, 1*period};
    };

    local syncDelayTimes = {1*period, 31*period};

    local data = codeBase4;
    local delayTimes = {};
    for i = 1, 12 do
        local trit = bit.band ( data, 2 ); -- B11
        appendTable ( delayTimes, tritDelayTimes [trit] );
        data = bit.arshift ( data, 2 ); -- next trit
    end
    appendTable ( delayTimes , syncDelayTimes );

    gpio.serout ( M.pin, 1, delayTimes, M.repeats, 1 ); -- asynchron
    
end

function dequeue ()

    if ( #M.queue > 0 ) then
        local code = table.remove ( M.queue, 1 );
        print ( "[RF] loop: code=", code );
        sendCode ( code );
    end
    
end

--------------------------------------------------------------------
-- public

function M.init ( pin, repeats, period )

    if ( pin ) then M.pin = pin; end
    if ( repeats ) then M.repeats = repeats; end
    if ( period ) then M.period = period; end

end

function M.send ( device, state )

    -- print ( "[RF] device=", device, " state=", state );
    
    local codes = {

        -- Fernbedienung 1 - Kueche
        -- DIP 123!4!5
        lampKitchenLongStrip    = { ON = 18218, OFF = 18222 }, -- 1/A
        lampKitchenShortStrip   = { ON = 19190, OFF = 19194 }, -- 1/B
        noName1C                = { ON = 19514, OFF = 19518 }, -- 1/C
        noName1D                = { ON = 19622, OFF = 19626 }, -- 1/D
        
        -- Fernbedienung 2 - Wohnzimmer
        -- DIP 1!2!3!45
        lampStudyPillar         = { ON = 171308, OFF = 171312 }, -- 2/A
        lampLoungeWhiteBall     = { ON = 172280, OFF = 172284 }, -- 2/B
        lampLoungeBlackBowl     = { ON = 172604, OFF = 172608 }, -- 2/C
        lampLoungeReading       = { ON = 172712, OFF = 172716 }, -- 2/D
        
        -- Fernbedienung 3 - Terrarium
        -- DIP !1!2345
        noName3A                = { ON = 473114, OFF = 473118 }, -- 3/A
        noName3B                = { ON = 474086, OFF = 474090 }, -- 3/A
        lamp1                   = { ON = 474410, OFF = 474414 }, -- 3/A
        noName3D                = { ON = 474518, OFF = 474522 }, -- 3/A

        -- Fernbedienung 4 - Nina
        -- DIP !1234!5

    };

    local deviceCodes = codes [device];
    if ( deviceCodes ~= nil and (state == "ON" or state == "OFF" ) ) then
        local code = deviceCodes [state];
        gpio.write ( M.ledpin, state == "ON" and gpio.HIGH or gpio.LOW );
        print ( "[RF] send: code=", code );
        -- sendCode ( code );
        table.insert ( M.queue, code );
    end
    
end

--------------------------------------------------------------------

print ( "[MODULE] loaded", moduleName )

M.pin = 7;
M.repeats = 16;
M.period = 320;
M.ledpin = 4;

gpio.mode ( M.pin, gpio.OUTPUT );
gpio.write ( M.pin, gpio.LOW );

gpio.mode ( M.ledpin, gpio.OUTPUT );
gpio.write ( M.ledpin, gpio.LOW );

M.queue = {};
tmr.alarm ( 2, 500, tmr.ALARM_AUTO, function () dequeue () end ) -- timer_id, interval_ms, mode, callback

return M;

--------------------------------------------------------------------


