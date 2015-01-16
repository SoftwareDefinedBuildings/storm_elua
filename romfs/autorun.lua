
--require("storm") -- libraries for interfacing with the board and kernel
require("cord") -- scheduler / fiber library
require("mpdb") -- msgpack database


sock = storm.net.bind(101, function(payload, from, port)
    print ("Got a message from ",from, " port ",port)
    print ("Payload: ", payload)
end)


mpdb.connect("2001:470:4885:1:9::", 7000)

count = 0
storm.os.invokePeriodically(5*storm.os.SECOND, function()
    --storm.net.sendto(socket_handle, buffer, addrstr, port)
    --print ("sending")
    packet = {
        key = "mykey",
        value = "here's a value",
        count = count
    }
    mpdb.insert(packet)
    count = count + 1
    if count % 4 == 0 then
        print (" oh")
        query = {
           count = 40
        }
        mpdb.query(query)
    end
end)

cord.enter_loop() -- start event/sleep loop
