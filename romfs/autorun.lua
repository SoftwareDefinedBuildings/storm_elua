
require("storm") -- libraries for interfacing with the board and kernel
require("cord") -- scheduler / fiber library

storm.io.set_mode(storm.io.OUTPUT, storm.io.D2)

local last = 0
storm.os.invokePeriodically(500*storm.os.MILLISECOND, function()
    storm.io.set(last, storm.io.D2)
    last = 1-last
end)


cord.new(function()
    storm.io.set_mode(storm.io.OUTPUT, storm.io.D3)
    while true do
        storm.io.set(1, storm.io.D3)
        cord.await(storm.os.invokeLater, 1000*storm.os.MILLISECOND)
        storm.io.set(0, storm.io.D3)
        cord.await(storm.os.invokeLater, 1000*storm.os.MILLISECOND)
    end
end)

print("um")
x = 5
x()
-- -cord.enter_loop() -- start event/sleep loop
