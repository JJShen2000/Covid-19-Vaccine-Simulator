import os
import pandas as pd
import math
import numpy as np

worker_population_by_town = []

def handle_wf(x):
    # print(x)
    ratio = x.sum() / worker_population_by_town[x.name]
    x /= ratio
    return x

def get_n(df_age_population):
    """ Return the number of population(nodes)
    """
    return df_age_population[3].sum()

def get_contact_group_probabilities(df_age_population, df_contact_prob, df_census_tracts):
    contact_group_list = df_contact_prob['Contact group'].unique()
    m = len(contact_group_list)
    contact_group_list

    age_group = df_contact_prob[df_contact_prob.iloc[:, 2].str.contains(' ')].iloc[:, 2].unique()
    a = len(age_group)
    age_group_prefix = set([g.split()[0] for g in list(age_group)])
    # age_group
    dict_age_group_prefix = {}
    for prefix in age_group_prefix:
        dict_age_group_prefix[prefix] = [] 

    dict_age_group_prefix = {'Anyone': []}
    age_group_to_number = {}
    for i, g in enumerate(age_group):
        prefix = g.split()[0]
        if prefix in dict_age_group_prefix:
            dict_age_group_prefix[prefix].append(g)
        else:
            dict_age_group_prefix[prefix] = [g]
        dict_age_group_prefix['Anyone'].append(g)
        age_group_to_number[g] = i

    contact_group_probabilities = []
    for contact_group in contact_group_list:
        contact_prob_dict = {}
        for i in range(a):
            for j in range(a):
                contact_prob_dict[(i,j)] = 0
        df_contact_group = df_contact_prob[df_contact_prob.iloc[:, 0] == contact_group]
        for _, df_row in df_contact_group.iterrows():
            if len(df_row[1].split()) == 1:
                for age_i in dict_age_group_prefix[df_row[1]]:
                    i = age_group_to_number[age_i]
                    if len(df_row[2].split()) == 1:
                        for age_j in dict_age_group_prefix[df_row[2]]:
                            j = age_group_to_number[age_j]
                            contact_prob_dict[(i, j)] = df_row[3]
                    else:
                        j = age_group_to_number[df_row[2]]
                        contact_prob_dict[(i, j)] = df_row[3]
            else:
                i = age_group_to_number[df_row[1]]
                if len(df_row[2].split()) == 1:
                    for age_j in dict_age_group_prefix[df_row[2]]:
                        j = age_group_to_number[age_j]
                        contact_prob_dict[(i, j)] = df_row[3]
                else:
                    j = age_group_to_number[df_row[2]]
                    contact_prob_dict[(i, j)] = df_row[3]                     

        contact_group_probabilities.append(contact_prob_dict)
    return a, m, contact_group_probabilities


def get_age_population(df_census_tracts, df_age_population, seperate_ages=[5, 19, 65]):
    """Get the populations of each census tract
    Args:
        2 dataframe: 
            df_census_tracts
            df_age_population
        1 list:
            seperate_ages
    Returns: 
        A list of lists
    """
    age_population = []
    for _, df_row in df_census_tracts.iterrows():
        mask_same_contract = (df_age_population.iloc[:, 0] == df_row[0]) & (df_age_population.iloc[:, 1] == df_row[1])
        df_contract = df_age_population[mask_same_contract]
        age_population_list = []
        
        for i, age in enumerate(seperate_ages):
            if i == 0:
                age_population_list.append(df_contract[df_contract[2] < age][3].sum())
                age_population_list.append(df_contract[(df_contract[2] >= age) & (df_contract[2] < seperate_ages[i+1])][3].sum())
            elif i == len(seperate_ages)-1:
                age_population_list.append(df_contract[df_contract[2] >= age][3].sum())
            else:
                age_population_list.append(df_contract[(df_contract[2] >= age) & (df_contract[2] < seperate_ages[i+1])][3].sum())
        # print(age_population_list)
        # sum_age_population_list = sum(age_population_list)
        # print(df_contract[mask_same_contract][0].iloc[0], df_contract[mask_same_contract][1].iloc[0])
        # s = df_contract[mask_same_contract][3].sum()
        # assert sum_age_population_list == s
        age_population.append(age_population_list)
    return age_population


