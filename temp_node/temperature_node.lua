-- junand 20.09.2016
-- mqtt temperature node
-- bases on: https://odd-one-out.serek.eu/esp8266-nodemcu-dht22-custom-modules-firmware/
-- MQTT connect script with deep sleep
-- Remember to connect GPIO16/D0 and RST to enable deep sleep

-------------------------------------------------------------------------------
--  Settings

RUN_MODE_PROD = 1;
RUN_MODE_DEV_HOME = -1;
RUN_MODE_DEV_XPERIA = -2;
RUN_MODE_DEV_IPHONE = -3;

runMode = RUN_MODE_DEV_HOME;

--- MQTT ---
if ( runMode == RUN_MODE_PROD ) then
    MQTT_BROKER_IP = ""; 
elseif ( runMode == RUN_MODE_DEV_HOME ) then
    -- 192.168.10.100
    MQTT_BROKER_IP = "192.168.2.101";
elseif ( runMode == RUN_MODE_DEV_XPERIA ) then
    --192.168.43.82
    MQTT_BROKER_IP = "192.168.43.78";
elseif ( runMode == RUN_MODE_DEV_IPHONE ) then
    -- 172.20.10.8
    MQTT_BROKER_IP = "172.20.10.2";
end

MQTT_BROKER_PORT = 1883;
MQTT_USERNAME = "";
MQTT_PASSWORD = "";

MQTT_SENSOR="DHT11"
MQTT_LOCATION="table"
MQTT_TOPIC_BASE = "nodes@home/sensor/" .. MQTT_SENSOR .. "/" .. MQTT_LOCATION .. "/"
MQTT_TOPIC_TEMPERATURE = MQTT_TOPIC_BASE .. "temperature";
MQTT_TOPIC_HUMIDITY = MQTT_TOPIC_BASE .. "humidity";

--- WIFI ---
-- wifi.PHYMODE_B 802.11b, More range, Low Transfer rate, More current draw
-- wifi.PHYMODE_G 802.11g, Medium range, Medium transfer rate, Medium current draw
-- wifi.PHYMODE_N 802.11n, Least range, Fast transfer rate, Least current draw 
WIFI_SIGNAL_MODE = wifi.PHYMODE_N;

-- If the settings below are filled out then the module connects 
-- using a static ip address which is faster than DHCP and 
-- better for battery life. Blank "" will use DHCP.
-- My own tests show around 1-2 seconds with static ip
-- and 4+ seconds for DHCP
-- CLIENT_IP, CLIENT_NETMASK, CLIENT_GATEWAY = "192.168.2.20", "255.255.255.0", "192.168.2.1";
CLIENT_IP, CLIENT_NETMASK, CLIENT_GATEWAY = "192.168.10.20", "255.255.255.0", "192.168.10.254";

--- INTERVAL ---
-- In milliseconds. Remember that the sensor reading, 
-- reboot and wifi reconnect takes a few seconds
TIME_BETWEEN_SENSOR_READINGS = 15 * 60; -- sec

-- DHT11
DHT_PIN = 7; -- GPIO13
-- dhtPin = 2; -- GIO4

-------------------------------------------------------------------------------
-- main

temperature = 0;
humidity = 0;

-- Setup MQTT client and events
mqttClient = mqtt.Client ( client_id, 120, MQTT_USERNAME, MQTT_PASSWORD ); -- 120 sec -> keep alive time

-- Connect to the wifi network
dofile ( "credential.lua" );

print ( "[WIFI] connecting to " .. WIFI_SSID );
wifi.setmode ( wifi.STATION );
wifi.setphymode ( WIFI_SIGNAL_MODE );
wifi.sta.config ( WIFI_SSID, WIFI_PASSWORD );
wifi.sta.connect ();
if CLIENT_IP ~= "" then
    wifi.sta.setip ( { ip = CLIENT_IP, netmask = CLIENT_NETMASK, gateway = CLIENT_GATEWAY } )
end

-- DHT11 sensor logic
function getSensorData ()

    dht = require ( "dht" );
    
    status, temp, humi, temp_decimial, humi_decimial = dht.read ( DHT_PIN );
    
    if( status == dht.OK ) then

        temperature = temp;
        humidity = humi;
        print ( "[DHT] Temperature: "..temperature.." °C" );
        print ( "[DHT] Humidity: "..humidity.."%" );
        
    elseif( status == dht.ERROR_CHECKSUM ) then
    
        print ( "[DHT] Checksum error" );
        temperature = -1;
        
    elseif( status == dht.ERROR_TIMEOUT ) then
    
        print ( "[DHT] Time out" );
        temperature = -2;
        
    end
    
    -- Release module
    dht = nil;
    package.loaded [ "dht" ] = nil;
    
end

function getJsonValueMessage ( value, unit )

    return [[{"value":]] .. value .. [[, "unit":"]] .. unit .. [["}]];
    
end

function loop ()

    -- 0: STA_IDLE,
    -- 1: STA_CONNECTING,
    -- 2: STA_WRONGPWD,
    -- 3: STA_APNOTFOUND,
    -- 4: STA_FAIL,
    -- 5: STA_GOTIP.

    if ( wifi.sta.status () == wifi.STA_GOTIP ) then -- TODO change 5 to symbol
    
        print ( wifi.sta.gethostname () );
        print ( wifi.sta.getip () );
        print ( wifi.sta.getmac () );
    
        -- Stop the loop
        tmr.stop ( 0 );

        print ( "[MQTT] Connecting to " .. MQTT_BROKER_IP );
        
        result = mqttClient:connect( MQTT_BROKER_IP , MQTT_BROKER_PORT, 0, 
        
            function ( conn )
            
                print ( "[MQTT] Connected to MQTT" )
                print ( "[MQTT]   IP: ".. MQTT_BROKER_IP)
                
                getSensorData() 
                
                mqttClient:publish ( MQTT_TOPIC_TEMPERATURE, getJsonValueMessage ( temperature, "°C" ), 0, 0, -- topic, payload, qos, retain, callback
                    function ( conn )
                        mqttClient:publish ( MQTT_TOPIC_HUMIDITY, getJsonValueMessage ( humidity, "%" ), 0, 0, 
                            function(conn)
                                print ( "Going to deep sleep for "..(TIME_BETWEEN_SENSOR_READINGS ).." seconds" );
                                node.dsleep ( TIME_BETWEEN_SENSOR_READINGS * 1000 * 1000 );
                            end
                        )
                    end
                )
            end,

            function ( conn, reason ) 
                print ( "[MQTT] not connected reason=", reason );
            end
        
        )
        print ( "[MQTT] connect result=", result );
    else
        print ( "[WIFI] Connecting..." )
    end
    
end
        
-- loop to wait up to connected to wifi
tmr.alarm ( 0, 1000, tmr.ALARM_AUTO, function () loop() end ) -- timer_id, interval_ms, mode

-- Watchdog loop, will force deep sleep the operation somehow takes to long
-- tmr.alarm ( 1, 4000, tmr.ALARM_AUTO, function() node.dsleep ( TIME_BETWEEN_SENSOR_READINGS * 1000 ) end )
