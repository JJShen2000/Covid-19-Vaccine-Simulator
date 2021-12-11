import pandas as pd
import os

class Data_line_chart():
    df = pd.read_csv('models/data_line_chart.csv')
    # df = pd.read_csv('models/toy_df_alls.csv')
    classes = ['num_infection', 'num_dead', 'ratio_infection', 'ratio_dead']
    classes2lable = {'num_infection':'Number of Infected', 'num_dead':'Number of deaths', 'ratio_infection':'Ratio of Infected', 'ratio_dead':'Ratio of deaths'}
    tracts = df.Country.unique()

df_census_tracts = pd.read_csv(os.path.join('census_tracts.csv'), header=None) 
tract_option_dict = {k.replace('臺', '台'): list(v) for k, v in df_census_tracts.groupby(0, sort=False)[1]}
for c in tract_option_dict:
    tract_option_dict[c].insert(0,'全部')
tract_option_dict['全台'] = ['全台']
