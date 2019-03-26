import argparse


parser = argparse.ArgumentParser(description="Zadanie z Laboratorium #2 z Bioinformatyki Strukturalnej")
parser.add_argument("target", help="plik BP z czasteczka referencyjna")
parser.add_argument("model", help="plik BP z modelem")
args = parser.parse_args()

def zad03(target, model):
	pairs_target = []
	pairs_model = []

	with open(target, 'r') as f:
		for line in f:
			splitted = line.split()
			pair = tuple(map(int, splitted))
			pairs_target.append(pair)

	with open(model, 'r') as f:
		for line in f:
			splitted = line.split()
			pair = tuple(map(int, splitted))
			pairs_model.append(pair)

	pairs_target = set(pairs_target)
	pairs_model = set(pairs_model)
	#print(f"1: {pairs_target}")
	#print(f"2: {pairs_model}")
	tp = pairs_target & pairs_model # Czesc wspolna
	#print(f"TP: {tp}")
	fp = pairs_model - pairs_target # Roznica zbiorow
	#print(f"FP: {fp}")
	fn = pairs_target - pairs_model
	#print(f"FN: {fn}")

	ppv = len(tp) / (len(tp) + len(fp))
	sty = len(tp) / (len(tp) + len(fn))
	inf = round((ppv * sty) ** 0.5, 2)
	print(inf)

zad03(args.target, args.model)