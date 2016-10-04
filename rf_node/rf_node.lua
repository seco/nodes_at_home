-- junand 03.10.2016

-------------------------------------------------------------------------------
--  Settings

--- run mode ---
RUN_MODE_PROD = 1;
RUN_MODE_DEV = -1;

runMode = RUN_MODE_PROD;
-- runMode = RUN_MODE_DEV

dofile ( "credential.lua" );

RUNMODE_CONFIG = {

    [RUN_MODE_PROD] = { mqttBroker = "192.168.2.117", credentials = WIFI [RUN_MODE_PROD] },
    [RUN_MODE_DEV]  = { mqttBroker = "192.168.0.100", credentials = WIFI [RUN_MODE_DEV] },
    
};

-- mac based prod config
-- only valid for prod
MAC_TO_CONFIG = {

    ["5c:cf:7f:16:4e:40"] = { mqttLocation = "first",  wifi = { ip = "192.168.2.20", gateway = "192.168.2.1", netmask = "255.255.255.0" } },

};

--- MQTT ---
MQTT_BROKER_PORT = 1883;
MQTT_USERNAME = "";
MQTT_PASSWORD = "";
MQTT_KEEP_ALIVE_TIME = 120; -- sec

MQTT_TOPIC_BASE = "nodes@home/switch/"
MQTT_TOPIC_SWITCH = "rfhub";
MQTT_LOCATION = "TEST";
MQTT_TOPIC_HELLO = "hello";
MQTT_TOPIC_VOLTAGE = "voltage";

MQTT_QOS = 0;
MQTT_RETAIN = 1;

--- WIFI ---
-- wifi.PHYMODE_B 802.11b, More range, Low Transfer rate, More current draw
-- wifi.PHYMODE_G 802.11g, Medium range, Medium transfer rate, Medium current draw
-- wifi.PHYMODE_N 802.11n, Least range, Fast transfer rate, Least current draw 
WIFI_SIGNAL_MODE = wifi.PHYMODE_N;

TIME_WIFI_WAIT_PERIOD = 1;                  -- sec

-------------------------------------------------------------------------------
-- mqtt data

function getMacConfig ()

    local result = nil;
    
    if ( runMode == RUN_MODE_PROD ) then
        result = MAC_TO_CONFIG [wifi.sta.getmac ()];
    end
    
    return result;
    
end

function getMqttTopicBase ( mac )

    local location = MQTT_LOCATION
    local config = getMacConfig ();
    if ( config ~= nil ) then
        location = config.mqttLocation;
    end

    return MQTT_TOPIC_BASE .. MQTT_TOPIC_SWITCH .. "/" .. location .. "/";
    
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
    
        print ( "[WIFI] dnsname=" .. wifi.sta.gethostname () );
        print ( "[WIFI] network=", wifi.sta.getip () );
        print ( "[WIFI] mac=" .. wifi.sta.getmac () );
    
        -- Setup MQTT client and events
        if ( mqttClient == nil ) then
            local mqttClientId = wifi.sta.gethostname ();
            mqttClient = mqtt.Client ( mqttClientId, MQTT_KEEP_ALIVE_TIME, MQTT_USERNAME, MQTT_PASSWORD );
        end

        -- Stop the loop
        tmr.stop ( 0 );
        
        

        local mqttBrokerIp = RUNMODE_CONFIG [runMode].mqttBroker;
        print ( "[MQTT] connecting to " .. mqttBrokerIp );

        mqttClient:on ( "connect", 
            function ( client )
                print ( "[MQTT] CONNECTED" );
            end
        );
            
        mqttClient:on ( "message", 
            function ( client, topic, payload )
                print ( "[MQTT] message received topic=" .. topic .." payload=" .. (payload == nil and "nothing" or payload) );
            end
        );
            
        mqttClient:on ( "offline", 
            function ( client )
                print ( "[MQTT] offline" );
                print ( "[SYSTEM] Going to deep sleep for "..(TIME_BETWEEN_SENSOR_READINGS ).." seconds" );
                node.dsleep ( TIME_BETWEEN_SENSOR_READINGS * 1000 * 1000 );
            end
        );
        
        -- TODO use a second timer for detecting not connecting situation, for example if the broker ist down
        result = mqttClient:connect( mqttBrokerIp , MQTT_BROKER_PORT, 0, 
        
            function ( client )
            
                print ( "[MQTT] connected to MQTT Broker" )
                
                print ( "[MQTT} base_topic=", getMqttTopicBase ( wifi.sta.getmac () ) );
                
                mqttClient:publish ( getMqttTopicBase ( wifi.sta.getmac () ) .. MQTT_TOPIC_HELLO, "Hello from " .. wifi.sta.gethostname (), MQTT_QOS, MQTT_RETAIN,
                    function (  client )
                        print ( "[MQTT] hello sent" );
                        -- .../<switch_name>
                        local topic = getMqttTopicBase ( wifi.sta.getmac () ) .. "+";
                        print ( "[MQTT] subscribe to topic=" .. topic );
                        mqttClient:subscribe ( topic, MQTT_QOS );
                    end
                )
                
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
local credentials = RUNMODE_CONFIG [runMode].credentials;
print ( "[WIFI] connecting to " .. credentials.ssid );
wifi.setmode ( wifi.STATION );
wifi.setphymode ( WIFI_SIGNAL_MODE );
wifi.sta.config ( credentials.ssid, credentials.password );
wifi.sta.connect ();

local config = getMacConfig ();
if ( config ~= nil ) then
    print ( "[WIFI] fix ip=" .. config.wifi.ip );
    wifi.sta.setip ( config.wifi );
end
 
-- loop to wait up to connected to wifi
tmr.alarm ( 0, TIME_WIFI_WAIT_PERIOD * 1000, tmr.ALARM_AUTO, function () loop() end ) -- timer_id, interval_ms, mode

-------------------------------------------------------------------------------