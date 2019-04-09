from collections import namedtuple


allowed_atoms = ["C:N3", "C:O2", "C:N4",
                 "G:N1", "G:N2", "G:O6",  # kanoniczna C-G
                 "A:N1", "A:N6",
                 "U:N3", "U:O4",  # kanoniczna A-U
                 "G:O6", "G:N1",
                 "U:N3", "U:O2"  # G-U (wobble)
                 ]

allowed_pairs = ["CG", "GC",
                 "AU", "UA",
                 "GU", "UG"]

# Nie wiem co tu lepszego. Na razie jest se tak

allowed_distances = {("C:N3", "G:N1"): (2.492, 3.150),
                     ("G:N1", "C:N3"): (2.492, 3.150),
                     ("C:O2", "G:N2"): (2.409, 3.135),
                     ("G:N2", "C:O2"): (2.409, 3.135),
                     ("C:N4", "G:O6"): (2.485, 3.185),
                     ("G:O6", "C:N4"): (2.485, 3.185),  # kanoniczna C-G
                     ("A:N1", "U:N3"): (2.390, 3.027),
                     ("U:N3", "A:N1"): (2.390, 3.027),
                     ("A:N6", "U:O4"): (2.503, 3.224),
                     ("U:O4", "A:N6"): (2.503, 3.224),  # kanoniczna A-U
                     ("G:O6", "U:N3"): (2.440, 3.220),
                     ("U:N3", "G:O6"): (2.440, 3.220),
                     ("G:N1", "U:O2"): (2.400, 3.180),
                     ("U:O2", "G:N1"): (2.400, 3.180)  # G-U (wobble)
                     }

Atom = namedtuple("Atom", ["atom_name", "x_coord", "y_coord", "z_coord"])
Residue = namedtuple("Residue", ["name", "number", "atoms", "paired"])


def zad01(pdb):
    residues = {}

    with open(pdb, 'r') as f:
        for line in f:
            if line.startswith("ATOM"):
                atom_name = line[12:16].strip()
                residue_name = line[17:20].strip()
                res_atom = ":".join([residue_name, atom_name])
                if res_atom in allowed_atoms:
                    residue_number = int(line[22:26].strip())
                    x_coord = float(line[30:38].strip())
                    y_coord = float(line[38:46].strip())
                    z_coord = float(line[46:54].strip())

                    new_atom = Atom(atom_name, x_coord, y_coord, z_coord)
                    res_key = tuple([residue_name, residue_number])

                    if res_key not in residues.keys():
                        new_residue = Residue(residue_name,
                                              residue_number, [], False)
                        new_residue.atoms.append(new_atom)
                        residues[res_key] = new_residue
                    else:
                        residues[res_key].atoms.append(new_atom)

    bpseq_list = []

    for key1, value1 in residues.items():
        for key2, value2 in residues.items():
            if key1 != key2:
                pair = "".join([key1[0], key2[0]])
                if pair in allowed_pairs:
                    interactions = []
                    for atom1 in value1.atoms:
                        for atom2 in value2.atoms:
                            res_atom1 = ":".join([key1[0],
                                                 atom1.atom_name])
                            res_atom2 = ":".join([key2[0],
                                                 atom2.atom_name])

                            interaction = (res_atom1, res_atom2)

                            if interaction in allowed_distances:

                                allowed_dist = allowed_distances[interaction]

                                x_distance = (atom1.x_coord
                                              - atom2.x_coord) ** 2
                                y_distance = (atom1.y_coord
                                              - atom2.y_coord) ** 2
                                z_distance = (atom1.z_coord
                                              - atom2.z_coord) ** 2
                                distance_sum = (x_distance + y_distance
                                                + z_distance)
                                distance = distance_sum ** 0.5

                                min_dist = allowed_dist[0]
                                max_dist = allowed_dist[1]

                                if min_dist < distance < max_dist:
                                    interactions.append(True)
                                    residues[key1] = residues[key1]._replace(
                                        paired=True)
                            else:
                                interactions.append(False)

                        if any(interactions):
                            new_interaction = (value1, value2.number)
                            if new_interaction not in bpseq_list:
                                bpseq_list.append(new_interaction)

    for key, value in residues.items():
        if value.paired is False:
            bpseq_list.append((value, 0))

    bpseq_list.sort(key=lambda tup: tup[0].number)

    for bp in bpseq_list:
        print(f"{bp[0].number} {bp[0].name} {bp[1]}")


zad01("2ZY6.pdb")
