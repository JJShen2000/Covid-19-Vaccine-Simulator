import random

dump_dir = "../../data/sim_data/"
dir_path =  dump_dir + "group_tract_age/"
total_infected = list()
for i in range(368):
    num_in_tract = 0
    people = list()
    for j in range(4):
        file_path = dir_path + str(i) + "-" + str(j) + ".txt"
        with open(file_path, 'r') as f:
            n = int(f.readline().strip())
            p = f.readline().strip().split()
        num_in_tract += n
        people += p

    max_infected = num_in_tract // 10000
    num_infected = random.randint(0, max_infected)
    infected = random.sample(people, num_infected)
    total_infected += infected

with open(dump_dir + "init_infector/init_infector_doc.txt", 'w') as f:
    line = str(len(total_infected)) + ' \n'
    for p in total_infected:
        line += (p + ' ')
    line += '\n'
    f.write(line)

