# final boss
def fib(a)
	m = 0
	n = 1
	i = 0
	while i <= a do
		k = m
		m = m + n
		n = k
		print k
		i = i + 1
	end
end

def fakt(n)
	i = 1
	k = 1
	while i <= n do
		k = k * i
		i = i + 1
	end
	print k
end

def fakt_rek(n)
	if n < 2 then
		result = 1
	else
		tmp = n - 1
		result = fakt_rek(tmp) * n
	end
	result
end

print "Zadej cislo pro faktorial (bonus rekurzi) a fibonaciho: "
lm = inputi
fib(lm)
fakt(lm)
tmp = fakt_rek(lm)
print tmp