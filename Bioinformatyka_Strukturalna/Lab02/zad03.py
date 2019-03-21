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

zad03("target.bp", "model.bp")