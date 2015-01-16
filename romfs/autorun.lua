
require("storm") -- libraries for interfacing with the board and kernel
require("cord") -- scheduler / fiber library

--[[
sock = storm.net.bind(101, function(payload, from, port)
    print ("Got a message from ",from, " port ",port)
    print ("Payload: ", payload)
end)
]]--

--[[
cord.new(function()
    while true do
        print ("tick")
        cord.await(storm.os.invokeLater, 5*storm.os.SECOND)
    end
end)
]]--
--[[
count = 0
storm.os.invokePeriodically(5*storm.os.SECOND, function()
    --storm.net.sendto(socket_handle, buffer, addrstr, port)
    print ("sending")
    storm.net.sendto(sock, string.format("hello %d", count), "ff02::1", 100)
    count = count + 1
end)
]]--

cord.new(function()
    local txt = ""
    while true do
        txt = cord.await(storm.os.read_stdin)
        print ("txt is '",txt,"'")
        local rv = storm.os.procline(txt)
        print ("rv is", rv)
    end
end)

print("free ram: ",storm.os.freeram())

-- this works
--[[
function foo(txt)
    print("got txt: ",txt)
    storm.os.read_stdin(foo)
end
storm.os.read_stdin(foo)
]]--

--[[
storm.os.read_stdin(function(txt)
    print("got txt: ",txt)
end)
]]--
--cord.enter_loop() -- start event/sleep loop
