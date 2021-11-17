import random
from load_data import load_data

class _base_initialization_module:
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

        self.attr_list = list()
        self.num_each_group = [0] * self.num_group_classes
        for tract in range(len(self.age_population)):
            print("Processing tract: {}/{}".format(tract+1, len(self.age_population)))
            self.attr_list += self._assign_attr_in_each_tract(tract)
    
    def _assign_attr_in_each_tract(self, tract):
        num_child1, num_child2, num_adult1, num_adult2 = self.age_population[tract]
        num_children = num_child1 + num_child2
        num_adults = num_adult1 + num_adult2
        num_people = num_children + num_adults

        group_list = list()
        for _ in range(num_child1):
            group_list.append({"location": tract, "age_group": 0})
        for _ in range(num_child2):
            group_list.append({"location": tract, "age_group": 1})
        for _ in range(num_adult1):
            group_list.append({"location": tract, "age_group": 2})
        for _ in range(num_adult2):
            group_list.append({"location": tract, "age_group": 3})

        '''Group households (index=0)'''
        people = list(range(0, num_people))
        households = self._assign_contact_group(people, 7)
        for i, group in enumerate(households):
            for p in group:
                group_list[p][0] = i + self.num_each_group[0]
        self.num_each_group[0] += len(households)

        '''Group clusters (index=1)'''
        hs = list(range(0, len(households)))
        clusters = self._assign_contact_group(hs, 4)
        for i, group in enumerate(clusters):
            for h in group:
                for p in households[h]:
                    group_list[p][1] = i + self.num_each_group[1]
        self.num_each_group[1] += len(clusters)

        '''Group neighborhoods (index=8)'''
        hcs = list(range(0, len(clusters)))
        neighborhoods = self._assign_contact_group(hcs, 4)
        for i, group in enumerate(neighborhoods):
            for hc in group:
                for h in clusters[hc]:
                    for p in households[h]:
                        group_list[p][8] = i + self.num_each_group[8]
        self.num_each_group[8] += len(neighborhoods)

        '''Group communities (index=9)'''
        people = list(range(0, num_people))
        communities = self._assign_contact_group(people, 2000)
        for i, group in enumerate(communities):
            for p in group:
                group_list[p][9] = i + self.num_each_group[9]
        self.num_each_group[9] += len(communities)

        '''Group play groups (index=2)'''
        child1 = list(range(0, num_child1))
        play_groups = self._assign_contact_group(child1, 4)
        for i, group in enumerate(play_groups):
            for p in group:
                group_list[p][2] = i + self.num_each_group[2]
        self.num_each_group[2] += len(play_groups)

        '''Group daycares (index=3)'''
        child1 = list(range(0, num_child1))
        daycares = self._assign_contact_group(child1, 14)
        for i, group in enumerate(daycares):
            for p in group:
                group_list[p][3] = i + self.num_each_group[3]
        self.num_each_group[3] += len(daycares)

        '''Group elementary schools (index=4)'''
        child2 = list(range(num_child1, num_child2))
        elem_schools = self._assign_contact_group(child2, 79)
        for i, group in enumerate(elem_schools):
            for p in group:
                group_list[p][4] = i + self.num_each_group[4]
        self.num_each_group[4] += len(elem_schools)

        '''Group middle schools (index=5)'''
        child2 = list(range(num_child1, num_child2))
        mid_schools = self._assign_contact_group(child2, 128)
        for i, group in enumerate(mid_schools):
            for p in group:
                group_list[p][5] = i + self.num_each_group[5]
        self.num_each_group[5] += len(mid_schools)

        '''Group high schools (index=6)'''
        child2 = list(range(num_child1, num_child2))
        high_schools = self._assign_contact_group(child2, 155)
        for i, group in enumerate(high_schools):
            for p in group:
                group_list[p][6] = i + self.num_each_group[6]
        self.num_each_group[6] += len(high_schools)

        '''Group work groups (index=7)'''
        adult1 = list(range(num_children, num_adult1))
        work_groups = self._assign_contact_group(adult1, 20)
        for i, group in enumerate(work_groups):
            for p in group:
                group_list[p][7] = i + self.num_each_group[7]
        self.num_each_group[7] += len(work_groups)

        '''Turn the group list into a string'''
        for idx, node_attr in enumerate(group_list):
            line = str(node_attr["age_group"]) + ' '
            g_list = list()
            for i in range(self.num_group_classes):
                if i in node_attr:
                    g_list.append(str(node_attr[i]))
            line += (str(len(g_list)) + ' ' + ' '.join(g_list) + ' ') * 2
            line += '\n'
            group_list[idx] = line

        return group_list
    
    def _assign_worker_edge_between_tract(self, G):
        '''
        - % of worker with diff day and night?
        - distribution to assign location of day and night?
        '''
        pass
    
    def _assign_initial_infectors(self, G):
        pass

    def output_sim_data(self):
        print("Generating graph.txt")
        with open('graph.txt', 'w') as f:
            line = str(self.num_nodes) + ' ' + str(self.num_group_classes) + ' ' + str(self.num_age_groups) + ' \n'
            f.write(line)

            line = ''
            for num in self.num_each_group:
                line += (str(num) + ' ')
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

            for node in self.attr_list:
                f.write(node)

    def _assign_contact_group(self, obj, size):
        groups = list()
        random.shuffle(obj)
        for i in range(len(obj) // size):
            groups.append(obj[size*i : size*(i+1)])
        groups.append(obj[size*(len(obj)//size) : ])
        return groups

if __name__ == '__main__':
    _base_initialization_module()
