import argparse
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


def zad03(infile, outfile):
	with open(infile, 'r') as f:
		pairs = []
		for linefile in f:
			linefile = linefile.split()
			pair = (int(linefile[0]), int(linefile[1]))
			pairs.append(pair)

	d = collections.OrderedDict()

	for i in range(len(pairs)):
		counter = 1
		for j in range(i+1, len(pairs)):
			if pairs[j][0] == pairs[i][0] + counter and pairs[j][1] == pairs[i][1] - counter:
				counter += 1
				values = sum(d.values(), [])
				if pairs[i] not in d.keys() and pairs[i] not in values:
					d[pairs[i]] = [pairs[i]]
				if pairs[j] not in values:
					d[pairs[i]].append(pairs[j])

	with open(outfile, 'w') as f:
		for k, v in d.items():
			f.write("{0} {1} {2}\n".format(k[0], k[1], len(v)))


functions = {"zad01": zad01,
			 "zad02": zad02,
			 "zad03": zad03
			 }

parser = argparse.ArgumentParser(description="Zadanie z Laboratorium #1 z Bioinformatyki Strukturalnej")
parser.add_argument("function", help="nazwa funkcji, ktora ma byc wywolana. Funkcje nazywaja sie zad01, zad02 itd.")
parser.add_argument("infile", help="nazwa pliku wejsciowego")
parser.add_argument("outfile", help="nazwa pliku wyjsciowego")
args = parser.parse_args()

try:
	func = functions[args.function]
	func(args.infile, args.outfile)
except:
	print("Cos poszlo nie tak!")