require("storm")

mpdb = {}
mpdb._sock = nil
mpdb._server_addr = nil
mpdb._server_port = nil

mpdb.connect = function (addr, port)
    mpdb._server_addr = addr
    mpdb._server_port = port
    mpdb._sock = storm.net.bind(200, function(payload, from, port)
        print ("Got a message from ",from, " port ",port)
        done = storm.mp.unpack(payload)
        for i, tab in ipairs(done["results"]) do 
            print("result", i)
            for k,v in pairs(tab) do print ("k/v",k,v) end
        end
        for key,value in pairs(done) do print ("k/v",key,value) end
    end)
end

mpdb.insert = function (table)
    local msg = {
        nodeid = 0xfeeb,
        oper = "INSERT",
        echo = 1,
        data = table
    }
    storm.net.sendto(mpdb._sock, storm.mp.pack(msg), mpdb._server_addr, mpdb._server_port)
end

mpdb.persist = function (table)
    local msg = {
        nodeid = 0xfeeb,
        oper = "PERSIST",
        echo = 1,
        data = table
    }
    storm.net.sendto(mpdb._sock, storm.mp.pack(msg), mpdb._server_addr, mpdb._server_port)
end

mpdb.query = function (where)
    local msg = {
        nodeid = 0xfeeb,
        oper = "QUERY",
        echo = 1,
        data = where
    }
    storm.net.sendto(mpdb._sock, storm.mp.pack(msg), mpdb._server_addr, mpdb._server_port)
end
