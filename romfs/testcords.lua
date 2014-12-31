require ("cord")
require ("storm")

function gotest()
    cord.new(function()
        while true do
            print ("FA1")
            cord.await(storm.os.invokeLater, 2*storm.os.SECOND)
            print ("FA2")
            cord.await(storm.os.invokeLater, 2*storm.os.SECOND)
            print ("FA3")
            cord.yield()
        end
    end)

    cord.new(function()
        while true do
            print ("FB1")
            cord.await(storm.os.invokeLater, 2*storm.os.SECOND)
            print ("FB2")
            cord.await(storm.os.invokeLater, 2*storm.os.SECOND)
            print ("FB3")
            cord.yield()
        end
    end)

    cord.enter_loop()
end