--------------------------------------------------------------------
--
-- nodes@home/luaNodes/mqttNode
-- author: andreas at jungierek dot de
-- LICENSE http://opensource.org/licenses/MIT
--
--------------------------------------------------------------------
-- junand 15.10.2016

local moduleName = ...;
local M = {};
_G [moduleName] = M;

require ( "mqtt" );
require ( "wifi" );
require ( "adc" );

-------------------------------------------------------------------------------
--  Settings

M.appNode = nil;    -- application callbacks
M.client = nil;     -- mqtt client

local TIMER_WIFI_LOOP = 1;
local TIMER_WIFI_PERIOD = 1; -- sec

local TIMER_VOLTAGE_LOOP = 2;
local TIMER_VOLTAGE_PERIOD = 15 * 60;

----------------------------------------------------------------------------------------
-- private

local function wifiLoop ()

    -- 0: STA_IDLE,
    -- 1: STA_CONNECTING,
    -- 2: STA_WRONGPWD,
    -- 3: STA_APNOTFOUND,
    -- 4: STA_FAIL,
    -- 5: STA_GOTIP.

    if ( wifi.sta.status () == wifi.STA_GOTIP ) then
    
        -- Stop the loop
        tmr.stop ( TIMER_WIFI_LOOP );

        print ( "[WIFI] dnsname=" .. wifi.sta.gethostname () );
        print ( "[WIFI] network=", wifi.sta.getip () );
        print ( "[WIFI] mac=" .. wifi.sta.getmac () );
    
        -- Setup MQTT client and events
        if ( M.client == nil ) then
            -- TODO bette maatClientName
            M.client = mqtt.Client ( wifi.sta.gethostname (), 120, "", "" ); -- ..., keep_alive_time, username, password
        end

        print ( "[MQTT] connecting to " .. espNode.config.mqttBroker );

        -- this is never called, because the last registration wins
        -- M.client:on ( "connect", 
            -- function ( client )
                -- print ( "[MQTT] CONNECTED" );
                -- M.appNode.connect ();
            -- end
        -- );
            
        M.client:on ( "message", 
            function ( client, topic, payload )
                print ( "[MQTT] message received topic=" .. topic .." payload=" .. (payload == nil and "***nothing***" or payload) );
                M.appNode.message ( client, topic, payload );
            end
        );
            
        M.client:on ( "offline", 
            function ( client )
                print ( "[MQTT] offline" );
                M.appNode.offline ( client );
            end
        );
        
        -- TODO use a second timer for detecting not connecting situation, for example if the broker is  down
        -- local result = M.client:connect( espNode.config.mqttBroker , 1883, 0, 
        result = M.client:connect( "192.168.2.117" , 1883, 0, 
        
            function ( client )
            
                print ( "[MQTT] connected to MQTT Broker" )
                print ( "[MQTT} node=", espNode.config.topic );
                
                local topic = espNode.config.topic .. "/+";
                print ( "[MQTT] subscribe to topic=" .. topic );
                M.client:subscribe ( topic, 0 ); -- ..., qos
                print ( "[MQTT] send <" .. M.appNode.version .. "> to topic=" .. espNode.config.topic );
                M.client:publish ( espNode.config.topic, M.appNode.version, 0, 1 ); -- ..., qos, retain
                M.client:publish ( espNode.config.topic .. "/value/voltage", M.createJsonValueMessage ( adc.readvdd33 (), "mV" ), 0, 1 ); -- qos, retain
                M.appNode.connect ( client );
                
            end,

            function ( client, reason ) 
                print ( "[MQTT] not connected reason=", reason );
            end
        
        );
        M.client = M.client;
        print ( "[MQTT] connect result=", result );
    else
        print ( "[WIFI] Connecting..." );
    end
    
end

local function noop () end

local function initAppNode ( app )

    if ( app.version == nil ) then app.version = "undefined"; end
    if ( app.connect == nil ) then app.connect = noop; end
    if ( app.offline == nil ) then app.offline = noop; end
    if ( app.message == nil ) then app.message = noop; end
    
    M.appNode = app;
    
end

--------------------------------------------------------------------
-- public

function M.start ( app )

    if ( app ) then
        initAppNode ( app );
    else
        print ( "[MQTT] no app" );
        initAppNode ( {} );
    end
    
    -- loop to wait up to connected to wifi
    tmr.alarm ( TIMER_WIFI_LOOP, TIMER_WIFI_PERIOD * 1000, tmr.ALARM_AUTO, function () wifiLoop() end ) -- timer_id, interval_ms, mode
    tmr.alarm ( TIMER_VOLTAGE_LOOP, TIMER_VOLTAGE_PERIOD * 1000, tmr.ALARM_AUTO, -- timer_id, interval_ms, mode
        function () 
            print ( "[MQTT] send voltage" );
            M.client:publish ( espNode.config.topic .. "/voltage", M.createJsonValueMessage ( adc.readvdd33 (), "mV" ), 0, 1 ); -- qos, retain
        end 
    ) 

end
  
function M.createJsonValueMessage ( value, unit )

    return [[{"value":]] .. value .. [[, "unit":"]] .. unit .. [["}]];
    
end

-------------------------------------------------------------------------------
-- main

print ( "[MODULE] loaded", moduleName )

return M;

-------------------------------------------------------------------------------