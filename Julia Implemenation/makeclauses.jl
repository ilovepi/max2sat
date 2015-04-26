
function make_clauses()
	outfile = open("instance.txt", "w")

	#srand(12345)

	#top = 70 #rand(1:40)
	#k = binomial(top,2)

	k = rand(20:1000)
	a = 1
	b = -1
	c = -k	
	lowerbound = int((-b + sqrt(b^2 - 4*a*c))/(2a))
	upperbound = 2k
	println("Lower bound = ", lowerbound)
	println("Upper bound = ", upperbound)
	println("K = ", k)
	for i = 1:k
		x1 = rand(lowerbound:upperbound)		
		x2 = rand(lowerbound:upperbound)
		
		if(randbool())
			x1=-x1
		end 

		if(randbool())
			x2=-x2
		end 
		 
		println(outfile, join((x1, x2), " "))
	end 
	close(outfile)
end