def get_worker_flow(df_age_population, df_city_to_city_commute, df_city, df_town):
    global worker_population_by_town
    worker_population_by_town = df_age_population[(df_age_population[2]>=19) & (df_age_population[2]<65)].drop([2], axis=1).groupby([0, 1], sort=False).sum()
    worker_population_by_city = worker_population_by_town.groupby([0], sort=False).sum()
    orther_area_worker_population = worker_population_by_city.iloc[-2] + worker_population_by_city.iloc[-1]
    int(orther_area_worker_population)

    new_c2c = df_city_to_city_commute.iloc[:, 1:]

    new_c2c["金門縣"] = new_c2c["其他地區"]*int(worker_population_by_city.iloc[-2])//int(orther_area_worker_population)
    new_c2c["連江縣"] = new_c2c["其他地區"]-new_c2c["金門縣"]
    new_c2c = new_c2c.drop(["其他地區"], axis=1)
    for i in range(20):
        new_c2c.iloc[i] = new_c2c.iloc[i]/new_c2c.iloc[i].sum()
    # new_c2c.append(new_c2c["金門縣"], ignore_index=True)
    # pd.concat([new_c2c, new_c2c["金門縣"]], ignore_index = True, axis = 0)
    new_row1 = pd.DataFrame(new_c2c["金門縣"])
    new_row1.loc[20]=1-new_row1.sum()
    new_row1.loc[21]=0
    new_row1 = new_row1.transpose()
    new_row1.columns = new_c2c.columns

    new_row2 = pd.DataFrame(new_c2c["連江縣"])
    new_row2.loc[20]=0
    new_row2.loc[21]=1-new_row2.sum()
    new_row2 = new_row2.transpose()
    new_row2.columns = new_c2c.columns

    # new_c2c.loc[20] = new_row1
    # new_c2c.loc[21] = new_row2

    wf_c2c = pd.concat([new_c2c, new_row1, new_row2], axis=0)
    # new_row1
    wf_c2c = wf_c2c.T.values.tolist()
    worker_population_by_city = worker_population_by_city[3].values.tolist()

    worker_population_by_town = list(worker_population_by_town[3])

    city_enocding = {}
    for i, city in enumerate(list(df_city[0])):
        city_enocding[city] = i
    town2citycode=[]
    for town_city in list(df_town[0]):
        town2citycode.append(city_enocding[town_city])
        
    n = 368
    wf_t2t = [[0 for _ in range(n)] for _ in range(n)]

    for i in range(368):
        for j in range(368):
            wf_t2t[i][j] = math.floor(worker_population_by_town[i] * (wf_c2c[town2citycode[i]][town2citycode[j]] * (worker_population_by_town[j]/worker_population_by_city[town2citycode[j]])))
    
    df_wf_t2t = pd.DataFrame(wf_t2t)
    df_wf_t2t = df_wf_t2t.apply(handle_wf, axis=1)

    # # Checking the correctness of overwriting
    # df_wf_t2t2 = pd.DataFrame(wf_t2t)
    # for i in range(368):
    #     ratio = df_wf_t2t2.iloc[i].sum() / worker_population_by_town[i]
    #     df_wf_t2t2.iloc[i] /= ratio

    # print(df_wf_t2t2.equals(df_wf_t2t))

    df_wf_t2t = df_wf_t2t.apply(np.floor).astype(int)
    wf = df_wf_t2t.T.values.tolist()

    for i in range(368):
        wf[i][i] += worker_population_by_town[i]-df_wf_t2t.iloc[i].sum()
        
    for i in range(368):
        for j in range(368):
            if str(type(wf[i][j])) == '<class \'numpy.int64\'>':
                wf[i][j] = wf[i][j].item()
    
    return wf



def load_data(dataPath=None):
    """Load data
    Returns:
        n, m, a, contact_group_probabilities, age_population, worker_flow

        n (int): the number of population(nodes)
        m (int): number of group class
        a (int): number of age group
        contact_group_probabilities (a list of dictionaries)
        age_population(list of lists)
        worker_flow(list of lists) from town to town
    """
    if dataPath is None:
        dataPath = os.path.join('../../data/init_data/')
    df_age_population = pd.read_csv(os.path.join(dataPath, 'age_population.csv'), header=None)
    df_contact_prob = pd.read_csv(os.path.join(dataPath, 'contact_prob.csv'), skipinitialspace=True)
    df_census_tracts = pd.read_csv(os.path.join(dataPath, 'census_tracts.csv'), header=None) 
    df_city_to_city_commute = pd.read_csv(os.path.join(dataPath, 'city_to_city_commute.csv'))
    df_city = pd.read_csv(os.path.join(dataPath, 'city.csv'), header=None)

    n = get_n(df_age_population) 
    a, m, contact_group_probabilities = get_contact_group_probabilities(df_age_population, df_contact_prob, df_census_tracts)
    age_population = get_age_population(df_census_tracts, df_age_population)
    worker_flow = get_worker_flow(df_age_population, df_city_to_city_commute, df_city, df_census_tracts)
    
    return n, m, a, contact_group_probabilities, age_population, worker_flow


if __name__ == '__main__':
    load_data()
