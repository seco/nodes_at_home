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

require ( "dht" );

-------------------------------------------------------------------------------
--  Settings

M.version = "V0.10 (tempNode)";

local TIME_BETWEEN_SENSOR_READINGS = 15 * 60;     -- sec

-- DHT11
local DHT_PIN = 4; -- nodemcu D4, GIO2

----------------------------------------------------------------------------------------
-- private

-- DHT11 sensor logic

function getSensorData ()

    local dht = require ( "dht" );
    
    local status, temperature, humidity, temp_decimial, humi_decimial = dht.read ( DHT_PIN );
    
    if( status == dht.OK ) then

        print ( "[DHT] Temperature: "..temperature.." C" );
        print ( "[DHT] Humidity: "..humidity.."%" );
        
    elseif( status == dht.ERROR_CHECKSUM ) then
    
        print ( "[DHT] Checksum error" );
        temperature = -1;
        humidity = 0;
        
    elseif( status == dht.ERROR_TIMEOUT ) then
    
        print ( "[DHT] Time out" );
        temperature = -2;
        humidity = 0;
        
    end
    
    -- Release module
    dht = nil;
    package.loaded [ "dht" ] = nil;
    
    return temperature, humidity;
    
end

local function createJsonValueMessage ( value, unit )

    return [[{"value":]] .. value .. [[, "unit":"]] .. unit .. [["}]];
    
end

-------------------------------------------------------------------------------
-- mqtt callbacks

local function connect ( client, baseTopic )

    print ( "[APP] connect" );
    
    local t, h = getSensorData();

    print ( "[APP] publish temperature t=", t );
    client:publish ( baseTopic .. "/value/temperature", createJsonValueMessage ( t, "C" ), 0, 1, -- qos, retain
        function ( client )
            print ( "[APP] publish temperature h=", h );
            client:publish ( baseTopic .. "/value/humidity", createJsonValueMessage ( h, "%" ), 0, 1, -- qos, retain
                function ( client )
                    -- wait a minute with closing connection
                    print ( "[APP] initiate alarm for closing connection" );
                    tmr.alarm ( 3, 60 * 1000, tmr.ALARM_SINGLE,  -- timer_id, interval_ms, mode
                        function () 
                            print ( "[APP] closing connection" );
                            client:close ();
                            print ( "[APP] going to deep sleep" );
                            node.dsleep ( (TIME_BETWEEN_SENSOR_READINGS - 60) * 1000 * 1000 );
                            -- node.dsleep ( (90 - 60) * 1000 * 1000 );
                        end
                    );
                end
            );
        end
    );
    
end

local function offline ( client )

    print ( "[APP] offline" );

    print ( "[APP] Going to deep sleep for "..(TIME_BETWEEN_SENSOR_READINGS ).." seconds" );
    node.dsleep ( TIME_BETWEEN_SENSOR_READINGS * 1000 * 1000 );
    
    return false; -- dont restart mqtt connection
    
end

local function message ( client, topic, payload )

    print ( "[APP] message: topic=", topic, " payload=", payload );

end

--------------------------------------------------------------------
-- public

-------------------------------------------------------------------------------
-- main

print ( "[MODULE] loaded", moduleName )
print ( "heap=", node.heap () );

M.connect = connect;
-- M.offline = offline;
-- M.message = message;

return M;

-------------------------------------------------------------------------------