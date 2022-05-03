import os
import glob
import sys
import pandas as pd
import csv
import shutil
import numpy as np

regions_dict = {}
pop_dict = {}
classes_ratio = []

def generate_region_map_dict():
    regions_dict = {}
    with open('census_tracts.csv', newline='', encoding="utf-8") as csvfile:
        spamreader = csv.reader(csvfile, delimiter=',', quotechar='|')
        for i, row in enumerate(spamreader):
            regions_dict[i] = (str(row[0]).replace('臺', '台'), str(row[1]))
    
    return regions_dict


def get_populationOfTaiwan():
    df_age_population = pd.read_csv(os.path.join('age_population.csv'), header=None)
    return df_age_population[3].sum()


def get_population_dict_by_country():
    df_age_population = pd.read_csv(os.path.join('age_population.csv'), header=None).drop([1], axis=1)
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


def handle_data_choropleth(file_list, verbose):
    if verbose > 0:
        print('Generating models/data_choropleth...')
    data_dir_path = os.path.join('models', 'data_choropleth')

    # mkdir models/data_choropleth
    if os.path.exists(data_dir_path):
        shutil.rmtree(data_dir_path, ignore_errors=True)
    os.mkdir(data_dir_path)

    num_age_group, columns, classes_num, classes_ratio = get_information(pd.read_csv(file_list[0]))

    scenario_num = len(file_list)
    cnt = 0

    # Processing  each scenario(csv file)
    for file in file_list:
        cnt += 1
        df_file = pd.read_csv(file)
        scenario_name = os.path.splitext(os.path.basename(file))[0]
        d_scenario_path = os.path.join(data_dir_path, scenario_name)

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


def handle_data_line_chart_by_state(file_list):
    data_dir_path = os.path.join('models', 'data_line_chart_by_state')

    # mkdir models/data_line_chart_by_state
    if os.path.exists(data_dir_path):
        shutil.rmtree(data_dir_path, ignore_errors=True)
    os.mkdir(data_dir_path)

    global pop_dict
    pop_dict = get_population_dict_by_country()

    first_df = pd.read_csv(file_list[0])

    columns = list(first_df.columns)
    columns.remove('Period')
    columns.remove('Town')
    columns.remove('Age')

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
        
        df_file = pd.read_csv(file)
        df_file['Country'] = df_file.apply(town_id2c_name, axis=1)
        df_file['Town'] = df_file.apply(town_id2t_name, axis=1)
        
        # Generating the df of all age
        print('Generating the df of all age...')
        pd_all_age = df_file.groupby(['Period', 'Country', 'Town'], sort=False, as_index = False).sum()
        pd_all_age['Age'] = 4

        df_file = pd.concat([df_file, pd_all_age])

        # Generate the data composed of whole Taiwan
        print('Generate the data composed of whole Taiwan...')
        df_allt = df_file.drop(classes_ratio, axis=1).groupby(['Period', 'Age'], sort=False, as_index = False).sum()

        for c in classes_ratio:
            df_allt[c] = df_allt['num'+c[5:]]/pop_taiwan 

        df_allt['Town'] = '全台'
        df_allt['Country'] = '全台'

        print('Generate the country data...')
        print('\tGrouping...')
        df_country = df_file.drop(classes_ratio, axis=1).groupby(['Period', 'Country', 'Age'], sort=False, as_index = False).sum()
        print('\tCalculating ratio')
        print(df_country.head())
        df_country = df_country.apply(calculate_ratio_state, axis=1)
        df_country['Town'] = '全部'
        
        print('Combine...')
        df_file = pd.concat([df_file, df_allt, df_country])

        print('Output...')
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



