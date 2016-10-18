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

-- key is node.chipid ()
local NODE_CONFIG_TAB = {

    [1461824] = { 
                    app = "rfNode",
                    mode = "prod",
                    class = "switch", type = "rfhub", location = "first",  
                    wifi = { ip = "192.168.2.20", gateway = "192.168.2.1", netmask = "255.255.255.0" }, 
                    mqttBroker = "192.168.2.117",
                    topic = "***nodes@home/switch/rfhub/first",
                },

    [1495931] = { 
                    app = "tempNode",
                    mode = "prod",
                    class = "sensor", type = "DHT11", location = "lounge",  
                    wifi = { ip = "192.168.2.21", gateway = "192.168.2.1", netmask = "255.255.255.0" }, 
                    mqttBroker = "192.168.2.117",
                    topic = "***nodes@home/sensor/DHT11/lounge",
                },

    [-2] = { 
                    app = "tempNode",
                    mode = "prod",
                    class = "sensor", type = "DHT11", location = "roof",  
                    wifi = { ip = "192.168.2.22", gateway = "192.168.2.1", netmask = "255.255.255.0" }, 
                    mqttBroker = "192.168.2.117",
                    topic = "***nodes@home/sensor/DHT11/roof",
                },

    [-3] = { 
                    app = "tempNode",
                    mode = "prod",
                    class = "sensor", type = "DHT11", location = "terrace",  
                    wifi = { ip = "192.168.2.23", gateway = "192.168.2.1", netmask = "255.255.255.0" }, 
                    mqttBroker = "192.168.2.117",
                    topic = "***nodes@home/sensor/DHT11/terrace",
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

