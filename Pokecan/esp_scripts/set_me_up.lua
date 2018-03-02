local mac = wifi.sta.getmac()
local pass = string.sub(mac, 7, 8) .. string.sub(mac, 10, 11) .. string.sub(mac, 13, 14) ..string.sub(mac, 16, 17)
local ssid = "Pokecan_" .. pass

wifi.setmode(wifi.STATIONAP)
apcfg = {}
apcfg.ssid = ssid
apcfg.pwd = pass
apcfg.auth = wifi.AUTH_WPA2_PSK
wifi.ap.config(apcfg)

dhcpcfg = {}
dhcpcfg.start = "192.168.4.77"
wifi.ap.dhcp.stop()
wifi.ap.dhcp.config(dhcpcfg)
wifi.ap.dhcp.start()

enduser_setup.manual(true)
enduser_setup.stop()
enduser_setup.start(
    function()
        enduser_setup.stop()
        wifi.ap.dhcp.stop()
        wifi.setmode(wifi.STATION)
    end,
    function(err, str)
        enduser_setup.stop()
        wifi.ap.dhcp.stop()
        wifi.setmode(wifi.STATION)
    end
)

print("started")