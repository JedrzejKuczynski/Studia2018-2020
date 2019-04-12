import argparse
import json

"""parser = argparse.ArgumentParser(description="Zadanie 2 z Laboratorium #3 \
                                 z Bioinformatyki Strukturalnej")
parser.add_argument("json", help="plik JSON")
args = parser.parse_args()"""


def zad02(pdb):
    with open(pdb, 'r') as f:
        data = json.load(f)

    data_readable_pairs = []
    data_readable_nts = {}

    for pair_dict in data["pairs"]:
        index = pair_dict["index"]
        nt1 = pair_dict["nt1"]
        nt2 = pair_dict["nt2"]
        bp = pair_dict["bp"]
        pair_type = pair_dict["LW"]
        pair_tuple = (index, nt1, nt2, bp, pair_type)
        data_readable_pairs.append(pair_tuple)

    for nt_dict in data["nts"]:
        index = nt_dict["index"]
        chain_name = nt_dict["chain_name"]
        nt_id = nt_dict["nt_id"]
        nt_code = nt_dict["nt_code"]
        nt_tuple = (nt_id, chain_name, nt_code, index)

        if chain_name not in data_readable_nts.keys():
            data_readable_nts[chain_name] = []
            data_readable_nts[chain_name].append(nt_tuple)
        else:
            data_readable_nts[chain_name].append(nt_tuple)

    # print(data_readable_pairs)
    # print(data_readable_nts)

    for i in range(len(data_readable_pairs) - 1):
        first_nt = data_readable_pairs[i]
        second_nt = data_readable_pairs[i+1]
        if first_nt[4] == "cWW" and second_nt[4] == "tSH":
            print(first_nt, second_nt)


zad02("1RLG.json")
