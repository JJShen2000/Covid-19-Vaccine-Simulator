import random

dump_dir = "../../data/sim_data/vaccine/"
dir_path = "../../data/sim_data/group_tract_age/"
all_age3 = list()
all_age2 = list()
all_age1 = list()
all_age0 = list()
for i in range(368):
    file_path = dir_path + str(i) + "-3.txt"
    with open(file_path, 'r') as f:
        n = int(f.readline().strip())
        p = f.readline().strip().split()
    all_age3 += p

    file_path = dir_path + str(i) + "-2.txt"
    with open(file_path, 'r') as f:
        n = int(f.readline().strip())
        p = f.readline().strip().split()
    all_age2 += p

    file_path = dir_path + str(i) + "-1.txt"
    with open(file_path, 'r') as f:
        n = int(f.readline().strip())
        p = f.readline().strip().split()
    all_age1 += p

    file_path = dir_path + str(i) + "-0.txt"
    with open(file_path, 'r') as f:
        n = int(f.readline().strip())
        p = f.readline().strip().split()
    all_age0 += p

print("Output strategy: age 65+ first")
vaccine_order = list()
random.shuffle(all_age3)
vaccine_order += all_age3
all_others = all_age2 + all_age1 + all_age0
random.shuffle(all_others)
vaccine_order += all_others
with open(dump_dir + "vaccination_doc_age3_first.txt", 'w') as f:
    line = str(len(vaccine_order)) + ' \n'
    for p in vaccine_order:
        line += (p + ' ')
    line += '\n'
    f.write(line)

print("Output strategy: age 19-64 first")
vaccine_order = list()
random.shuffle(all_age2)
vaccine_order += all_age2
all_others = all_age3 + all_age1 + all_age0
random.shuffle(all_others)
vaccine_order += all_others
with open(dump_dir + "vaccination_doc_age2_first.txt", 'w') as f:
    line = str(len(vaccine_order)) + ' \n'
    for p in vaccine_order:
        line += (p + ' ')
    line += '\n'
    f.write(line)

print("Output strategy: age 5-18 first")
vaccine_order = list()
random.shuffle(all_age1)
vaccine_order += all_age1
all_others = all_age3 + all_age2 + all_age0
random.shuffle(all_others)
vaccine_order += all_others
with open(dump_dir + "vaccination_doc_age1_first.txt", 'w') as f:
    line = str(len(vaccine_order)) + ' \n'
    for p in vaccine_order:
        line += (p + ' ')
    line += '\n'
    f.write(line)

print("Output strategy: age 0-4 first")
vaccine_order = list()
random.shuffle(all_age0)
vaccine_order += all_age0
all_others = all_age3 + all_age2 + all_age1
random.shuffle(all_others)
vaccine_order += all_others
with open(dump_dir + "vaccination_doc_age0_first.txt", 'w') as f:
    line = str(len(vaccine_order)) + ' \n'
    for p in vaccine_order:
        line += (p + ' ')
    line += '\n'
    f.write(line)

print("Output strategy: no constraint")
vaccine_order = all_age3 + all_age2 + all_age1 + all_age0
random.shuffle(vaccine_order)
with open(dump_dir + "vaccination_doc_no_constraint.txt", 'w') as f:
    line = str(len(vaccine_order)) + ' \n'
    for p in vaccine_order:
        line += (p + ' ')
    line += '\n'
    f.write(line)

