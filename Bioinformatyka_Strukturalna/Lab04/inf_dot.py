import argparse


parser = argparse.ArgumentParser(description="Zadanie z Laboratorium #2 z Bioinformatyki Strukturalnej")
parser.add_argument("target", help="plik Dot-bracket z czasteczka referencyjna")
parser.add_argument("model", help="plik Dot-bracket z modelem")
args = parser.parse_args()


def zad03(target, model):
    dot_bracket_target = ""
    dot_bracket_model = ""
    pairs_target = []
    pairs_model = []

    with open(target, 'r') as f:
        dot_bracket_target = f.readline().strip()

    with open(model, 'r') as f:
        dot_bracket_model = f.readline().strip()

    pairs_stack_target = []
    pairs_stack_model = []

    for i, char in enumerate(dot_bracket_target):
        if char == '(':
            pairs_stack_target.append(i)
        if char == ')':
            start = pairs_stack_target.pop()
            end = i
            pairs_target.append((start, end))

    for i, char in enumerate(dot_bracket_model):
        if char == '(':
            pairs_stack_model.append(i)
        if char == ')':
            start = pairs_stack_model.pop()
            end = i
            pairs_model.append((start, end))

    pairs_target = set(pairs_target)
    pairs_model = set(pairs_model)
    tp = pairs_target & pairs_model  # Czesc wspolna
    fp = pairs_model - pairs_target  # Roznica zbiorow
    fn = pairs_target - pairs_model

    ppv = len(tp) / (len(tp) + len(fp))
    sty = len(tp) / (len(tp) + len(fn))
    inf = round((ppv * sty) ** 0.5, 2)
    print(inf)


zad03(args.target, args.model)