def handle_data_line_chart(file_list, mode=None):
    ''' Generating the csv file for line chart
    '''
    print('Generating models/data_line_chart.csv...')

    # Transform from [Town] id to [Country] and [Town] name, add [scenario] only for town data
    df_list = []
    print('\tTransform from [Town] id and add [scenario] (only for town) ...', end='')
    for file in file_list:
        df_file = pd.read_csv(file)
        df_file['scenario'] = os.path.splitext(os.path.basename(file))[0]

        df_file['Country'] = df_file.apply(town_id2c_name, axis=1)
        df_file['Town'] = df_file.apply(town_id2t_name, axis=1)
        
        df_list.append(df_file)
    print('\r\tTransform from [Town] id and add [scenario] (only for town) done')

    # Combine town data into a dataframe
    pd_all_scenario = pd.concat(df_list)

    columns = list(df_list[0].columns)
    columns.remove('Period')
    columns.remove('Town')
    columns.remove('Age')
    columns.remove('Country')
    columns.remove('scenario')

    global pop_dict
    global classes_ratio

    classes_num = []

    for c in columns:
        if 'ratio' in c or 'Ratio' in c:
            classes_ratio.append(c)
        else:
            classes_num.append(c)
    
    # Generate the data consisting of all age groups.
    pd_all_age = pd_all_scenario.groupby(['Period', 'Country', 'Town', 'scenario'], sort=False, as_index = False).sum()
    pd_all_age['Age'] = 4
    # print(pd_all_age.head())
    pd_all_scenario = pd.concat([pd_all_scenario, pd_all_age])

    # Generate the data composed of whole Taiwan
    df_allt = pd_all_scenario.drop(classes_ratio, axis=1).groupby(['Period', 'scenario', 'Age'], sort=False, as_index = False).sum()
    pop_taiwan = get_populationOfTaiwan()

    for c in classes_ratio:
        if mode:
            df_allt[c] = df_allt['num'+c[5:]]/pop_taiwan 
        else:
            df_allt[c] = df_allt[c[:-8]]/pop_taiwan
    df_allt['Town'] = '全台'
    df_allt['Country'] = '全台'

    pop_dict = get_population_dict_by_country()
    print('______________')

    df_country = pd_all_scenario.drop(classes_ratio, axis=1).groupby(['Period', 'scenario', 'Country', 'Age'], sort=False, as_index = False).sum()

    if mode:
        df_country = df_country.apply(calculate_ratio_state, axis=1)
    else:
        df_country = df_country.apply(calculate_ratio, axis=1)


    df_country['Town'] = '全部'

    pd_all_scenario = pd.concat([df_allt, pd_all_scenario, df_country])
    if mode:
        data_dir_path = os.path.join('models', 'data_line_chart_state')

        # mkdir models/data_line_chart_state
        if os.path.exists(data_dir_path):
            shutil.rmtree(data_dir_path, ignore_errors=True)
        os.mkdir(data_dir_path)

        scenario_list = pd_all_scenario.scenario.unique()

        for scenario in scenario_list:
            scen_dir_path = os.path.join(data_dir_path, scenario)
            os.mkdir(scen_dir_path)
            for age in range(4):
                file_dir_path = os.path.join(scen_dir_path, str(age)+'.csv')
                mask = (pd_all_scenario['scenario'] == scenario) & (pd_all_scenario['Age'] == age)
                pd_all_scenario[mask].to_csv(file_dir_path, index=False)
            file_dir_path = os.path.join(scen_dir_path, 'all.csv')
            mask = (pd_all_scenario['scenario'] == scenario) & (pd_all_scenario['Age'] == 4)
            pd_all_scenario[mask].to_csv(file_dir_path, index=False)
    else:
        pd_all_scenario.to_csv('models/data_line_chart.csv', index=False)



def main():
    # sim_data_dir_path: dir path of raw data (for visualization) 
    if '-p' in sys.argv:
        sim_data_dir_path = sys.argv[2]
    else:
        sim_data_dir_path = os.path.join("models", "sim_data", "*")

    # print('argv2',sim_data_dir_path)
    global regions_dict
    regions_dict = generate_region_map_dict()
    file_list = glob.glob(sim_data_dir_path)

    print('List of sim_data files:')
    for i, f in enumerate(file_list):
        print(str(i+1) + ':', f)
    print()

    verbose = 0
    if '--v1' in sys.argv or '-v1' in sys.argv:
        verbose = 1
    elif '--v2' in sys.argv or '-v2' in sys.argv:
        verbose = 2

    if '--c' in sys.argv:
        handle_data_choropleth(file_list, verbose=verbose)

    if '--l'  in sys.argv and '--state' not in sys.argv:
        handle_data_line_chart(file_list)
    
    if '--l' in sys.argv:
        handle_data_line_chart(file_list, mode='state')


    if '--lc-s' in sys.argv:
        handle_data_line_chart_by_state(file_list)
    

        

if __name__ == '__main__':
    main()