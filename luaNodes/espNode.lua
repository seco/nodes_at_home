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
                },

    [1495931] = { 
                    app = "tempNode",
                    mode = PROD_MODE,
                    class = "sensor", type = "DHT11", location = "lounge",  
                    wifi = { ip = "192.168.2.21", gateway = PROD_GATEWAY, netmask = PROD_NETMASK }, 
                    mqttBroker = PROD_MQTT_BROKER,
                },

    [-2] = { 
                    app = "tempNode",
                    mode = PROD_MODE,
                    class = "sensor", type = "DHT11", location = "roof",  
                    wifi = { ip = "192.168.2.22", gateway = PROD_GATEWAY, netmask = PROD_NETMASK }, 
                    mqttBroker = PROD_MQTT_BROKER,
                },

    [-3] = { 
                    app = "tempNode",
                    mode = PROD_MODE,
                    class = "sensor", type = "DHT11", location = "terrace",  
                    wifi = { ip = "192.168.2.23", gateway = PROD_GATEWAY, netmask = PROD_NETMASK }, 
                    mqttBroker = PROD_MQTT_BROKER,
                },
                
    -- test
    [8391351] = {
                    app = "rfNode",
                    mode = PROD_MODE,
                    class = "switch", type = "test", location = "nowhere",  
                    wifi = { ip = "192.168.2.25", gateway = PROD_GATEWAY, netmask = PROD_NETMASK }, 
                    mqttBroker = PROD_MQTT_BROKER,
                },

};

--------------------------------------------------------------------
-- private

--------------------------------------------------------------------
-- public

--------------------------------------------------------------------
-- main

print ( "[MODULE] loaded", moduleName )

M.config = NODE_CONFIG_TAB [node.chipid ()];

-- node base topic
M.config.topic = "nodes@home/" .. M.config.class .. "/" .. M.config.type .. "/" .. M.config.location;

return M;

--------------------------------------------------------------------

