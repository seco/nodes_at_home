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
require ( "util" );

-------------------------------------------------------------------------------
--  Settings

----------------------------------------------------------------------------------------
-- private
-- mqtt callbacks

function M.offline ( client )

    print ( "[APP] offline" );
    
    return true; -- restart mqtt connection
    
end

function M.message ( client, topic, payload )

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

return M;

-------------------------------------------------------------------------------