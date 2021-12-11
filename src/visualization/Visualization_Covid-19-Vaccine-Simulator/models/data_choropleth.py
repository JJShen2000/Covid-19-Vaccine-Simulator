import pandas as pd
import os
import glob

sim_data_dir_path = os.path.join("models", "data_choropleth", "*")
file_list = glob.glob(sim_data_dir_path)

df_list = []
scenarioes = []
dict_df_by_period = {}

for file in file_list:
    scenario_name = os.path.splitext(os.path.basename(file))[0]
    scenarioes.append(scenario_name)
    dict_df_by_period[scenario_name] = []
    df_file = pd.read_csv(file)
    df_list.append(df_file)


classes = ['num_infection', 'num_dead', 'ratio_infection', 'ratio_dead']
classes2lable = {'num_infection':'Number of Infected', 'num_dead':'Number of deaths', 'ratio_infection':'Ratio of Infected', 'ratio_dead':'Ratio of deaths'}
range_color_classes = {'num_infection':[0, 10000], 'num_dead':[0, 100], 'ratio_infection':[0,1], 'ratio_dead':[0,1]}

period_num = df_list[0]['Period'].max()+1

for i in range(period_num):
    for j, s in enumerate(scenarioes):
        dict_df_by_period[s].append(df_list[j][df_list[j]['Period']==i+1])
