import argparse
import json

parser = argparse.ArgumentParser(description="Zadanie 2 z Laboratorium #3 \
                                 z Bioinformatyki Strukturalnej")
parser.add_argument("json", help="plik JSON")
args = parser.parse_args()


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

    for key in data_readable_nts.keys():
        chain_dict = data_readable_nts[key]
        for i in range(len(chain_dict) - 5):
            first_nt = chain_dict[i][0]
            first_nt_letter = chain_dict[i][2]
            first_nt_number = chain_dict[i][3]
            last_nt = chain_dict[i+5][0]
            last_nt_letter = chain_dict[i+5][2]
            last_nt_number = chain_dict[i+5][3]
            first_pair = [tup for tup in data_readable_pairs
                          if tup[1] == first_nt
                          and tup[2] == last_nt
                          and tup[4] == "cWW"]
            if first_pair:
                second_nt = chain_dict[i+1][0]
                second_nt_letter = chain_dict[i+1][2]
                penultimate_nt = chain_dict[i+4][0]
                penultimate_nt_letter = chain_dict[i+4][2]
                second_pair = [tup for tup in data_readable_pairs
                               if tup[1] == second_nt
                               and tup[2] == penultimate_nt
                               and tup[4] == "tSH"]
                if second_pair:
                    unpaired_nt1 = chain_dict[i+2][0]
                    unpaired_nt1_letter = chain_dict[i+2][2]
                    unpaired_nt2 = chain_dict[i+3][0]
                    unpaired_nt2_letter = chain_dict[i+3][2]
                    unpaired = [tup for tup in data_readable_pairs
                                if tup[1] == unpaired_nt1
                                and tup[2] == unpaired_nt2]
                    if not unpaired:
                        seq_list = [first_nt_letter, second_nt_letter,
                                    unpaired_nt1_letter, unpaired_nt2_letter,
                                    penultimate_nt_letter, last_nt_letter]
                        seq_string = "".join(seq_list)

                        print(first_nt_number, seq_string, last_nt_number)


zad02(args.json)
