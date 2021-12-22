import random
import numpy as np
import pandas as pd

dump_dir = "../../data/sim_data/vaccine/"
dir_path = "../../data/sim_data/group_tract_age/"

print("Loading demographic data")
num_vaccine = 0     # should include all people later
people_each_tract_age = np.empty((368, 4), dtype=object)
for i in range(368):
    for j in range(4):
        file_path = dir_path + str(i) + "-" + str(j) + ".txt"
        with open(file_path, 'r') as f:
            n = int(f.readline().strip())
            p = f.readline().strip().split()
        num_vaccine += n
        people_each_tract_age[i, j] = p
people_each_age = np.sum(people_each_tract_age, axis=0)
people_each_tract = np.sum(people_each_tract_age, axis=1)

# Prioritize different age groups
for i in range(4):
    print("Output strategy: age group {} first".format(i))
    prioritized = list()
    all_others = list()
    for j in range(4):
        if j == i:
            prioritized += people_each_age[j]
        else:
            all_others += people_each_age[j]
    random.shuffle(prioritized)
    random.shuffle(all_others)
    output_filepath = dump_dir + "vaccination_doc_age" + str(i) + "_first.txt"
    with open(output_filepath, 'w') as f:
        line = str(num_vaccine) + ' \n'
        for p in prioritized:
            line += (p + ' ')
        for p in all_others:
            line += (p + ' ')
        line += '\n'
        f.write(line)

# Prioritize specific census tracts
print("Output strategy: specified census tracts first")
df_tracts = pd.read_csv('../../data/init_data/census_tracts.csv', header=None)
tract_dict = dict()
for i in range(len(df_tracts)):
    tract_name = df_tracts.iloc[i, 0]
    if tract_name not in tract_dict:
        tract_dict[tract_name] = list()
    tract_dict[tract_name].append(i)
df_prioritized_tracts = pd.read_csv('../../input/tunables/vacc_prioritized_tracts.csv', header=None)
prioritized = list()
unprioritized_tracts_mask = np.array([True] * 368)
for _, df_row in df_prioritized_tracts.iterrows():
    prioritized_tracts_per_county = tract_dict[df_row[0]]
    unprioritized_tracts_mask[prioritized_tracts_per_county] = False
    prioritized_per_county = list()
    for tract in prioritized_tracts_per_county:
        prioritized_per_county += people_each_tract[tract]
    random.shuffle(prioritized_per_county)
    prioritized += prioritized_per_county
all_others = list()
unprioritized_tracts = np.array(list(range(368)))[unprioritized_tracts_mask]
for tract in unprioritized_tracts:
    all_others += people_each_tract[tract]
random.shuffle(all_others)
with open(dump_dir + "vaccination_doc_tract_based.txt", 'w') as f:
    line = str(num_vaccine) + ' \n'
    for p in prioritized:
        line += (p + ' ')
    for p in all_others:
        line += (p + ' ')
    line += '\n'
    f.write(line)

# Random vaccination
print("Output strategy: no constraint")
vaccine_order = np.sum(people_each_age)
random.shuffle(vaccine_order)
with open(dump_dir + "vaccination_doc_no_constraint.txt", 'w') as f:
    line = str(len(vaccine_order)) + ' \n'
    for p in vaccine_order:
        line += (p + ' ')
    line += '\n'
    f.write(line)

