import argparse


parser = argparse.ArgumentParser(description="Zadanie z Laboratorium #2 z Bioinformatyki Strukturalnej")
parser.add_argument("pdb1", help="plik PDB")
parser.add_argument("pdb2", help="plik PDB")
parser.add_argument("atom_name", help="interesujacy nas atom")
args = parser.parse_args()

def zad01(pdb_1, pdb_2, atom_name):
	coords_1 = []
	coords_2 = []

	with open(pdb_1, 'r') as f:
		for line in f:
			if line.startswith(("ATOM", "HETATM")):
				atom = line[12:16].strip()
				if atom == atom_name:
					x = line[30:38].strip()
					y = line[38:46].strip()
					z = line[46:54].strip()
					coords = tuple((map(float, [x, y, z])))
					coords_1.append(coords)

	with open(pdb_2, 'r') as f:
		for line in f:
			if line.startswith(("ATOM", "HETATM")):
				atom = line[12:16].strip()
				if atom == atom_name:
					x = line[30:38].strip()
					y = line[38:46].strip()
					z = line[46:54].strip()
					coords = tuple((map(float, [x, y, z])))
					coords_2.append(coords)

	if len(coords_1) != len(coords_2):
		print(-1)
	else:
		total_sum = 0
		for i in range(len(coords_1)):
			x_dist = (coords_1[i][0] - coords_2[i][0]) ** 2
			y_dist = (coords_1[i][1] - coords_2[i][1]) ** 2
			z_dist = (coords_1[i][2] - coords_2[i][2]) ** 2
			dist_sum = x_dist + y_dist + z_dist
			result = (dist_sum ** 0.5) ** 2
			total_sum += result

		rmsd = round(((total_sum / len(coords_1)) ** 0.5), 2)
		print(rmsd)


zad01(args.pdb1, args.pdb2, args.atom_name)