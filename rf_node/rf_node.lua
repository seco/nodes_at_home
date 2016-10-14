-- junand 03.10.2016

-------------------------------------------------------------------------------
--  Settings

VERSION = "V0.10";

-- modules
require ( "credential" );
require ( "espNode" );
package.loaded ["credential"] = nil;
package.loaded ["espNode"] = nil;
collectgarbage ();

require ( "rfCode" );

--- rf ---
local RF_PIN = 7;
local RF_REPEATS = 8;
local RF_PERIOD = 320;

--- mqtt ---
local MQTT_TOPIC_NODE = "nodes@home/" .. espNode.config.class .. "/" .. espNode.config.type .. "/" .. espNode.config.location;
local MQTT_TOPIC_VOLTAGE = MQTT_TOPIC_NODE .. "/voltage";
local MQTT_TOPIC_STATE = MQTT_TOPIC_NODE .. "/state";

--- WIFI ---
-- wifi.PHYMODE_B 802.11b, More range, Low Transfer rate, More current draw
-- wifi.PHYMODE_G 802.11g, Medium range, Medium transfer rate, Medium current draw
-- wifi.PHYMODE_N 802.11n, Least range, Fast transfer rate, Least current draw 
local WIFI_SIGNAL_MODE = wifi.PHYMODE_N;

local TIME_WIFI_WAIT_PERIOD = 1;                  -- sec
local TIMER_WIFI_LOOP = 1;

-- modules

require ( "cjson" );

print ( "heap=", node.heap () );

----------------------------------------------------------------------------------------
-- mqtt data

-- from: http://lua-users.org/wiki/SplitJoin
function split ( str, pat )

    local t = {};  -- NOTE: use {n = 0} in Lua-5.0
    local fpat = "(.-)" .. pat;
    local last_end = 1;
    local s, e, cap = str:find ( fpat, 1 );
    while s do
        if s ~= 1 or cap ~= "" then
            table.insert ( t, cap );
        end
        last_end = e + 1
        s, e, cap = str:find ( fpat, last_end );
    end
    if last_end <= #str then
        cap = str:sub ( last_end );
        table.insert ( t, cap );
    end
    
    return t;
   
end

function splitTopic ( str )

   return split ( str, '[\\/]+' );
   
end

-------------------------------------------------------------------------------
-- loop

function loop ()

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
        if ( mqttClient == nil ) then
            mqttClient = mqtt.Client ( wifi.sta.gethostname (), 120, "", "" ); -- ..., keep_alive_time, username, password
        end

        print ( "[MQTT] connecting to " .. espNode.config.mqttBroker );

        -- this is never called, because the last registration wins
        -- mqttClient:on ( "connect", 
            -- function ( client )
                -- print ( "[MQTT] CONNECTED" );
            -- end
        -- );
            
        mqttClient:on ( "message", 
            function ( client, topic, payload )
                print ( "[MQTT] message received topic=" .. topic .." payload=" .. (payload == nil and "***nothing***" or payload) );
                -- if ( payload ~= nil ) then
                if ( payload ) then
                    local topicParts = splitTopic ( topic );
                    local device = topicParts [#topicParts];
                    rfCode.send ( device, payload );
                    print ( "heap=", node.heap () );
                end
            end
        );
            
        -- mqttClient:on ( "offline", 
            -- function ( client )
                -- print ( "[MQTT] offline" );
            -- end
        -- );
        
        -- TODO use a second timer for detecting not connecting situation, for example if the broker is  down
        result = mqttClient:connect( espNode.config.mqttBroker , 1883, 0, 
        
            function ( client )
            
                print ( "[MQTT] connected to MQTT Broker" )
                print ( "[MQTT} node=", MQTT_TOPIC_NODE );
                
                local topic = MQTT_TOPIC_NODE .. "/+";
                print ( "[MQTT] subscribe to topic=" .. topic );
                mqttClient:subscribe ( topic, 0 ); -- ..., qos
                
                mqttClient:publish ( MQTT_TOPIC_NODE, espNode.config.class .. " " .. espNode.config.type .. " " .. VERSION, 0, 1 ); -- ..., qos, retain
                
            end,

            function ( client, reason ) 
                print ( "[MQTT] not connected reason=", reason );
            end
        
        );
        print ( "[MQTT] connect result=", result );
    else
        print ( "[WIFI] Connecting..." );
    end
    
end
  
-------------------------------------------------------------------------------
-- main

-- Connect to the wifi network
print ( "[WIFI] connecting to " .. credential.ssid );
wifi.setmode ( wifi.STATION );
wifi.setphymode ( WIFI_SIGNAL_MODE );
wifi.sta.config ( credential.ssid, credential.password );
wifi.sta.connect ();

if ( NODE_CONFIG ~= nil ) then
    print ( "[WIFI] fix ip=" .. NODE_CONFIG.wifi.ip );
    wifi.sta.setip ( NODE_CONFIG.wifi );
end
 
-- loop to wait up to connected to wifi
tmr.alarm ( TIMER_WIFI_LOOP, TIME_WIFI_WAIT_PERIOD * 1000, tmr.ALARM_AUTO, function () loop() end ) -- timer_id, interval_ms, mode

-------------------------------------------------------------------------------