import collections
import itertools


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

#zad01("1DDY.txt", "outfile_01.txt")


def zad02(infile, outfile):
	with open(infile, 'r') as f:
		pairs = []
		for linefile in f:
			linefile = linefile.split()
			pair = (int(linefile[0]), int(linefile[1]))
			pairs.append(pair)
	combinations = list(itertools.combinations(pairs, 2))
	with open(outfile, 'w') as f:
		for x, y in combinations:
			if x[0] < y[0] < x[1] < y[1]:
				f.write("{0} {1}\n".format(x, y))


#zad02("outfile_01.txt", "outfile_02.txt")


def zad03(infile, outfile):
	with open(infile, 'r') as f:
		pairs = []
		for linefile in f:
			linefile = linefile.split()
			pair = (int(linefile[0]), int(linefile[1]))
			pairs.append(pair)
	d = collections.OrderedDict()
	for i in range(len(pairs) - 1):
		diff_first = pairs[i + 1][0] - pairs[i][0]
		diff_second = pairs[i][1] - pairs[i + 1][1]
		if diff_first == 1 and diff_second == 1:
			if pairs[i] in sum(d.values(), []):
				d[pairs[i - 1]].append(pairs[i + 1])
			else:
				d[pairs[i]] = []
				d[pairs[i]].extend([pairs[i], pairs[i + 1]])
	with open(outfile, 'w') as f:
		for k, v in d.items():
			f.write("{0} {1} {2}\n".format(k[0], k[1], len(v)))


zad03("outfile_01.txt", "outfile_03.txt")