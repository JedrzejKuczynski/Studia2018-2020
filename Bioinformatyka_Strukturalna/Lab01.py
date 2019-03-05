def zad01(infile, outfile):
	combinations = ["AU", "UA", "GC", "CG", "GU", "UG"]
	with open(infile, 'r') as f:
		pairs = []
		for linefile in f:
			if "cWW" in linefile:
				splitted = linefile.split()
				comb = splitted[1] + splitted[4]
				if comb in combinations:
					first_pair = splitted[2]
					second_pair = splitted[5]
					first_pair = int(first_pair.split('(')[0])
					second_pair = int(second_pair.split('(')[0])
					if first_pair < second_pair:
						pair = (first_pair, second_pair)
						pairs.append(pair)

	with open(outfile, 'w') as f:
		for first, second in pairs:
			f.write("{0} {1}\n".format(first, second))

zad01("1DDY.txt", "outfile.txt")

def zad02(infile, outfile):
	pass