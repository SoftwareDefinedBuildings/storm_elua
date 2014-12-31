function test()
    print("Testing")
    storm.io.set_mode(storm.io.OUTPUT, storm.io.D2)
    storm.io.set(1, storm.io.D2)
    print("Test done")
end

require ("storm")

function test2()
    print ("loading timer")
    storm.os.invokeLater(100, function ()
            print("invoked!")
    end
    )
    print ("callback registered")
end

function run(f)
    ok, err = pcall(f)
    if (not ok) then
        print (err)
    end
end


