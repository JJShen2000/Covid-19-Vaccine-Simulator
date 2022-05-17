import os
import random
import copy
import numpy as np
from load_data import load_data

dump_dir = "../../data/sim_data/"

class graph_init:
    def __init__(self):
        self.__load_data()
        self.build_graph()
        self.output_sim_data()

    
    def __load_data(self):
        [   self.num_nodes,          # num. of population(nodes)
            self.num_group_classes,  # num. of types of contact group
            self.num_age_groups,     # num. of age group
            self.contact_probs,      # dim: num_group_classes X num_age_groups X num_age_groups
            self.age_population,     # (list of lists) dim: num_tracts X num_age_groups
            self.worker_flow         # (list of lists) from town to town
        ] = load_data()

        self.num_tracts = len(self.age_population)
        
    
    def build_graph(self):
        ''' Build the graph.
        '''
        # Generating nodes(people)
        print("Generating nodes")
        group_tract_age_dir_path = os.path.join(dump_dir, "group_tract_age")

        if not os.path.exists(group_tract_age_dir_path):
            os.makedirs(group_tract_age_dir_path)

        self.nodes = list()     # self.nodes[i][j]: people in the j-th age group of the i-th tract
        num_curr = 0

        for i, tract in enumerate(self.age_population):
            self.nodes.append(list())
            for j, population_age in enumerate(tract):
                self.nodes[i].append(list(range(num_curr, num_curr+population_age)))
                num_curr += population_age

                # Output
                with  open(os.path.join(group_tract_age_dir_path, f'{i}-{j}.txt'), 'w') as f:
                    line = str(len(self.nodes[i][j])) + ' \n'
                    line += ' '.join(str(p) for p in self.nodes[i][j])
                    line += '\n'
                    f.write(line)
        
        # For output
        self.node_night_tracts = list()
        for i, tract in enumerate(self.age_population):
            for population_age in tract:
                self.node_night_tracts += ([i] * population_age)

        print("Assigning contact groups at night")
        self.num_contact_groups = 0  # num. of group instances
        self.group_type = list()     # Store types of each group by idx.
        # Store contact group indices of each node(person)
        self.node_group_attr = np.full((self.num_nodes, self.num_group_classes), np.nan, dtype='uint64')

        # Assigning contact groups at night
        for i, tract_group in enumerate(self.nodes):
            print("\rProcessing tract: {}/{}".format(i+1, self.num_tracts), end='')
            self._assign_night_group_in_each_tract(tract_group)
        
        print()

        # Worker flow (nodes in night → nodes in day)
        print("Applying worker flow")
        self.nodes_day = copy.deepcopy(self.nodes)
        workers_each_day_tract = [[] for _ in range(self.num_tracts)]

        for tract_idx, tract in enumerate(self.nodes):
            print("\r{}/{}".format(tract_idx+1, self.num_tracts), end='')
            weight_distribution = self.worker_flow[tract_idx]
            for worker in tract[2]:  # tract[2]: The people in age group2(age 19-65)
                day_tract_idx = random.choices(list(range(self.num_tracts)), weights=weight_distribution, k=1)[0]
                workers_each_day_tract[day_tract_idx].append(worker)
        
        print()

        for tract_idx in range(len(self.nodes)):
            self.nodes_day[tract_idx][2] = workers_each_day_tract[tract_idx]

        # Assigning contact groups at day
        print("Assigning contact groups at day")
        for i, tract_group in enumerate(self.nodes_day):
            print("\rProcessing tract: {}/{}".format(i+1, self.num_tracts), end='')
            self._assign_day_group_in_each_tract(tract_group)

        print()


    def _assign_night_group_in_each_tract(self, tract_group):
        '''Assign night group in each tract.
        
        night groups: 
            households = 7 people
            clusters = 4 households
            neighborhoods = 4 clusters

            communities = 2000 people

        Args: 
            tract_group: A list of 4 lists(Each list represents a different age group). 
        Returns:
            None
        '''
        child1, child2 = tract_group[:2]
        adult1 = [x for xs in tract_group[2:7] for x in xs] # itertools.chain()
        adult2 = [x for xs in tract_group[7:] for x in xs] 

        '''Group households (index=0)'''
        people = child1 + child2 + adult1 + adult2
        households = self._assign_contact_group(people, 7)
        for i, group in enumerate(households):
            self.group_type.append(0)
            for p in group:
                self.node_group_attr[p, 0] = i + self.num_contact_groups
        self.num_contact_groups += len(households)

        '''Group clusters (index=1)'''
        hs = list(range(len(households)))
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
        '''Assign day group in each tract.
        
        day groups: 
            age 0-4: play groups, daycares
            age 5-18: elementary schools, middle schools, high schools
            age 19-64: work groups

        Args: 
            tract_group: A list of [a] lists(Each list represents a different age group). 
        Returns:
            None
        '''
        child1, child2 = tract_group[:2]
        adult1 = [x for xs in tract_group[2:7] for x in xs] # itertools.chain()

        '''Group play groups (index=2)'''
        self.__assign_by_group_type(child1, 4, 2)

        '''Group daycares (index=3)'''
        self.__assign_by_group_type(child1, 14, 3)

        '''Group elementary schools (index=4)'''
        self.__assign_by_group_type(child2, 79, 4)

        '''Group middle schools (index=5)'''
        self.__assign_by_group_type(child2, 128, 5)

        '''Group high schools (index=6)'''
        self.__assign_by_group_type(child2, 155, 6)

        '''Group work groups (index=7)'''
        self.__assign_by_group_type(adult1, 20, 7)

    
    def __assign_by_group_type(self, people_list, group_size, group_type_idx):
        people_list_cp = copy.deepcopy(people_list)
        groups = self._assign_contact_group(people_list_cp, group_size)

        for i, group in enumerate(groups):
            self.group_type.append(group_type_idx)
            for p in group:
                self.node_group_attr[p, group_type_idx] = i + self.num_contact_groups
        self.num_contact_groups += len(groups)


    def _assign_contact_group(self, people_list, group_size):
        '''Assign contact group.

        Concept: Split people_list into sublist of length group_size randomly.

        Args:
            people_list: A list of nodes(int: serial numbers). e.g. [1,2,5,7,9]
            group_size: Node number of contact group. 
        Returns:
            groups: A 2D list. len(groups) is the num. of groups. Each element is a group list(list of nodes).
        '''
        random.shuffle(people_list)

        # number of groups
        group_num = len(people_list) // group_size
        remainder = len(people_list) % group_size

        groups = [people_list[group_size*i : group_size*(i+1)] for i in range(group_num)]

        # Assign remainder
        if remainder > 0:
            if remainder > group_num:
                groups.append(people_list[group_size*group_num : ])
            else:
                for r in range(remainder):
                    groups[r%group_num].append(people_list[group_size*group_num + r])

        return groups


    def output_sim_data(self):
        ''' Output the simulation data.
        '''
        print("Generating graph.txt")
        with open(dump_dir + 'graph/graph.txt', 'w') as f:
            line = f'{self.num_nodes} 2 {self.num_contact_groups} {self.num_age_groups} {self.num_group_classes} {self.num_tracts} \n'
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
                        line = f'{j} {self.node_night_tracts[p]} \n'
                        if j == 0:
                            line += f'2 {self.node_group_attr[p, 2]} {self.node_group_attr[p, 3]} \n'
                        elif j == 1:
                            line += f'3 {self.node_group_attr[p, 4]} {self.node_group_attr[p, 5]} {self.node_group_attr[p, 6]} \n'
                        elif j >= 2 and j < 7:
                            line += f'1 {self.node_group_attr[p, 7]} \n'
                        else:
                            line += f'2 {self.node_group_attr[p, 8]} {self.node_group_attr[p, 9]} \n'
                        line += f'4 {self.node_group_attr[p, 0]} {self.node_group_attr[p, 1]} {self.node_group_attr[p, 8]} {self.node_group_attr[p, 9]} \n'
                        f.write(line)


if __name__ == '__main__':
    graph_init()
