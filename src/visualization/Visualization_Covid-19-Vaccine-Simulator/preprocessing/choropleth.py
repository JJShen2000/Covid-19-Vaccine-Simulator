import os
import glob
import numpy as np
import pandas as pd
import shutil
from util import *

# output dir path
output_dir_path = os.path.join(repo_dir_path, 'src', 'visualization', 'Visualization_Covid-19-Vaccine-Simulator', 'preprocessing', 'data')

# data
regions_dict = {}
pop_dict = {}
classes_ratio = []

# util
def town_id2name(x):
    return '/'.join(regions_dict[x['Town']])


def town_id2c_name(x):
    return regions_dict[x['Town']][0]


def town_id2t_name(x):
    return regions_dict[x['Town']][1]

def calculate_ratio(x):
    country_pop = pop_dict[x['Country'].replace('台','臺')][int(x['Age'])]
    for c in classes_ratio:
        x[c] = x[c[:-8]]/country_pop

    return x


# For columns name: num_..., ratio_...
def calculate_ratio_state(x):
    country_pop = pop_dict[x['Country'].replace('台','臺')][int(x['Age'])]
    for c in classes_ratio:
        x[c] = x['num'+c[5:]]/country_pop

    return x

def get_information(df):
    num_age_group = df.Age.max()+1

    columns = list(df.columns)
    columns.remove('Period')
    columns.remove('Town')
    columns.remove('Age')

    classes_num = []
    classes_ratio = []

    for c in columns:
        if 'ratio' in c or 'Ratio' in c:
            classes_ratio.append(c)
        else:
            classes_num.append(c)

    # classes = [c.split('_')[1] for c in columns]
    # classes = list(set(classes))

    return num_age_group, columns, classes_num, classes_ratio

def handle_choropleth(file_list, verbose=1):

    # Set dir of output
    if not os.path.exists(output_dir_path):
        os.mkdir(output_dir_path)

    data_dir_path = os.path.join(output_dir_path, 'choropleth')
    
    if not os.path.exists(data_dir_path):
        os.mkdir(data_dir_path)

    first_df = pd.read_csv(file_list[0])

    columns = list(first_df.columns)
    time_opt = 'E'
    if 'Period' in columns:
        time_opt = 'P'
        columns.remove('Period')
    elif 'Day' in columns:
        time_opt = 'D'
        columns.remove('Day')

    global regions_dict
    regions_dict = generate_region_map_dict()

    # num_age_group, columns, classes_num, classes_ratio = get_information(pd.read_csv(file_list[0]))
    num_age_group = first_df.Age.max()+1

    town_opt_each = False
    if 'Town' in columns:
        town_opt_each = True
        columns.remove('Town')
    columns.remove('Age')

    # classes_num = []
    # classes_ratio = []

    # for c in columns:
    #     if 'ratio' in c or 'Ratio' in c:
    #         classes_ratio.append(c)
    #     else:
    #         classes_num.append(c)

    scenario_num = len(file_list)
    cnt = 0

    # Processing  each scenario(csv file)
    for file in file_list:
        cnt += 1
        df_file = pd.read_csv(file)
        scenario_name = os.path.splitext(os.path.basename(file))[0]
        d_scenario_path = os.path.join(data_dir_path, scenario_name)

        if time_opt == 'D':
            df_file.rename(columns = {'Day':'Period'}, inplace = True)

        # mkdir d_scenario_path for each scenario for storing all files of different age groups
        if os.path.exists(d_scenario_path):
            shutil.rmtree(d_scenario_path, ignore_errors=True)
        os.mkdir(d_scenario_path)

        if verbose == 1:
            print('\rProgress:',cnt,'/',scenario_num, end='')

        if verbose > 1:
            print('\tProcessing the '+d_scenario_path)
        
        # First transform Town from id to name
        if verbose > 1:
            print('\t\tTransform df.Town from id to name ...', end='')

        df_file['Town'] = df_file.apply(town_id2name, axis=1)
        if verbose > 1:
            print('\r\t\tTransform df.Town from id to name (done)')
        # print('col:',df_file.columns)

        for age in range(num_age_group):
            df_age = df_file[df_file['Age']==age].copy()
            # Generating log1p columns

            for c in columns:
                df_age['log1p('+c+')'] = np.log1p(df_age[c])
            file_path = os.path.join(d_scenario_path, str(age)+'.csv')
            df_age.to_csv(file_path, index=False)

        # Generating the data for all age groups
        file_path = os.path.join(d_scenario_path, 'all.csv')
        if verbose > 1:
            print('Generating the file for all age groups(' + file_path + ')')

        df_all_age = df_file.groupby(['Period', 'Town'], sort=False, as_index = False).sum()
        for c in columns:
            df_all_age['log1p('+c+')'] = np.log1p(df_all_age[c])
        df_all_age.to_csv(file_path, index=False)


if __name__ == '__main__':
    file_list = glob.glob(os.path.join(vis_data_dir_path, '*'))
    file_list.sort()
    
    print('List of source data files:')
    for i, f in enumerate(file_list):
        print('\t' + str(i+1) + ':', os.path.basename(f))
    handle_choropleth(file_list)