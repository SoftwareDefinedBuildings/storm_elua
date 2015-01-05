

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
--[[
cord.new(function()
    storm.io.set_mode(storm.io.OUTPUT, storm.io.D2)
    while true do
        storm.io.set(1, storm.io.D2)
        storm.io.set(0, storm.io.D2)
        storm.io.set(1, storm.io.D2)
        storm.io.set(0, storm.io.D2)
        storm.io.set(1, storm.io.D2)
        storm.io.set(0, storm.io.D2)
    end
end)
--]]
N = 12
 
board = {}
for i = 1, N do
    board[i] = {}
    for j = 1, N do
    board[i][j] = false
    end
end
 
function Allowed( x, y )
    for i = 1, x-1 do
    if ( board[i][y] ) or ( i <= y and board[x-i][y-i] ) or ( y+i <= N and board[x-i][y+i] ) then 
        return false 
    end
    end     
    return true
end
 
function Find_Solution( x )
    for y = 1, N do
    if Allowed( x, y ) then 
        board[x][y] = true 
        if x == N or Find_Solution( x+1 ) then
        return true
        end
        board[x][y] = false          
    end     
    end
    return false
end
 
if Find_Solution( 1 ) then
    for i = 1, N do
    for j = 1, N do
        if board[i][j] then 
        io.write( "|Q" )
        else 
        io.write( "| " )
        end
    end
    print( "|" )
    end
else
    print( string.format( "No solution for %d queens.\n", N ) )
end

print(Allowed)
print(cord)
print(cord.enter_loop)
--cord.enter_loop() -- start event/sleep loop
