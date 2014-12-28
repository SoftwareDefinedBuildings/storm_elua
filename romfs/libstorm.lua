function test()
    print("Testing")
    storm.io.set_mode(storm.io.OUTPUT, storm.io.D2)
    storm.io.set(1, storm.io.D2)
    print("Test done")
end

function run(f)
    ok, err = pcall(f)
    if (not ok) then
        print (err)
    end
end