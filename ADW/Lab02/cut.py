import math
import random

seq = ""
random.seed(42)

with open("abisynczyk.fasta", 'r') as f:
    header = f.readline()
    for line in f:
        seq += line.strip()
        
reads = []
        
for length in [25, 50, 100]:
    for _ in range(3):
        start = random.randint(0, len(seq) - length)
        read = seq[start:start+length]
        read_tuple = (start, read)
        reads.append(read_tuple)

print(reads)

range_counter = 0

for length in [25, 50 , 100]:
	times = math.ceil(length / 70)
	for i in range(range_counter, range_counter+3):
		with open(f"reads_{length}_{i}.fasta", 'w') as f:
			f.write(f">start_{reads[i][0]}\n")
			line_counter = 0
			for _ in range(times):
				f.write(f"{reads[i][1][line_counter:line_counter+70]}\n")
				line_counter += 70
	range_counter += 3
