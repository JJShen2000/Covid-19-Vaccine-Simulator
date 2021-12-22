import random
import pandas as pd

df_tracts = pd.read_csv('../../data/init_data/census_tracts.csv', header=None)
tract_dict = dict()
for i in range(len(df_tracts)):
    tract_name = df_tracts.iloc[i, 0] + df_tracts.iloc[i, 1]
    tract_dict[tract_name] = i

num_infected_each_tract = dict()
df_tract_infectors = pd.read_csv('../../input/tunables/tract_init_infectors.csv', header=None)
for _, df_row in df_tract_infectors.iterrows():
    tract_code = tract_dict[df_row[0]]
    num_infected = int(df_row[1])
    num_infected_each_tract[tract_code] = num_infected

dump_dir = "../../data/sim_data/"
dir_path =  dump_dir + "group_tract_age/"
total_infected = list()
for i in num_infected_each_tract.keys():
    num_in_tract = 0
    people = list()
    for j in range(4):
        file_path = dir_path + str(i) + "-" + str(j) + ".txt"
        with open(file_path, 'r') as f:
            n = int(f.readline().strip())
            p = f.readline().strip().split()
        num_in_tract += n
        people += p
    infected = random.sample(people, num_infected_each_tract[i])
    total_infected += infected

with open(dump_dir + "init_infector/init_infector_doc.txt", 'w') as f:
    line = str(len(total_infected)) + ' \n'
    for p in total_infected:
        line += (p + ' ')
    line += '\n'
    f.write(line)

