import gffutils


def create_database(gff_file, database_name):
    _ = gffutils.create_db(gff_file, dbfn=f"{database_name}.db",
                           force=True, keep_order=True,
                           merge_strategy="merge",
                           sort_attribute_values=True,
                           verbose=True)

    return


# create_database("Ovis_aries.Oar_v3.1.99.chromosome.13.gff3", "ovis_aries_chr13")
# create_database("Capra_hircus.ARS1.99.chromosome.13.gff3", "capra_hircus_chr13")


def get_gene_ids(filename):

    first_species_ids = []
    second_species_ids = []

    with open(filename, 'r') as f:
        f.readline()  # Header
        for line in f:
            splitted = line.strip().split(',')
            if splitted[-1] == "ortholog_one2one":
                first_species_ids.append(splitted[0])
                second_species_ids.append(splitted[1])

    return first_species_ids, second_species_ids


def get_genes(database_first, database_second, gene_ids):
    db_first = gffutils.FeatureDB(database_first, keep_order=True)
    db_second = gffutils.FeatureDB(database_second, keep_order=True)

    genes_first = []
    genes_second = []

    for gene_id_first, gene_id_second in gene_ids:
        gene_first = db_first[f"gene:{gene_id_first}"]
        gene_id_first = gene_first.id
        start_first = gene_first.start
        end_first = gene_first.end
        strand_first = gene_first.strand

        if "description" in gene_first.attributes.keys():
            description_first = gene_first.attributes["description"]
        else:
            description_first = ""

        try:
            gene_second = db_second[f"gene:{gene_id_second}"]
            gene_id_second = gene_second.id
            start_second = gene_second.start
            end_second = gene_second.end
            strand_second = gene_second.strand

            if "description" in gene_second.attributes.keys():
                description_second = gene_second.attributes["description"]
            else:
                description_second = ""

            gene_attributes_first = (gene_id_first, start_first, end_first,
                                     strand_first, description_first)
            genes_first.append(gene_attributes_first)

            gene_attributes_second = (gene_id_second, start_second, end_second,
                                      strand_second, description_second)
            genes_second.append(gene_attributes_second)
        except gffutils.exceptions.FeatureNotFoundError:
            continue

    # print(len(genes_first), len(genes_second))

    return list(zip(genes_first, genes_second))


def analysis(found_orthologs, first_chr_length, second_chr_length, allowed_diff):
    shared_synteny = []
    reversed_synteny = []
    no_synteny = []

    for first, second in found_orthologs:
        if first[3] == second[3]:
            start_diff = abs(first[1] - second[1])
            end_diff = abs(first[2] - second[2])
            if start_diff <= allowed_diff and end_diff <= allowed_diff:
                shared_synteny.append((first, second))
            else:
                no_synteny.append((first, second))
        elif first[3] == '-' and second[3] == '+':
            plus_start = first_chr_length - first[2]
            plus_end = first_chr_length - first[1]
            start_diff = abs(plus_start - second[1])
            end_diff = abs(plus_end - second[2])
            if start_diff <= allowed_diff and end_diff <= allowed_diff:
                reversed_synteny.append((first, second))
            else:
                no_synteny.append((first, second))
        else:
            plus_start = second_chr_length - second[2]
            plus_end = second_chr_length - second[1]
            start_diff = abs(plus_start - first[1])
            end_diff = abs(plus_end - first[2])
            if start_diff <= allowed_diff and end_diff <= allowed_diff:
                reversed_synteny.append((first, second))
            else:
                no_synteny.append((first, second))

    return shared_synteny, reversed_synteny, no_synteny


sheep_ids, goat_ids = get_gene_ids("ovis_aries_chr13_orthogenes_capra_hircus.txt")
orthologs = list(zip(sheep_ids, goat_ids))

found_orthologs = get_genes("ovis_aries_chr13.db", "capra_hircus_chr13.db", orthologs)

# for sheep, goat in found_orthologs:
   # print(sheep, goat)

shared_synteny, reversed_synteny, no_synteny = analysis(found_orthologs, 94_726_778, 94_672_733, 100_000)
print(len(found_orthologs))
print(ss := len(shared_synteny), rs := len(reversed_synteny), ns := len(no_synteny), ss + rs + ns)
