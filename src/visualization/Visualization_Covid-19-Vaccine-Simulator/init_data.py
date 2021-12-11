import os
import glob
import sys
import pandas as pd
import csv


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
    df_age_population = pd.read_csv(os.path.join('age_population.csv'), header=None)
    return df_age_population.drop([2], axis=1).groupby([0], sort=False, as_index = False).sum().set_index(0).to_dict()[3]



def main():
    if '-p' in sys.argv:
        sim_data_dir_path = sys.argv[1]
    else:
        sim_data_dir_path = os.path.join("models", "sim_data", "*")


    regions_dict = generate_region_map_dict()
    file_list = glob.glob(sim_data_dir_path)

    print('List of sim_data files:')
    for i, f in enumerate(file_list):
        print(str(i+1) + ':', f)

    print('Generating models/data_choropleth...')
    data_choropleth_path = os.path.join('models', 'data_choropleth')
    if os.path.exists(data_choropleth_path):
        os.rmdir(data_choropleth_path)
    os.mkdir(data_choropleth_path)

    for file in file_list:
        df_file = pd.read_csv(file)
        file_path = os.path.join(data_choropleth_path, os.path.basename(file))

        for idx in range(df_file.shape[0]):
            df_file.loc[idx, 'Town'] = '/'.join(regions_dict[df_file.loc[idx, 'Town']])
        df_file.to_csv(file_path, index=False)
    

    if '--nl' not in sys.argv:
        print('Generating models/data_line_chart.csv...')
        df_list = []
        for file in file_list:
            df_file = pd.read_csv(file)
            df_file['scenario'] = os.path.splitext(os.path.basename(file))[0]
            for idx in range(df_file.shape[0]):
                town_idx = df_file.loc[idx, 'Town']
                df_file.loc[idx, 'Town'] = regions_dict[df_file.loc[idx, 'Town']][1]
                df_file.loc[idx, 'Country'] = regions_dict[town_idx][0]
            df_list.append(df_file)

        pd_all_scenario = pd.concat(df_list)
        df_allt = pd_all_scenario.groupby(['Period', 'scenario'], sort=False, as_index = False).sum()
        pop_taiwan = get_populationOfTaiwan()
        df_allt['ratio_infection'] = df_allt['num_infection']/pop_taiwan
        df_allt['ratio_dead'] = df_allt['num_dead']/pop_taiwan
        df_allt['Town'] = '全台'
        df_allt['Country'] = '全台'

        pop_dict = get_population_dict_by_country()

        df_country = pd_all_scenario.drop(['ratio_dead', 'ratio_infection'], axis=1).groupby(['Period', 'scenario', 'Country'], sort=False, as_index = False).sum()
        for idx in range(df_country.shape[0]):
            country_pop = pop_dict[df_country.loc[idx, 'Country'].replace('台','臺')]
            df_country.loc[idx, 'ratio_infection'] = df_country.loc[idx, 'num_infection']/country_pop
            df_country.loc[idx, 'ratio_dead'] = df_country.loc[idx, 'num_dead']/country_pop
        df_country['Town'] = '全部'

        pd_all_scenario = pd.concat([df_allt, pd_all_scenario, df_country])
        pd_all_scenario.to_csv('models/data_line_chart.csv', index=False)


if __name__ == '__main__':
    main()