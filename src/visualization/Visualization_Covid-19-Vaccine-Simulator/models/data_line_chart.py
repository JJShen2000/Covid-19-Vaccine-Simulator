import pandas as pd
import os

# df_list = []
# for i in range(5):
#     df_all = pd.read_csv('models/data_line_chart.csv')
#     df_list.append(df_all[df_all['Age'] == i])
# df = df_list[4]
# df = pd.read_csv('models/toy_df_alls.csv')
df = pd.read_csv('models/data_line_chart.csv')
df = df[df['Age'] == 4]
now_age = 4
classes = list(df.columns)
classes.remove('Period')
classes.remove('Town')
classes.remove('Country')
classes.remove('scenario')
classes.remove('Age')

classes_num = []
classes_ratio = []

for c in classes:
    if 'ratio' in c or 'Ratio' in c:
        classes_ratio.append(c)
    else:
        classes_num.append(c)

age_group_id = range(5)
age_group_label = ['0-4', '5-18', '19-64', '65+', 'All']

tracts = df.Country.unique()

df_census_tracts = pd.read_csv(os.path.join('census_tracts.csv'), header=None) 
tract_option_dict = {k.replace('臺', '台'): list(v) for k, v in df_census_tracts.groupby(0, sort=False)[1]}
for c in tract_option_dict:
    tract_option_dict[c].insert(0,'全部')
tract_option_dict['全台'] = ['全台']

def update_age(age):
    global df
    global now_age
    if age != now_age:
        df = pd.read_csv('models/data_line_chart.csv')
        df = df[df['Age'] == age]
        now_age = age