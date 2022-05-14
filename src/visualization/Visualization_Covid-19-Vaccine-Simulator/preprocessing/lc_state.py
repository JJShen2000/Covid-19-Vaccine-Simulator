import os
import glob
import sys
import pandas as pd
import csv
import shutil
import numpy as np

# file/dir path
repo_dir_path = os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "..", "..", "..")
data_dir_path = os.path.join(repo_dir_path, "data")

vis_data_dir_path = os.path.join(data_dir_path, "vis_data")

census_tracts_path = os.path.join(repo_dir_path, "data", "init_data", "census_tracts.csv")
age_population_path = os.path.join(repo_dir_path, "data", "init_data", "age_population.csv")

output_dir_path = os.path.join(repo_dir_path, 'src', 'visualization', 'Visualization_Covid-19-Vaccine-Simulator', 'preprocessing', 'data')


# data
regions_dict = {}
pop_dict = {}
classes_ratio = []

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


def town_id2name(x):
    return '/'.join(regions_dict[x['Town']])


def town_id2c_name(x):
    return regions_dict[x['Town']][0]


def town_id2t_name(x):
    return regions_dict[x['Town']][1]


def calculate_ratio(x):
    country_pop = pop_dict[x['Country'].replace('台','臺')][x['Age']]
    for c in classes_ratio:
        x[c] = x[c[:-8]]/country_pop

    return x


# For columns name: num_..., ratio_...
def calculate_ratio_state(x):
    country_pop = pop_dict[x['Country'].replace('台','臺')][x['Age']]
    for c in classes_ratio:
        x[c] = x['num'+c[5:]]/country_pop

    return x


def handle_lc_state(file_list):
  
    # Set dir of output
    if not os.path.exists(output_dir_path):
        os.mkdir(output_dir_path)

    data_dir_path = os.path.join(output_dir_path, 'lc_state')
    
    if not os.path.exists(data_dir_path):
        os.mkdir(data_dir_path)

    # Prepare data
    global pop_dict
    pop_dict = get_population_dict_by_country()

    first_df = pd.read_csv(file_list[0])

    columns = list(first_df.columns)
    time_opt = 'E'
    if 'Period' in columns:
        time_opt = 'P'
        columns.remove('Period')
    elif 'Day' in columns:
        time_opt = 'D'
        columns.remove('Day')

    columns.remove('Age')

    town_opt_each = False
    if 'Town' in columns:
        town_opt_each = True
        columns.remove('Town')

    pop_taiwan = get_populationOfTaiwan()

    global classes_ratio

    classes_num = []
    classes_ratio = []

    for c in columns:
        if 'ratio' in c or 'Ratio' in c:
            classes_ratio.append(c)
        else:
            classes_num.append(c)

    cnt = 0

    # Processing  each scenario(csv file)
    for file in file_list:
        cnt += 1
        print('Progress:',cnt,'/',len(file_list), end='\n')
        scen_dir_path = os.path.join(data_dir_path, os.path.splitext(os.path.basename(file))[0])
        if os.path.exists(scen_dir_path):
            print('\tExist!')
            continue
        
        df_file = pd.read_csv(file)
        if time_opt == 'D':
            df_file.rename(columns = {'Day':'Period'}, inplace = True)

        if town_opt_each:
            df_file['Country'] = df_file.apply(town_id2c_name, axis=1)
            df_file['Town'] = df_file.apply(town_id2t_name, axis=1)
        else:
            df_file['Country'] = '全台'
            df_file['Town'] = '全台'
        
        # Generating the df of all age
        print('\tGenerating the df of all age...')
        pd_all_age = df_file.groupby(['Period', 'Country', 'Town'], sort=False, as_index = False).sum()
        pd_all_age['Age'] = 4

        df_file = pd.concat([df_file, pd_all_age])

        if town_opt_each:
            # Generate the data composed of whole Taiwan
            print('\tGenerate the data composed of whole Taiwan...')
            df_allt = df_file.drop(classes_ratio, axis=1).groupby(['Period', 'Age'], sort=False, as_index = False).sum()

            for c in classes_ratio:
                df_allt[c] = df_allt['num'+c[5:]]/pop_taiwan 

            df_allt['Town'] = '全台'
            df_allt['Country'] = '全台'

            # Generate the country data
            print('\tGenerate the country data...')
            print('\t\tGrouping...')
            df_country = df_file.drop(classes_ratio, axis=1).groupby(['Period', 'Country', 'Age'], sort=False, as_index = False).sum()
            print('\t\tCalculating ratio')
            df_country = df_country.apply(calculate_ratio_state, axis=1)
            df_country['Town'] = '全部'
        
            # Combine
            print('\tCombine...')
            df_file = pd.concat([df_file, df_allt, df_country])

        # Outout
        print('\tOutput...')
        scenario_name = os.path.splitext(os.path.basename(file))[0]

        scen_dir_path = os.path.join(data_dir_path, scenario_name)
        os.mkdir(scen_dir_path)
        for age in range(4):
            file_dir_path = os.path.join(scen_dir_path, str(age)+'.csv')
            mask = (df_file['Age'] == age)
            df_file[mask].drop(['Age'], axis=1).to_csv(file_dir_path, index=False)
        file_dir_path = os.path.join(scen_dir_path, 'all.csv')
        mask = df_file['Age'] == 4
        df_file[mask].drop(['Age'], axis=1).to_csv(file_dir_path, index=False)


if __name__ == '__main__':
    file_list = glob.glob(os.path.join(vis_data_dir_path, '*'))
    file_list.sort()
    
    print('List of source data files:')
    for i, f in enumerate(file_list):
        print('\t' + str(i+1) + ':', os.path.basename(f))
    handle_lc_state(file_list)