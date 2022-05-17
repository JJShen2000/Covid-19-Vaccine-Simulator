import os
import pandas as pd
import csv

# file/dir path
repo_dir_path = os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "..", "..", "..")
data_dir_path = os.path.join(repo_dir_path, "data")

vis_data_dir_path = os.path.join(data_dir_path, "vis_data")

census_tracts_path = os.path.join(repo_dir_path, "data", "init_data", "census_tracts.csv")
age_population_path = os.path.join(repo_dir_path, "data", "init_data", "age_population.csv")

# Util functions
def generate_region_map_dict():
    regions_dict = {}
    with open(census_tracts_path, newline='', encoding="utf-8") as csvfile:
        spamreader = csv.reader(csvfile, delimiter=',', quotechar='|')
        for i, row in enumerate(spamreader):
            regions_dict[i] = (str(row[0]).replace('臺', '台'), str(row[1]))
    
    return regions_dict


def get_populationOfTaiwan():
    df_age_population = pd.read_csv(os.path.join(age_population_path), header=None)
    return df_age_population[3].sum()


def get_population_dict_by_country():
    df_age_population = pd.read_csv(os.path.join(age_population_path), header=None).drop([1], axis=1)
    pop_by_country_age = dict.fromkeys(df_age_population[0].unique().tolist())
    age_group_boundary = [(0, 4), (5, 18), (19, 64), (65, 100)]
    for bound in age_group_boundary:
        mask = (df_age_population[2] <= bound[1]) & (df_age_population[2] >= bound[0])
        temp = df_age_population[mask].drop([2], axis=1).groupby([0], sort=False, as_index = False).sum().set_index(0)

        for country in pop_by_country_age.keys():
            if pop_by_country_age[country] is None:
                pop_by_country_age[country] = []
            pop_by_country_age[country].append(temp.loc[country][3])
            
    temp = df_age_population.drop([2], axis=1).groupby([0], sort=False, as_index = False).sum().set_index(0)

    for country in pop_by_country_age.keys():
        pop_by_country_age[country].append(temp.loc[country][3])
    
    return pop_by_country_age


