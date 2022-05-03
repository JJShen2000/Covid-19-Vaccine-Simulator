import pandas as pd
import os
import urllib

base_url = 'https://raw.githubusercontent.com/JJShen2000/Visualization-Data_Covid-19-Vaccine-Simulator/main/data_line_chart_by_scenario/'

# Set information about age group for dropdown
age_group_id = range(5)
age_group_label = ['0-4', '5-18', '19-64', '65+', 'All']

# Init the setting of df
df_setting = ['全台', '全台', 4]

df_all_age = pd.read_csv(base_url+urllib.parse.quote(df_setting[0]+'/'+df_setting[1]+'.csv')).drop(['Unnamed: 0'], axis=1)
df = df_all_age[df_all_age['Age'] == 4]

classes = list(df.columns)
classes.remove('Period')
classes.remove('scenario')
classes.remove('Age')

classes_num = []
classes_ratio = []

for c in classes:
    if 'ratio' in c or 'Ratio' in c:
        classes_ratio.append(c)
    else:
        classes_num.append(c)

tracts = list(pd.read_csv('census_tracts.csv', header=None)[0].unique())
tracts = [x.replace('臺', '台') for x in tracts]
tracts.append('全台')

df_census_tracts = pd.read_csv(os.path.join('census_tracts.csv'), header=None) 
tract_option_dict = {k.replace('臺', '台'): list(v) for k, v in df_census_tracts.groupby(0, sort=False)[1]}
for c in tract_option_dict:
    tract_option_dict[c].insert(0,'全部')
tract_option_dict['全台'] = ['全台']

def update_df(country, town, age):
    global df
    global df_setting
    global df_all_age

    if country == df_setting[0] and town == df_setting[1] and age != df_setting[2]:
        df = df_all_age[df_all_age['Age'] == age]
        df_setting[2] = age
    else:
        df_all_age = pd.read_csv(base_url+urllib.parse.quote(country+'/'+town+'.csv')).drop(['Unnamed: 0'], axis=1)
        df = df_all_age[df_all_age['Age'] == age]
        df_setting = [country, town, age]

print('Data of line chart-by-scenarios has set.')