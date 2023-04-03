-- Stage 1: Reading Simple Values
a = 5
b = 7
c = a + b
d = "OneLoneCoder.com"

-- Stage 2: Reading a simple object
player = {Title = "Squire", Name = "Ciaran", Family = "Wirral", Level = 20}

-- Stage 3: Calling a Lua Function
function CalledFromCPP1(r1, r2, r3)
	io.write("[LUA S3] CalledFromCPP1("..r1..", "..r2..", "..r3..")\n")
	return r1 * r2
end

-- Stage 4: Calling a C++ Function
function CalledFromCPP2(r1, r2)
	io.write("[LUA S4] CalledFromCPP2("..r1..", "..r2..")\n")
	io.write("[LUA S4] Calling HostFunction("..r1..", "..r2..")\n")
	r = HostFunction(r1, r2)
	io.write("[LUA S4] HostFunction returned "..r.."\n")
	return r * 15
end