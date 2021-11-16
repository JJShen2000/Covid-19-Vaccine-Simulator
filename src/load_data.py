import os
import pandas as pd

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


def load_data(dataPath=None):
    """Load data
    Returns:
        n, m, a, contact_group_probabilities, age_population

        n (int): the number of population(nodes)
        m (int): number of group class
        a (int): number of age group
        contact_group_probabilities (a list of dictionaries)
        age_population(list of lists)
    """
    if dataPath is None:
        dataPath = os.path.join('..','data')
    df_age_population = pd.read_csv(os.path.join(dataPath, 'age_population.csv'), header=None)
    df_contact_prob = pd.read_csv(os.path.join(dataPath, 'contact_prob.csv'), skipinitialspace=True)
    df_census_tracts = pd.read_csv(os.path.join(dataPath, 'census_tracts.csv'), header=None) 

    n = get_n(df_age_population) 
    a, m, contact_group_probabilities = get_contact_group_probabilities(df_age_population, df_contact_prob, df_census_tracts)
    age_population = get_age_population(df_census_tracts, df_age_population)
    return n, m, a, contact_group_probabilities, age_population


if __name__ == '__main__':
    load_data()
