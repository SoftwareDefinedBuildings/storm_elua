

--[[
storm.os.invokePeriodically(5*storm.os.SECOND, function()
    print("hello 1")
    storm.os.invokeLater(500*storm.os.MILLISECOND, function ()
        print ("chained operation 1")
        storm.os.invokeLater(500*storm.os.MILLISECOND, function()
            print ("chained operation 2")
            storm.os.invokeLater(500*storm.os.MILLISECOND, function()
                print ("chained operation 3")
                print ("etc, this gets tedious")
            end)
        end)
    end)
end)
--]]

require("storm") -- libraries for interfacing with the board and kernel
require("cord") -- scheduler / fiber library

cord.new(function()
    print("This is like a lightweight thread")
    local retval = cord.await(storm.os.invokeLater, 500*storm.os.MILLISECOND)
    print("I can call any async function with await, and I will be")
    local retval2 = cord.await(storm.os.invokeLater, 500*storm.os.MILLISECOND)
    print("scheduled when the result(s) are available. The timer async I am")
    cord.await(storm.os.invokeLater, 500*storm.os.MILLISECOND)
    print("using doesn't actually return anything. But ADC's would")
    print("No more 100 levels of callbacks!!")
end)

cord.new(function()
    print("I will be interleaved")
    cord.yield()
    print("With any other cord")
    cord.yield()
    print("on either yield() or await() calls")
end)

cord.enter_loop() -- start event/sleep loop