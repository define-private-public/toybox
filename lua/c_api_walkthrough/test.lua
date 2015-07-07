function foo(str)
	
	if (bar == nil) then
		print("bar() is nil")
		return
	end

	local s, len = bar(str)
	print(s)
	return len
end


