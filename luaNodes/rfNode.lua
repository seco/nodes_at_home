--------------------------------------------------------------------
--
-- nodes@home/luaNodes/rfNode
-- author: andreas at jungierek dot de
-- LICENSE http://opensource.org/licenses/MIT
--
--------------------------------------------------------------------
-- junand 03.10.2016

local moduleName = ...;
local M = {};
_G [moduleName] = M;

require ( "rfCode" );
require ( "mqttNode" );
require ( "util" );

-------------------------------------------------------------------------------
--  Settings

M.version = "V0.10 (rfNode)";

----------------------------------------------------------------------------------------
-- private
-- mqtt callbacks

local function offline ( client )

    print ( "[APP] offline" );
    
    return true; -- restart mqtt connection
    
end

local function message ( client, topic, payload )

    print ( "[APP] message: topic=", topic, " payload=", payload );
    local topicParts = util.splitTopic ( topic );
    local device = topicParts [#topicParts];
    rfCode.send ( device, payload );
    print ( "heap=", node.heap () );

end

--------------------------------------------------------------------
-- public

-------------------------------------------------------------------------------
-- main

print ( "[MODULE] loaded", moduleName )

-- M.connect = connect;
M.offline = offline;
M.message = message;

return M;

-------------------------------------------------------------------------------