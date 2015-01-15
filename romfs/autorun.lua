
require("storm") -- libraries for interfacing with the board and kernel
require("cord") -- scheduler / fiber library


sock = storm.net.bind(101, function(payload, from, port)
    print ("Got a message from ",from, " port ",port)
    print ("Payload: ", payload)
end)




count = 0
storm.os.invokePeriodically(5*storm.os.SECOND, function()
    --storm.net.sendto(socket_handle, buffer, addrstr, port)
    print ("sending")
    storm.net.sendto(sock, string.format("hello %d", count), "ff02::1", 100)
    count = count + 1
end)

cord.enter_loop() -- start event/sleep loop
