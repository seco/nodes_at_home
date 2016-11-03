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

require  ( "util" );

-------------------------------------------------------------------------------
--  Settings

----------------------------------------------------------------------------------------
-- private

--------------------------------------------------------------------
-- public
-- mqtt callbacks

function M.connect ( client, baseTopic )

    print ( "[APP] connect" );
    
    local t, h = util.getSensorData ( espConfig.node.appCfg.dhtPin );

    print ( "[APP] publish temperature t=", t );
    client:publish ( baseTopic .. "/value/temperature", util.createJsonValueMessage ( t, "C" ), 0, 1, -- qos, retain
        function ( client )
            print ( "[APP] publish humidity h=", h );
            client:publish ( baseTopic .. "/value/humidity", util.createJsonValueMessage ( h, "%" ), 0, 1, -- qos, retain
                function ( client )
                    -- wait a minute with closing connection
                    if ( not espConfig.node.appCfg.useOfflineCallback ) then
                        print ( "[APP] initiate alarm for closing connection in " ..  espConfig.node.timer.deepSleepDelay/1000 .. " seconds" );
                        tmr.alarm ( espConfig.node.timer.deepSleep, espConfig.node.timer.deepSleepDelay, tmr.ALARM_SINGLE,  -- timer_id, interval_ms, mode
                            function () 
                                print ( "[APP] closing connection" );
                                client:close ();
                                print ( "[APP] Going to deep sleep for ".. espConfig.node.appCfg.timeBetweenSensorReadings/1000 .." seconds" );
                                node.dsleep ( (espConfig.node.appCfg.timeBetweenSensorReadings - espConfig.node.timer.deepSleepDelay) * 1000 ); -- us
                                -- node.dsleep ( (90 - 60) * 1000 * 1000 );
                            end
                        );
                    else
                        print ( "[APP] closing connection using offline handler" );
                        client:close ();
                    end
                end
            );
        end
    );

end

local function offline ( client )

    print ( "[APP] offline" );

    print ( "[APP] initiate alarm for closing connection in " ..  espConfig.node.timer.deepSleepDelay/1000 .. " seconds" );
    node.dsleep ( espConfig.node.appCfg.timeBetweenSensorReadings * 1000 ); -- us
    
    return false; -- dont restart mqtt connection
    
end

local function message ( client, topic, payload )

    print ( "[APP] message: topic=", topic, " payload=", payload );

end

-------------------------------------------------------------------------------
-- main

print ( "[MODULE] loaded", moduleName )

if ( espConfig.node.appCfg.useOfflineCallback ) then
    M.offline = offline;
end
-- M.message = message;

return M;

-------------------------------------------------------------------------------