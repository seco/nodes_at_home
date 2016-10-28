--------------------------------------------------------------------
--
-- nodes@home/luaNodes/startup
-- author: andreas at jungierek dot de
-- LICENSE http://opensource.org/licenses/MIT
--
--------------------------------------------------------------------
-- junand 22.09.2016
-- 27.09.2016 integrated some code from https://bigdanzblog.wordpress.com/2015/04/24/esp8266-nodemcu-interrupting-init-lua-during-boot/

local moduleName = ...;
local M = {};
_G [moduleName] = M;

require ( "espConfig" );
require ( "credential" ).init ( espConfig.node.mode );
package.loaded ["espConfig"] = nil;
package.loaded ["credential"] = nil;
collectgarbage ();

require ( "wifi" );

--------------------------------------------------------------------
-- vars

local startupTimer = espConfig.node.timer.startup;

local startupDelay1 = espConfig.node.timer.startupDelay1;
local startupDelay2 = espConfig.node.timer.startupDelay2;


--- WIFI ---
-- wifi.PHYMODE_B 802.11b, More range, Low Transfer rate, More current draw
-- wifi.PHYMODE_G 802.11g, Medium range, Medium transfer rate, Medium current draw
-- wifi.PHYMODE_N 802.11n, Least range, Fast transfer rate, Least current draw 
local WIFI_SIGNAL_MODE = wifi.PHYMODE_N;

--------------------------------------------------------------------
-- private

local function startup()

    print ( "[STARTUP] press ENTER to abort" );
    
    -- if <CR> is pressed, abort startup
    uart.on ( "data", "\r", 
        function ()
            tmr.unregister ( startupTimer );   -- disable the start up timer
            uart.on ( "data" );                 -- stop capturing the uart
            print ( "[STARTUP] aborted" );
        end, 
        0 );

    -- startup timer to execute startup function in 5 seconds
    tmr.alarm ( startupTimer, startupDelay2, tmr.ALARM_SINGLE, 
    
        function () 
            -- stop capturing the uart
            uart.on ( "data" );
            print ( "[STARTUP] application " .. espConfig.node.app .. " is starting" );
            -- Connect to the wifi network
            print ( "[WIFI] connecting to " .. credential.ssid );
            wifi.setmode ( wifi.STATION );
            wifi.setphymode ( WIFI_SIGNAL_MODE );
            wifi.sta.config ( credential.ssid, credential.password );
            wifi.sta.connect ();
            local wificfg = espConfig.node.wifi;
            if ( wificfg ) then
                print ( "[WIFI] fix ip=" .. wificfg.ip );
                wifi.sta.setip ( wificfg );
            end
            
            if ( file.exists ( "update.url" ) ) then
                print ( "[STARTUP] update file found" );
                require ( "update" ).update ();
            else
                print ( "[STARTUP] start app=", espConfig.node.app );
                local app = require ( espConfig.node.app );
                require ( "mqttNode" ).start ( app );
            end
            
        end 

    );

end
    
--------------------------------------------------------------------
-- public

--------------------------------------------------------------------
-- main

print ( "[MODULE] loaded", moduleName )

if ( adc.force_init_mode ( adc.INIT_VDD33 ) ) then
  node.restart ();
  return; -- don't bother continuing, the restart is scheduled
end

print ( "[STARTUP] waiting for application start" );
tmr.alarm ( startupTimer, startupDelay1, tmr.ALARM_SINGLE, startup )

return M;

--------------------------------------------------------------------
