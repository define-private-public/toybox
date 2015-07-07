function foo(str)
	local s, len = bar(str)

	local s2, len2 = bar("hamburger")

	local t = {}
	t[s] = len
	t[s2] = len2
	return t
end


