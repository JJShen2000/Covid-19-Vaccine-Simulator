import random
import load_data.load_data

class _base_initialization_module:
    def __init__(self, fname):
        self.data = load_data()
        build_graph()
        output_sim_data()
    
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
        for tract in age_populaltion:
            self.attr_list += self._assign_attr_in_each_tract(tract)
    
    def _assign_attr_in_each_tract(self, tract):
        '''
        - 建人數的node
        - 根據年齡分布給age
        - 根據contact group給group attribute
        - 根據contact matrix給contact group內的node建邊
        '''
        num_child1, num_child2, num_adult1, num_adult2 = self.age_population[tract]
        num_children = num_child1 + num_child2
        num_adults = num_adult1 + num_adult2
        num_people = num_children + num_adults

        group_list = [{"location": tract, "age_group": 0}] * num_child1
        group_list += ([{"location": tract, "age_group": 1}] * num_child2)
        group_list += ([{"location": tract, "age_group": 2}] * num_adult1)
        group_list += ([{"location": tract, "age_group": 3}] * num_adult2)

        '''Group households (index=0)'''
        people = list(range(0, num_people))
        households = self._assign_contact_group(people, 7)
        for i, group in enumerate(households):
            for p in group:
                group_list[p][0] = i + self.num_each_group[0]
        self.num_each_group[0] += len(households)

        '''Group clusters (index=1)'''
        hs = list(range(0, len(households)))
        clusters = _assign_contact_group(hs, 4)
        for i, group in enumerate(clusters):
            for h in group:
                for p in households[h]:
                    group_list[p][1] = i + self.num_each_group[1]
        self.num_each_group[1] += len(clusters)

        '''Group neighborhoods (index=8)'''
        hcs = list(range(0, len(clusters)))
        neighborhoods = _assign_contact_group(hcs, 4)
        for i, group in enumerate(neighborhoods):
            for hc in group:
                for h in clusters[hc]:
                    for p in households[h]:
                        group_list[p][8] = i + self.num_each_group[8]
        self.num_each_group[8] += len(neighborhoods)

        '''Group communities (index=9)'''
        people = list(range(0, num_people))
        communities = _assign_contact_group(people, 2000)
        for i, group in enumerate(communities):
            for p in group:
                group_list[p][9] = i + self.num_each_group[9]
        self.num_each_group[9] += len(communities)

        '''Group play groups (index=2)'''
        child1 = list(range(0, num_child1))
        play_groups = _assign_contact_group(child1, 4)
        for i, group in enumerate(play_groups):
            for p in group:
                group_list[p][2] = i + self.num_each_group[2]
        self.num_each_group[2] += len(play_groups)

        '''Group daycares (index=3)'''
        child1 = list(range(0, num_child1))
        daycares = _assign_contact_group(child1, 14)
        for i, group in enumerate(daycares):
            for p in group:
                group_list[p][3] = i + self.num_each_group[3]
        self.num_each_group[3] += len(daycares)

        '''Group elementary schools (index=4)'''
        child2 = list(range(num_child1, num_child2))
        elem_schools = _assign_contact_group(child2, 79)
        for i, group in enumerate(elem_schools):
            for p in group:
                group_list[p][4] = i + self.num_each_group[4]
        self.num_each_group[4] += len(elem_schools)

        '''Group middle schools (index=5)'''
        child2 = list(range(num_child1, num_child2))
        mid_schools = _assign_contact_group(child2, 128)
        for i, group in enumerate(mid_schools):
            for p in group:
                group_list[p][5] = i + self.num_each_group[5]
        self.num_each_group[5] += len(mid_schools)

        '''Group high schools (index=6)'''
        child2 = list(range(num_child1, num_child2))
        high_schools = _assign_contact_group(child2, 155)
        for i, group in enumerate(high_schools):
            for p in group:
                group_list[p][6] = i + self.num_each_group[6]
        self.num_each_group[6] += len(high_schools)

        '''Group work groups (index=7)'''
        adult1 = list(range(num_children, num_adult1))
        work_groups = _assign_contact_group(adult1, 20)
        for i, group in enumerate(work_groups):
            for p in group:
                group_list[p][7] = i + self.num_each_group[7]
        self.num_each_group[7] += len(work_groups)

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
        with open('graph.txt', 'w') as f:
            line = str(self.num_nodes) + ' ' + str(self.num_group_classes) + ' ' + str(self.num_age_groups) + ' '
            f.write(line)

            line = ''
            for num in self.num_each_group:
                line += (str(num) + ' ')
            f.write(line)

            line = ''
            for probs_each_group in self.contact_probs:
                for i in range(self.num_age_groups):
                    for j in range(self.num_age_groups):
                        prob = probs_each_group[(i, j)]
                        line += (str(prob) + ' ')
                    line += '\n'
            f.write(line)

            for i, node_attr in enumerate(self.attr_list):
                line = str(i) + ' '
                group_list = list()
                for i in range(self.num_group_classes):
                    if i in node_attr:
                        group_list.append(str(node_attr[i]))
                line += (str(len(group_list)) + ' ' + ' '.join(group_list) + ' ') * 2
                f.write(line)

    def _assign_contact_group(obj, size):
        groups = list()
        random.shuffle(obj)
        for i in range(len(obj) // size):
            groups.append(obj[size*i : size*(i+1)])
        groups.append(obj[size*(len(obj)//size) : ])
        return groups

if __name__ == '__main__':
    _base_initialization_module()
