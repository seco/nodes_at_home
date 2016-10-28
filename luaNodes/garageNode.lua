--------------------------------------------------------------------
--
-- nodes@home/luaNodes/tempNode
-- author: andreas at jungierek dot de
-- LICENSE http://opensource.org/licenses/MIT
--
--------------------------------------------------------------------
-- junand 15.10.2016

local moduleName = ...;
local M = {};
_G [moduleName] = M;

require ( "util" );

-------------------------------------------------------------------------------
--  Settings

local triggerTimer = espConfig.node.timer.trigger;
local triggerDelay = espConfig.node.timer.triggerDelay;

local debounceTimer = espConfig.node.timer.debounce;
local debounceDelay = espConfig.node.timer.debounceDelay;

local stateTimer = espConfig.node.timer.state;
local statePeriod = espConfig.node.timer.statePeriod;

local position = 0; -- 0: closed, 1: in move up, 2: in move down, 3: stopped from move up, 4: stopped from move down, 5: fully open
local action;

local baseTopic = espConfig.node.topic;

local relayPin = espConfig.node.appCfg.relayPin;
local openPositionPin = espConfig.node.appCfg.openPositionPin;
local closedPositionPin = espConfig.node.appCfg.closePositionPin;
local dhtPin = espConfig.node.appCfg.dhtPin;

----------------------------------------------------------------------------------------
-- private

local function triggerCover ( callback )

    print ( "[APP] trigger", action );

    gpio.write ( relayPin, gpio.HIGH );
    -- TODO Nutzung gpio.serout
    tmr.alarm ( triggerTimer, triggerDelay, tmr.ALARM_SINGLE,  -- timer_id, interval_ms, mode
        function ()
            gpio.write ( relayPin, gpio.LOW );
            if ( callback ) then
                tmr.alarm ( triggerTimer, triggerDelay, tmr.ALARM_SINGLE,  -- timer_id, interval_ms, mode
                    function ()
                        callback ();
                    end
                );
            end
        end
    );
    
end

function triggerCoverSerout ( count )

    local delays = {
        [1] = { triggerDelay, triggerDelay },
        [2] = { triggerDelay, triggerDelay, triggerDelay, triggerDelay },
    };

    gpio.serout ( relayPin, 1, delays [count], 1, function () end ); 

end

local function publishState ( client, baseTopic, state )

    state = state == 0 and "closed" or state;
    state = state == 5 and "open" or state;
    print ( "[APP] publish state=", state );
    client:publish ( baseTopic .. "/value/position", state, 0, 1, function () end ); -- qos, retain

end

local function startSwitchTimer ( client, baseTopic )

    tmr.alarm ( stateTimer, statePeriod, tmr.ALARM_AUTO,  -- timer_id, interval_ms, mode
        function ()
            local openSwitch = gpio.read ( openPositionPin );
            local closeSwitch = gpio.read ( closedPositionPin );
            print ( "[APP] positions: open=", openSwitch, "closeSwitch=", closeSwitch );
            -- client:publish ( baseTopic .. "/value/position", util.createJsonValueMessage ( h, "%" ), 0, 1, function () end ); -- qos, retain
            if ( ( position == 0 or position == 4 ) and openSwitch == 1 and closeSwitch == 1 ) then -- just in move
                position = 1; -- move up
                print ( "[APP] new position=1 (move up)" );
                publishState ( client, baseTopic, position );
            elseif ( ( position == 5 or position == 3 ) and openSwitch == 1 and closeSwitch == 1 ) then -- just in move
                position = 2; -- move down
                print ( "[APP] new position=1 (move down)" );
                publishState ( client, baseTopic, position );
            elseif ( position == 2 and openSwitch == 1 and closeSwitch == 0 ) then -- just closed
                position = 0;
                tmr.stop ( stateTimer );
                print ( "[APP] new position=0 (closed)" );
                publishState ( client, baseTopic, position );
            elseif ( position == 1 and openSwitch == 0 and closeSwitch == 1 ) then -- just opened
                position = 5;
                tmr.stop ( stateTimer );
                print ( "[APP] new position=5 (opened)" );
                publishState ( client, baseTopic, position );
            end
        end
    );

