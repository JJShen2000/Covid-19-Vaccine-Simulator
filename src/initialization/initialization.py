import os
import random
import numpy as np
from load_data import load_data

dump_dir = "../../data/sim_data/basic/"

class graph_init:
    def __init__(self):
        self.data = load_data()
        self.build_graph()
        self.output_sim_data()
    
    def build_graph(self):
        ''' Build the graph.
        Return: the graph
        '''
        self.num_nodes = self.data[0]
        self.num_group_classes = self.data[1]
        self.num_age_groups = self.data[2]
        self.contact_probs = self.data[3]
        self.age_population = self.data[4]
        self.worker_flow = self.data[5]

        self.num_tracts = len(self.age_population)

        print("Generating nodes")
        self.node_night_tracts = list()
        self.nodes = list()     # self.nodes[i][j]: people in the j-th age group of the i-th tract
        num_curr = 0
        if not os.path.exists(dump_dir + "group_tract_age"):
            os.makedirs(dump_dir + "group_tract_age")
        for i, tract in enumerate(self.age_population):
            self.nodes.append(list())
            for j, age in enumerate(tract):
                self.nodes[i].append(list(range(num_curr, num_curr+age)))
                self.node_night_tracts += ([i] * age)
                num_curr += age
                with open(dump_dir + "group_tract_age/" + str(i) + "-" + str(j) + ".txt", 'w') as f:
                    line = str(len(self.nodes[i][j])) + ' \n'
                    for p in self.nodes[i][j]:
                        line += (str(p) + ' ')
                    line += '\n'
                    f.write(line)

        print("Assigning contact groups at night")
        self.num_contact_groups = 0
        self.group_type = list()
        self.node_group_attr = np.full((self.num_nodes, self.num_group_classes), np.nan, dtype='uint64')
        for i, tract_group in enumerate(self.nodes):
            print("Processing tract: {}/{}".format(i+1, self.num_tracts))
            self._assign_night_group_in_each_tract(tract_group)

        print("Applying worker flow")
        self.node_day_tracts = self.node_night_tracts[:]
        workers_each_day_tract = list()
        for i in range(self.num_tracts):
            workers_each_day_tract.append(list())
        for tract_idx, tract in enumerate(self.nodes):
            print("{}/{}".format(tract_idx+1, self.num_tracts))
            weight_distribution = self.worker_flow[tract_idx]
            for worker in tract[2]:
                day_tract_idx = random.choices(list(range(self.num_tracts)), weights=weight_distribution, k=1)[0]
                workers_each_day_tract[day_tract_idx].append(worker)
        for tract_idx in range(len(self.nodes)):
            self.nodes[tract_idx][2] = workers_each_day_tract[tract_idx]
            for wk in workers_each_day_tract[tract_idx]:
                self.node_day_tracts[wk] = tract_idx

        print("Assigning contact groups at day")
        for i, tract_group in enumerate(self.nodes):
            print("Processing tract: {}/{}".format(i+1, self.num_tracts))
            self._assign_day_group_in_each_tract(tract_group)

    def _assign_night_group_in_each_tract(self, tract_group):
        child1, child2, adult1, adult2 = tract_group

        '''Group households (index=0)'''
        people = child1 + child2 + adult1 + adult2
        households = self._assign_contact_group(people, 7)
        for i, group in enumerate(households):
            self.group_type.append(0)
            for p in group:
                self.node_group_attr[p, 0] = i + self.num_contact_groups
        self.num_contact_groups += len(households)

        '''Group clusters (index=1)'''
        hs = list(range(0, len(households)))
        clusters = self._assign_contact_group(hs, 4)
        for i, group in enumerate(clusters):
            self.group_type.append(1)
            for h in group:
                for p in households[h]:
                    self.node_group_attr[p, 1] = i + self.num_contact_groups
        self.num_contact_groups += len(clusters)

        '''Group neighborhoods (index=8)'''
        hcs = list(range(0, len(clusters)))
        neighborhoods = self._assign_contact_group(hcs, 4)
        for i, group in enumerate(neighborhoods):
            self.group_type.append(8)
            for hc in group:
                for h in clusters[hc]:
                    for p in households[h]:
                        self.node_group_attr[p, 8] = i + self.num_contact_groups
        self.num_contact_groups += len(neighborhoods)

        '''Group communities (index=9)'''
        people = child1 + child2 + adult1 + adult2
        communities = self._assign_contact_group(people, 2000)
        for i, group in enumerate(communities):
            self.group_type.append(9)
            for p in group:
                self.node_group_attr[p, 9] = i + self.num_contact_groups
        self.num_contact_groups += len(communities)

    def _assign_day_group_in_each_tract(self, tract_group):
        child1, child2, adult1, adult2 = tract_group

        '''Group play groups (index=2)'''
        child_1 = child1[:]
        play_groups = self._assign_contact_group(child_1, 4)
        for i, group in enumerate(play_groups):
            self.group_type.append(2)
            for p in group:
                self.node_group_attr[p, 2] = i + self.num_contact_groups
        self.num_contact_groups += len(play_groups)

        '''Group daycares (index=3)'''
        child_1 = child1[:]
        daycares = self._assign_contact_group(child_1, 14)
        for i, group in enumerate(daycares):
            self.group_type.append(3)
            for p in group:
                self.node_group_attr[p, 3] = i + self.num_contact_groups
        self.num_contact_groups += len(daycares)

        '''Group elementary schools (index=4)'''
        child_2 = child2[:]
        elem_schools = self._assign_contact_group(child_2, 79)
        for i, group in enumerate(elem_schools):
            self.group_type.append(4)
            for p in group:
                self.node_group_attr[p, 4] = i + self.num_contact_groups
        self.num_contact_groups += len(elem_schools)

        '''Group middle schools (index=5)'''
        child_2 = child2[:]
        mid_schools = self._assign_contact_group(child_2, 128)
        for i, group in enumerate(mid_schools):
            self.group_type.append(5)
            for p in group:
                self.node_group_attr[p, 5] = i + self.num_contact_groups
        self.num_contact_groups += len(mid_schools)

        '''Group high schools (index=6)'''
        child_2 = child2[:]
        high_schools = self._assign_contact_group(child_2, 155)
        for i, group in enumerate(high_schools):
            self.group_type.append(6)
            for p in group:
                self.node_group_attr[p, 6] = i + self.num_contact_groups
        self.num_contact_groups += len(high_schools)

        '''Group work groups (index=7)'''
        worker = adult1[:]
        work_groups = self._assign_contact_group(worker, 20)
        for i, group in enumerate(work_groups):
            self.group_type.append(7)
            for p in group:
                self.node_group_attr[p, 7] = i + self.num_contact_groups
        self.num_contact_groups += len(work_groups)

    def output_sim_data(self):
        print("Generating graph.txt")
        with open(dump_dir + 'graph.txt', 'w') as f:
            line = str(self.num_nodes) + ' 2 ' + str(self.num_contact_groups) + ' ' + str(self.num_age_groups) + ' ' + str(self.num_group_classes) + ' ' + str(self.num_tracts) + ' \n'
            f.write(line)

            line = ''
            for cg_type in self.group_type:
                line += (str(cg_type) + ' ')
            line += '\n'
            f.write(line)

            line = ''
            for probs_each_group in self.contact_probs:
                for i in range(self.num_age_groups):
                    for j in range(self.num_age_groups):
                        prob = probs_each_group[(i, j)]
                        line += (str(prob) + ' ')
                    line += '\n'
            f.write(line)

            for tract_group in self.nodes:
                for j, age_group in enumerate(tract_group):
                    for p in age_group:
                        line = str(j) + ' ' + str(self.node_night_tracts[p]) + ' \n'
                        if j == 0:
                            line += ('2 ' + str(self.node_group_attr[p, 2]) + ' ' + str(self.node_group_attr[p, 3]) + ' \n')
                        elif j == 1:
                            line += ('3 ' + str(self.node_group_attr[p, 4]) + ' ' + str(self.node_group_attr[p, 5]) + ' ' + str(self.node_group_attr[p, 6]) + ' \n')
                        elif j == 2:
                            line += ('1 ' + str(self.node_group_attr[p, 7]) + ' \n')
                        else:
                            line += ('2 ' + str(self.node_group_attr[p, 8]) + ' ' + str(self.node_group_attr[p, 9]) + ' \n')
                        line += ('4 ' + str(self.node_group_attr[p, 0]) + ' ' + str(self.node_group_attr[p, 1]) + ' ' + str(self.node_group_attr[p, 8]) + ' ' + str(self.node_group_attr[p, 9]) + ' \n')
                        f.write(line)

    def _assign_contact_group(self, obj, size):
        groups = list()
        random.shuffle(obj)
        group_num = len(obj) // size
        remainder = len(obj) % size
        for i in range(group_num):
            groups.append(obj[size*i : size*(i+1)])

        if remainder > 0:
            if remainder > group_num:
                groups.append(obj[size*group_num : ])
            else:
                for r in range(remainder):
                    groups[r%group_num].append(obj[size*group_num + r])

        return groups

if __name__ == '__main__':
    graph_init()
