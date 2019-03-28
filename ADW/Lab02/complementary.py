seq = "AACAACAACAACAACAACAACAACACAGAGGCAGAGATCAATAGAATAGA"

complementary = {
	"A": "T",
	"T": "A",
	"G": "C",
	"C": "G"
}

complementary_seq = "".join([complementary[char] for char in seq])
rna_seq = seq.replace("T", "U")
print(rna_seq)