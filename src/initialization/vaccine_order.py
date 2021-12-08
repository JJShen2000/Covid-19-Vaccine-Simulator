import random

dump_dir = "../../data/sim_data/vaccine/"
dir_path = "../../data/sim_data/basic/group_tract_age/"
all_age3 = list()
all_others = list()
for i in range(368):
    file_path = dir_path + str(i) + "-3.txt"
    with open(file_path, 'r') as f:
        n = int(f.readline().strip())
        p = f.readline().strip().split()
    all_age3 += p

    for j in range(3):
        file_path = dir_path + str(i) + "-" + str(j) + ".txt"
        with open(file_path, 'r') as f:
            n = int(f.readline().strip())
            p = f.readline().strip().split()
        all_others += p

# Strategy 1: age group 3 first
num_vaccine = (len(all_others) + len(all_age3)) // 2
vaccine_order = list()
random.shuffle(all_age3)
vaccine_order += all_age3
num_vaccine -= len(all_age3)
others_vaccine = random.sample(all_others, num_vaccine)
vaccine_order += others_vaccine
with open(dump_dir + "vaccination_doc_age3_first.txt", 'w') as f:
    line = str(len(vaccine_order)) + ' \n'
    for p in vaccine_order:
        line += (p + ' ')
    line += '\n'
    f.write(line)

# Strategy 2: no constraint
num_vaccine = (len(all_others) + len(all_age3)) // 2
all_people = all_others + all_age3
vaccine_order = random.sample(all_people, num_vaccine)
with open(dump_dir + "vaccination_doc_no_constraint.txt", 'w') as f:
    line = str(len(vaccine_order)) + ' \n'
    for p in vaccine_order:
        line += (p + ' ')
    line += '\n'
    f.write(line)

