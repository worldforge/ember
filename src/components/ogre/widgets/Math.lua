-- Returns a normally distributed random number with mean mu and standard deviation sigma
-- Algorithm from Knuth' The art of computer programming
function math.randn(mu, sigma)
	if mu == nil then
		mu = 0
	end
	if sigma == nil then
		sigma = 1
	end
	local V1, V2, S, d
	repeat
		V1 = 2*math.random() - 1
		V2 = 2*math.random() - 1
		S = V1^2 + V2^2
	until S < 1
	d = math.sqrt(-2*math.log(S)/S)
	return mu + sigma*V1*d, mu + sigma*V2*d
end
