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

-------------------------------------------------------------------------------
--  Settings

M.version = "V0.10";

----------------------------------------------------------------------------------------
-- private
-- mqtt data

-- from: http://lua-users.org/wiki/SplitJoin
local function split ( str, pat )

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

local function splitTopic ( str )

   return split ( str, '[\\/]+' );
   
end

-------------------------------------------------------------------------------
-- mqtt callbacks

local function offline ( client )

    print ( "[APP] offline" );
    -- TODO reconnect to broker
    
end

local function message ( client, topic, payload )

    print ( "[APP] message: topic=", topic, " payload=", payload );
    if ( payload ) then
        local topicParts = splitTopic ( topic );
        local device = topicParts [#topicParts];
        rfCode.send ( device, payload );
        print ( "heap=", node.heap () );
    end

end

--------------------------------------------------------------------
-- public

-------------------------------------------------------------------------------
-- main

print ( "[MODULE] loaded", moduleName )
print ( "heap=", node.heap () );

M.connect = connect;
M.offline = offline;
M.message = message;

return M;

-------------------------------------------------------------------------------