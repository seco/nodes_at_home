--------------------------------------------------------------------
--
-- nodes@home/luaNodes/sonoffNode
-- author: andreas at jungierek dot de
-- LICENSE http://opensource.org/licenses/MIT
--
--------------------------------------------------------------------
-- junand 30.10.2016

local moduleName = ...;
local M = {};
_G [moduleName] = M;

-------------------------------------------------------------------------------
--  Settings

local debounceTimer = espConfig.node.timer.debounce;
local debounceDelay = espConfig.node.timer.debounceDelay;

local baseTopic = espConfig.node.topic;

local relayPin = espConfig.node.appCfg.relayPin;
local ledPin = espConfig.node.appCfg.ledPin;
local buttonPin = espConfig.node.appCfg.buttonPin;

----------------------------------------------------------------------------------------
-- private

local function publishState ( client, topic, state )

    print ( "[APP] publish state=", state, "to", topic );
    
    client:publish ( topic .. "/value/state", state, 0, 1, function () end ); -- qos, retain

end

local function changeState ( client, topic, payload )

    gpio.write ( ledPin, payload == "ON" and gpio.LOW or gpio.HIGH );
    gpio.write ( relayPin, payload == "ON" and gpio.HIGH or gpio.LOW );
    publishState ( client, topic, payload );

end

local function flashLed ( times )

    gpio.serout ( ledPin, 0, { 50 * 1000, 200 * 1000 }, times, function () end ); -- async, ledPin is inverted
 
 end

--------------------------------------------------------------------
-- public
-- mqtt callbacks

function M.connect ( client, topic )

    print ( "[APP] connected with topic=", topic );
    
    flashLed ( 2 );
    publishState ( client, baseTopic, "OFF" ); -- default
    
    gpio.trig ( buttonPin, "up",
        function ( level )
            tmr.alarm ( debounceTimer, debounceDelay, tmr.ALARM_SINGLE,  -- timer_id, interval_ms, mode
                function ()
                    local state = gpio.read ( relayPin );
                    print ( "state=", state );
                    changeState ( client, topic .. "/lamp", state == 0 and "ON" or "OFF" );
                end
            );
        end 
    );

end

function M.message ( client, topic, payload )

    print ( "[APP] message: topic=", topic, " payload=", payload );
    
    local topicParts = util.splitTopic ( topic );
    local device = topicParts [#topicParts];
    
    if ( device == "lamp" ) then
        if ( payload == "ON" or payload == "OFF" ) then
            changeState ( client, topic, payload ); 
        end
    end

end

function M.offline ( client )

    print ( "[APP] offline" );
    
    return true; -- restart mqtt connection
    
end

-------------------------------------------------------------------------------
-- main

print ( "[MODULE] loaded", moduleName )

gpio.mode ( ledPin, gpio.OUTPUT );
flashLed ( 3 );

gpio.mode ( relayPin, gpio.OUTPUT );
gpio.write ( relayPin, gpio.LOW );

gpio.mode ( buttonPin, gpio.INT, gpio.PULLUP );

return M;

-------------------------------------------------------------------------------