import pandas as pd
import os
import glob

sim_data_dir_path = os.path.join("models", "data_choropleth", "*")
scenDir_list = glob.glob(sim_data_dir_path)
df_list = []
scenarioes = []
dict_df_by_period = {}
age_group_id = range(5)
age_group_label = ['0-4', '5-18', '19-64', '65+', 'All']

def update_df(file_base_name):
    if file_base_name == len(age_group_id)-1:
        file_base_name = 'all'
    global df_list
    global dict_df_by_period
    df_list = []
    for sc in scenDir_list:
        scenario_name = os.path.splitext(os.path.basename(sc))[0]
        dict_df_by_period[scenario_name] = []
        df_file = pd.read_csv(os.path.join(sc, str(file_base_name)+'.csv'))
        df_list.append(df_file)
    for i in range(period_num):
        for j, s in enumerate(scenarioes):
            dict_df_by_period[s].append(df_list[j][df_list[j]['Period']==i])

for sc in scenDir_list:
    scenario_name = os.path.splitext(os.path.basename(sc))[0]
    scenarioes.append(scenario_name)
    dict_df_by_period[scenario_name] = []
    df_file = pd.read_csv(os.path.join(sc, 'all.csv'))
    df_list.append(df_file)
    

classes = list(df_list[0].columns)
classes.remove('Period')
classes.remove('Town')
classes.remove('Age')

classes_num = []
classes_ratio = []

for c in classes:
    if c[:3]!='log':
        if 'ratio' in c or 'Ratio' in c:
            classes_ratio.append(c)
        else:
            classes_num.append(c)


# classes = ['num_infection', 'num_dead', 'ratio_infection', 'ratio_dead']
range_color_classes = {}
for c in classes:
    range_color_classes[c] = [0, 15] if 'ratio' in c or 'Ratio' in c else [0, 1]

# classes2lable = {'num_infection':'Number of Infected', 'num_dead':'Number of deaths', 'ratio_infection':'Ratio of Infected', 'ratio_dead':'Ratio of deaths'}
range_color_classes['Deaths'] = [0, 8]
range_color_classes['Infections'] = [0, 11]

range_color_classes['Deaths in that period'] = [0, 4]
range_color_classes['Infections in that period'] = [0, 5]

range_color_classes['Deaths (ratio)'] = [0, 0.2]
range_color_classes['Infections (ratio)'] = [0, 0.15]

period_num = df_list[0]['Period'].max()+1

for i in range(period_num):
    for j, s in enumerate(scenarioes):
        dict_df_by_period[s].append(df_list[j][df_list[j]['Period']==i])

# print(len(dict_df_by_period['sfeirvd_aon_graph_covid_init_infector_doc_vaccination_doc_age3_first']))
# update_df(2)