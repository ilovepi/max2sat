using JuMP

using Cbc

type clause
	x::Int
	y::Int
end

function menu(clauses::Array{clause}, x_max::Int)	
	println("Which algorithm would you like to run?")
	println("1. Fast")
	println("2. Exact")
	print("3. Quit\n> ")
	choice = strip(readline(STDIN))
	println()
	if choice == "3"
		println("now exiting max2sat solver ...")
		return true
 	elseif choice == "1"
		best, ans =approx(clauses, x_max)
		println("Clauses satisfied = ", int(best), " out of ", length(clauses))
		println("Truth assignment = ", bitpack(int(getValue(ans).innerArray)))
	elseif choice == "2"
		best, ans = exact(clauses, x_max)
		println("Clauses satisfied = ", int(best), " out of ", length(clauses))
		println("Truth assignment = ", ans)
	else 
		println("Bad input, please try again (1-3)")		
	end 
	return false
end 

function prep(filename::String)
	println("Reading file from ", filename)
	f = open(filename)
	lines=readlines(f)
	len = length(lines)
	close(f)

	x_max= 0
	clauses = Array(clause, len)

	for index = 1:len	
		items=int(split(lines[index]))	
		clauses[index]=clause(items[1],items[2])
		if(abs(items[1]) > x_max)
			x_max = abs(items[1])
		end
		if(abs(items[2]) > x_max)
			x_max = abs(items[2])
		end
	end	
	return x_max, clauses
end 

function sat_solver(filename::String)
	x_max, clauses = prep(filename)

	while !menu(clauses, x_max) == true
		println()		
	end
	
end

function exact(clauses::Array{clause}, x_max::Int)
	best = 0	
	x = BitArray(x_max + 1)
	ans = deepcopy(x)
	while !x[x_max+1]
		temp = check(x, clauses, best)
		if best < temp
			best = temp
			ans = deepcopy(x)
		end 
		increment(x)
	end 
	return best, ans[1:length(ans)-1]
end

function check_clause(c::clause, bs::BitArray)
	idx = abs(c.x)
	idy = abs(c.y)
	if(c.x < 0)
		if(c.y<0)
			!bs[idx] || !bs[idy]
		else 
			!bs[idx] || bs[idy]
		end 
	else 
		if(c.y<0)
			bs[idx] || !bs[idy]
		else 
			bs[idx] || bs[idy]
		end 
	end 
end 


function check(bs::BitArray, clauses::Array{clause}, best::Int)
	count = 0
	len = length(clauses)
	for i = 1: len
		if(check_clause(clauses[i], bs))
			count +=1
		end 
		if count + len -i < best 
			break
		end 
	end 
	return count
end 


function increment(bs::BitArray)
	bslen = length(bs)
	assert(bslen != 0)
	for loop = 1:bslen
		if ((bs[loop] $= 0x1)== 0x1)
			break
		end 
	end 
end 


function approx(clauses::Array{clause}, x_max::Int)
	m = Model(solver=CbcSolver(threads=6))

	len = length(clauses)
	@defVar(m, 0<= x[1:x_max] <= 1, Int)
	#@defVar(m, 0<= x[1:x_max] <= 1)
	@defVar(m, 0<= z[1:len] <= 1)
	
	for i = 1:len
		idx = abs(clauses[i].x)	
		idy = abs(clauses[i].y)
		if(clauses[i].x < 0)
			if(clauses[i].y<0)
				@addConstraint(m, (1-x[idx]) + (1-x[idy]) >= z[i])
			else 
				@addConstraint(m, (1-x[idx]) + x[idy] >= z[i])
			end 
		else 
			if(clauses[i].y<0)
				@addConstraint(m, x[idx] + (1-x[idy]) >= z[i])
			else 
				@addConstraint(m, x[idx] + x[idy] >= z[i])
			end 
		end 
	end 
	@setObjective(m, :Max, sum{z[i], i=1:len} )	
	
	status = solve(m)
	#println(m)

	satisfied = getObjectiveValue(m)

	println(status)
	
	return satisfied, x	
end