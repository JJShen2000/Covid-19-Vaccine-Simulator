import os
import glob
import pandas as pd
import csv
from util import *

# output dir path
output_dir_path = os.path.join(repo_dir_path, 'src', 'visualization', 'Visualization_Covid-19-Vaccine-Simulator', 'preprocessing', 'data')


# data
regions_dict = {}
pop_dict = {}
classes_ratio = []

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


def handle_lc_scenario(file_list):

    # Set dir of output
    if not os.path.exists(output_dir_path):
        os.mkdir(output_dir_path)

    data_dir_path = os.path.join(output_dir_path, 'lc_scenario')
    
    if not os.path.exists(data_dir_path):
        os.mkdir(data_dir_path)

    # Prepare data
    global regions_dict
    regions_dict = generate_region_map_dict()

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

    country_list = []
    if town_opt_each:
        country_list = list(pd.read_csv('census_tracts.csv', header=None)[0].unique())
        country_list = [x.replace('臺', '台') for x in country_list]
    country_list.append('全台')

    for c in country_list:
        country_path = os.path.join(data_dir_path, c)
        if not os.path.exists(country_path):
            os.mkdir(country_path)

    cnt = 0

    for file in file_list:
        cnt += 1
        print('Progress:',cnt,'/',len(file_list), end='\n')
        scenario_name = os.path.splitext(os.path.basename(file))[0].replace('_anal', '')

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
        # key_col = ['Period', 'Country', 'Town']
        # if time_opt == 'D':
        #     key_col = ['Day', 'Country', 'Town']
        # elif time_opt == 'E':
        #     key_col = ['Country', 'Town']
        pd_all_age = df_file.groupby(['Period', 'Country', 'Town'], sort=False, as_index = False).sum()
        pd_all_age['Age'] = 4

        df_file = pd.concat([df_file, pd_all_age])

        if town_opt_each:
            # Generate the data composed of whole Taiwan
            print('\tGenerate the data composed of whole Taiwan...')
            df_allt = df_file.drop(classes_ratio, axis=1).groupby(['Period', 'Age'], sort=False, as_index = False).sum()

            for c in classes_ratio:
                df_allt[c] = df_allt[c[:-8]]/pop_taiwan

            df_allt['Town'] = '全台'
            df_allt['Country'] = '全台'

            # Generate the country data
            print('\tGenerate the country data...')
            print('\t\tGrouping...')
            df_country = df_file.drop(classes_ratio, axis=1).groupby(['Period', 'Country', 'Age'], sort=False, as_index = False).sum()
            print('\t\tCalculating ratio')

            df_country = df_country.apply(calculate_ratio, axis=1)
            df_country['Town'] = '全部'

            # Combine
            print('\tCombine...')
            df_file = pd.concat([df_file, df_allt, df_country])

        # Output
        print('Output...')
        for c in country_list:
            df_country = df_file[df_file["Country"] == c]
            for town in df_country.Town.unique():
                mask = (df_country["Town"] == town)
                local_df = df_country[mask]
                local_df = local_df.drop(['Town', 'Country'], axis=1)
                local_df['scenario'] = scenario_name

                # if age == 4:
                #   output_path = os.path.join(data_dir_path, c, 'all.csv')
                # else:
                #   output_path = os.path.join(data_dir_path, c, str(age)+'.csv')
                output_path = os.path.join(data_dir_path, c, town+'.csv')


                if file == file_list[0]:
                    local_df.to_csv(output_path, index=False)
                else:
                    output_df = pd.read_csv(output_path)
                    output_df = pd.concat([output_df, local_df])
                    output_df.to_csv(output_path, index=False)


if __name__ == '__main__':
    file_list = glob.glob(os.path.join(vis_data_dir_path, '*'))
    file_list.sort()
    
    print('List of source data files:')
    for i, f in enumerate(file_list):
        print('\t' + str(i+1) + ':', os.path.basename(f))
    handle_lc_scenario(file_list)