end

--------------------------------------------------------------------
-- public

function M.connect ( client, topic )

    print ( "[APP] connected with topic=", topic );
    
    M.periodic ( client, topic );
    
end

function M.message ( client, topic, payload )

    print ( "[APP] message: topic=", topic, " payload=", payload );
    local topicParts = util.splitTopic ( topic );
    local command = topicParts [#topicParts];
    
    if ( command == "command" ) then
        -- OPEN only possible if door is in motion down (position == 2, 2x triggerCover) or if closed (position == 0, 1x triggerCover)
        -- STOP only possibly if door is in motion (position == 1 or 2)
        -- CLOSE only possible if door is in motion up (position == 1, 2x triggerCover) or if opened (position == 3, 1x triggerCover)
        action = payload;
        if ( action == "OPEN" ) then
            if ( position == 2 ) then
                print ( "move door action=", action, "position=", position );
                triggerCover (
                    function ()
                        triggerCover ();
                        position = 1;
                    end
                );
                print ( "[APP] new position=1 (move up)" );
                publishState ( client, baseTopic, position );
            elseif ( position == 0 or position == 4 ) then
                print ( "move door action=", action, "position=", position );
                triggerCover ();
                startSwitchTimer ( client, baseTopic );
            else
                print ( "[APP] forbidden action=", action, "for position=", position );
            end
        elseif ( action == "STOP" ) then
            if ( position == 1 ) then
                print ( "stop door action=", action, "position=", position );
                triggerCover ();
                tmr.stop ( stateTimer );
                position = 3;
                print ( "[APP] new position=3 (stopped from move up)" );
                publishState ( client, baseTopic, position );
            elseif ( position == 2 ) then
                print ( "stop door action=", action, "position=", position );
                triggerCover ();
                tmr.stop ( stateTimer );
                position = 4;
                print ( "[APP] new position=4 (stopped from move down)" );
                publishState ( client, baseTopic, position );
            else
                print ( "[APP] forbidden action=", action, "for position=", position );
            end
        elseif ( action == "CLOSE" ) then
            if ( position == 1 ) then
                print ( "move door action=", action, "position=", position );
                triggerCover (
                    function ()
                        triggerCover ();
                        position = 2;
                    end
                );
                print ( "[APP] new position=2 (move down)" );
                publishState ( client, baseTopic, position );
            elseif ( position == 5 or position == 3 ) then
                print ( "move door action=", action, "position=", position );
                triggerCover ();
                startSwitchTimer ( client, baseTopic );
            else
                print ( "[APP] forbidden action=", action, "for position=", position );
            end
        else
            print ( "[APP] unknown action=", action );
        end
    end

end

function M.periodic ( client, baseTopic )

    print ( "[APP] periodic call topic=", baseTopic );
    
    local t, h = util.getSensorData ( espConfig.node.appCfg.dhtPin );
    
    print ( "[APP] publish temperature t=", t );
    client:publish ( baseTopic .. "/value/temperature", util.createJsonValueMessage ( t, "C" ), 0, 1, -- qos, retain
        function ( client )
            print ( "[APP] publish humidity h=", h );
            client:publish ( baseTopic .. "/value/humidity", util.createJsonValueMessage ( h, "%" ), 0, 1, function () end ); -- qos, retain
        end
    );
    
end

-------------------------------------------------------------------------------
-- main

print ( "[MODULE] loaded", moduleName )

gpio.mode ( relayPin, gpio.OUTPUT );
gpio.write ( relayPin, gpio.LOW );

gpio.mode ( openPositionPin, gpio.INPUT, gpio.PULLUP );
gpio.mode ( closedPositionPin, gpio.INPUT, gpio.PULLUP );

return M;

-------------------------------------------------------------------------------