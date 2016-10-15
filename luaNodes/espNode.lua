--------------------------------------------------------------------
--
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
                    -- topic = "nodes@home/" .. class .. "/" .. type .. "/" .. location,
                    topic = "nodes@home/switch/rfhub/first",
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

return M;

--------------------------------------------------------------------

