--------------------------------------------------------------------
--
-- nodes@home/luaNodes/espNode
-- author: andreas at jungierek dot de
-- LICENSE http://opensource.org/licenses/MIT
--
--------------------------------------------------------------------
-- junand 14.10.2016

local moduleName = ...;
local M = {};
_G [moduleName] = M;

--------------------------------------------------------------------
-- vars

local VERSION = "V0.20dev_rc11"

local PROD_MODE = "prod";
local PROD_GATEWAY = "192.168.2.1";
local PROD_NETMASK = "255.255.255.0";
local PROD_MQTT_BROKER = "192.168.2.117";

-- key is node.chipid ()
local NODE_CONFIG_TAB = {

    [1461824] = { 
                    app = "rfNode",
                    mode = PROD_MODE,
                    class = "switch", type = "rfhub", location = "first",  
                    wifi = { ip = "192.168.2.20", gateway = PROD_GATEWAY, netmask = PROD_NETMASK }, 
                    mqttBroker = PROD_MQTT_BROKER,
                    appCfg = {
                        rfpin = 7,
                        rfrepeats = 16,
                        rfperiod = 320,
                        ledpin = 4,
                        dhtPin = 6,
                    },
                    timer = {
                        startup = 0,
                        startupDelay1 = 2 * 1000,
                        startupDelay2 = 5 * 1000,
                        wifiLoop = 1,
                        wifiLoopPeriod = 1 * 1000,
                        periodic = 2,
                        periodicPeriod = 15 * 60 * 1000,
                        queue = 3,
                        queuePeriod = 500,
                    },
                },

    [1495931] = { 
                    app = "tempNode",
                    mode = PROD_MODE,
                    class = "sensor", type = "DHT11", location = "lounge",  
                    wifi = { ip = "192.168.2.21", gateway = PROD_GATEWAY, netmask = PROD_NETMASK }, 
                    mqttBroker = PROD_MQTT_BROKER,
                    appCfg = {
                        useOfflineCallback = false,
                        dhtPin = 4,
                        timeBetweenSensorReadings = 15 * 60 * 1000, -- ms
                    },
                    timer = {
                        startup = 0,
                        startupDelay1 = 2 * 1000,
                        startupDelay2 = 5 * 1000,
                        wifiLoop = 1,
                        wifiLoopPeriod = 1 * 1000,
                        periodic = 2,
                        periodicPeriod = 15 * 60 * 1000,
                        deepSleep = 3,
                        deepSleepDelay = 60 * 1000, -- ms, only if not useOfflineCallback
                    },
                },

    [1829768] = { 
                    app = "tempNode",
                    mode = PROD_MODE,
                    class = "sensor", type = "DHT11", location = "roof",  
                    wifi = { ip = "192.168.2.22", gateway = PROD_GATEWAY, netmask = PROD_NETMASK }, 
                    mqttBroker = PROD_MQTT_BROKER,
                    appCfg = {
                        useOfflineCallback = false,
                        dhtPin = 4,
                        timeBetweenSensorReadings = 15 * 60 * 1000, -- ms
                    },
                    timer = {
                        startup = 0,
                        startupDelay1 = 2 * 1000,
                        startupDelay2 = 5 * 1000,
                        wifiLoop = 1,
                        wifiLoopPeriod = 1 * 1000,
                        periodic = 2,
                        periodicPeriod = 15 * 60 * 1000,
                        deepSleep = 3,
                        deepSleepDelay = 60 * 1000, -- ms, only if not useOfflineCallback
                    },
                },

    [1495743] = { 
                    app = "tempNode",
                    mode = PROD_MODE,
                    class = "sensor", type = "DHT11", location = "terrace",  
                    wifi = { ip = "192.168.2.23", gateway = PROD_GATEWAY, netmask = PROD_NETMASK }, 
                    mqttBroker = PROD_MQTT_BROKER,
                    appCfg = {
                        useOfflineCallback = false,
                        dhtPin = 4,
                        timeBetweenSensorReadings = 15 * 60 * 1000, -- ms
                    },
                    timer = {
                        startup = 0,
                        startupDelay1 = 2 * 1000,
                        startupDelay2 = 5 * 1000,
                        wifiLoop = 1,
                        wifiLoopPeriod = 1 * 1000,
                        periodic = 2,
                        periodicPeriod = 15 * 60 * 1000,
                        deepSleep = 3,
                        deepSleepDelay = 60 * 1000, -- ms, only if not useOfflineCallback
                    },
                },
                
    [8391351] = {
                    app = "garageNode",
                    mode = PROD_MODE,
                    class = "cover", type = "relay", location = "garage",  
                    wifi = { ip = "192.168.2.25", gateway = PROD_GATEWAY, netmask = PROD_NETMASK }, 
                    mqttBroker = PROD_MQTT_BROKER,
                    appCfg = {
                        relayPin = 1;
                        openPositionPin = 2;
                        closedPositionPin = 3;
                        dhtPin = 4;
                    },
                    timer = {
                        startup = 0,
                        startupDelay1 = 2 * 1000,
                        startupDelay2 = 5 * 1000,
                        wifiLoop = 1,
                        wifiLoopPeriod = 1 * 1000,
                        periodic = 2,
                        periodicPeriod = 15 * 60 * 1000,
                        debounce = 3,
                        debounceDelay = 200,
                        trigger = 4,
                        triggerDelay = 300,
                        state = 5,
                        statePeriod = 1000,
                    },
                },

    [485535] = {
                    app = "sonoffNode",
--                    mode = PROD_MODE,
                    mode = "vaio",
                    class = "switch", type = "sonoff", location = "backyard",  
--                    wifi = { ip = "192.168.2.26", gateway = PROD_GATEWAY, netmask = PROD_NETMASK }, 
--                    mqttBroker = PROD_MQTT_BROKER,
                    mqttBroker = "192.168.137.1",
                    appCfg = {
                        relayPin = 6;
                        ledPin = 7;
                        buttonPin = 3;
                        extraPin = 5;
                    },
                    timer = {
                        startup = 0,
                        startupDelay1 = 2 * 1000,
                        startupDelay2 = 5 * 1000,
                        wifiLoop = 1,
                        wifiLoopPeriod = 1 * 1000,
                        periodic = 2,
                        periodicPeriod = 15 * 60 * 1000,
                        debounce = 3,
                        debounceDelay = 200,
                    },
                },
                
    [518010] = {
                    app = "sonoffNode",
--                    mode = PROD_MODE,
                    mode = "vaio",
                    class = "switch", type = "sonoff", location = "terrace",  
--                    wifi = { ip = "192.168.2.27", gateway = PROD_GATEWAY, netmask = PROD_NETMASK }, 
--                    mqttBroker = PROD_MQTT_BROKER,
                    mqttBroker = "192.168.137.1",
                    appCfg = {
                        relayPin = 6;
                        ledPin = 7;
                        buttonPin = 3;
                        extraPin = 5;
                    },
                    timer = {
                        startup = 0,
                        startupDelay1 = 2 * 1000,
                        startupDelay2 = 5 * 1000,
                        wifiLoop = 1,
                        wifiLoopPeriod = 1 * 1000,
                        periodic = 2,
                        periodicPeriod = 15 * 60 * 1000,
                        debounce = 3,
                        debounceDelay = 500,
                    },
                },
                

};

--------------------------------------------------------------------
-- private

--------------------------------------------------------------------
-- public

--------------------------------------------------------------------
-- main

print ( "[MODULE] loaded", moduleName )

M.node = NODE_CONFIG_TAB [node.chipid ()];

-- node base topic
M.node.topic = "nodes@home/" .. M.node.class .. "/" .. M.node.type .. "/" .. M.node.location;
M.node.version = VERSION .. " (" .. M.node.app .. ")";

return M;

--------------------------------------------------------------------

