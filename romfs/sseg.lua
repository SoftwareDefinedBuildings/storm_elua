--
-- This provides the support for controlling the seven segment display on the clock shield
--

require("storm")

sseg = function ()
    local m = {}
    local clk = storm.io.D7
    local dat = storm.io.D8
    local blueled = storm.io.D2
    local greenled = storm.io.D3
    local redled1 = storm.io.D4
    local redled2 = storm.io.D5
    local writeByte = function(dat)
    end
    m.init = function ()
        storm.io.set_mode(storm.io.OUTPUT, clk, dat)
    end

end  --module