import pandas as pd

choropleth_data_url = 'https://raw.githubusercontent.com/JJShen2000/Visualization-Data_Covid-19-Vaccine-Simulator/main/data_choropleth/'
# sim_data_dir_path = os.path.join("models", "data_choropleth", "*")
# scenDir_list = glob.glob(sim_data_dir_path)
# df_list = []
# scenarioes = []
# dict_df_by_period = {}

# Set information about age group for dropdown
age_group_id = range(5)
age_group_label = ['0-4', '5-18', '19-64', '65+', 'All']

# Combinations of scenario
scenario_conf = {'basic':'原始病毒株(無NPI, 高疫苗施打量)',
                 'basic_low_vac':'原始病毒株(無NPI, 低疫苗施打量)', 
                 'basic_NPI':'原始病毒株(有NPI, 高疫苗施打量)',
                 'basic_NPI_low_vac':'原始病毒株(有NPI, 低疫苗施打量)',
                 'delta':'Delta病毒株(無NPI, 高疫苗施打量)',
                 'delta_low_vac':'Delta病毒株(無NPI, 低疫苗施打量)',
                 'delta_NPI':'Delta病毒株(有NPI, 高疫苗施打量)',
                 'delta_NPI_low_vac':'Delta病毒株(有NPI, 低疫苗施打量)'}
scenario_conf_keys = list(scenario_conf.keys())

scenario_init_infectors = {'wanwua':'萬華爆發', 'xiaogun':'小港爆發'} # Data naming wrong: wanwua
scenario_init_infectors_keys = list(scenario_init_infectors.keys())

scenario_vaccine_strategies = {
    'no_vaccine':'無疫苗',
    'age0_first':'0~4歲優先',
    'age1_first':'5~18歲優先',
    'age2_first':'19~64歲優先',
    'age3_first':'65歲以上優先',
    'no_constraint':'無限制隨機打',
    'Taipei':'台北優先',
    'Kaohsiung':'高雄優先'
} 
scenario_vaccine_strategies_keys = list(scenario_vaccine_strategies.keys())

# Init the setting of 2 scenarios
df_sc1_setting = [4, scenario_conf_keys[0], scenario_init_infectors_keys[0], scenario_vaccine_strategies_keys[0]]
df_sc2_setting = [4, scenario_conf_keys[1], scenario_init_infectors_keys[1], scenario_vaccine_strategies_keys[1]]

# Init df of 2 scenarios
url1 = choropleth_data_url + '/' + '_'.join([df_sc1_setting[1], df_sc1_setting[2], df_sc1_setting[3], 'anal'])
url1 = url1 + '/all.csv'
df_sc1 = pd.read_csv(url1)

url2 = choropleth_data_url + '/' + '_'.join([df_sc2_setting[1], df_sc2_setting[2], df_sc2_setting[3], 'anal'])
url2 = url2 + '/all.csv'
df_sc2 = pd.read_csv(url2)



# def update_df(file_base_name):
#     if file_base_name == len(age_group_id)-1:
#         file_base_name = 'all'
#     global df_list
#     global dict_df_by_period
#     df_list = []
#     for sc in scenDir_list:
#         scenario_name = os.path.splitext(os.path.basename(sc))[0]
#         dict_df_by_period[scenario_name] = []
#         df_file = pd.read_csv(os.path.join(sc, str(file_base_name)+'.csv'))
#         df_list.append(df_file)
#     for i in range(period_num):
#         for j, s in enumerate(scenarioes):
#             dict_df_by_period[s].append(df_list[j][df_list[j]['Period']==i])

# for sc in scenDir_list:
#     scenario_name = os.path.splitext(os.path.basename(sc))[0]
#     scenarioes.append(scenario_name)
#     dict_df_by_period[scenario_name] = []
#     df_file = pd.read_csv(os.path.join(sc, 'all.csv'))
#     df_list.append(df_file)
    

classes = list(df_sc1.columns)
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
range_color_classes['Infections'] = [0, 12]

range_color_classes['Deaths in that period'] = [0, 4]
range_color_classes['Infections in that period'] = [0, 5]

range_color_classes['Deaths (ratio)'] = [0, 0.2]
range_color_classes['Infections (ratio)'] = [0, 0.15]

period_num = df_sc1['Period'].max()+1

df_sc1_by_period = []
df_sc2_by_period = []

for i in range(period_num):
    df_sc1_by_period.append(df_sc1[df_sc1['Period'] == i])
    df_sc2_by_period.append(df_sc2[df_sc2['Period'] == i])

del df_sc1
del df_sc2

# for i in range(period_num):
#     for j, s in enumerate(scenarioes):
#         dict_df_by_period[s].append(df_list[j][df_list[j]['Period']==i])

# print(len(dict_df_by_period['sfeirvd_aon_graph_covid_init_infector_doc_vaccination_doc_age3_first']))
# update_df(2)
def update_df_by_age_scenario(df_id, age_group, sc_conf, sc_init_inf, sc_vac_strategy):
    global df_sc1_by_period
    global df_sc2_by_period
    global df_sc1_setting
    global df_sc2_setting

    if sc_vac_strategy == 'no_vaccine' and sc_conf[-3:] == 'vac':
        sc_conf = sc_conf[:-8]
    
    if df_id == 1 and [age_group, sc_conf, sc_init_inf, sc_vac_strategy] != df_sc1_setting:
        url1 = choropleth_data_url + '/' + '_'.join([sc_conf, sc_init_inf, sc_vac_strategy, 'anal'])
        age = str(age_group) if age_group < len(age_group_id)-1 else 'all'
        url1 = url1 + '/' + age + '.csv'
        df_sc1 = pd.read_csv(url1)
        df_sc1_setting = [age_group, sc_conf, sc_init_inf, sc_vac_strategy]
        df_sc1_by_period = []
        for i in range(period_num):
            df_sc1_by_period.append(df_sc1[df_sc1['Period'] == i])

    elif df_id == 2 and [age_group, sc_conf, sc_init_inf, sc_vac_strategy] != df_sc2_setting:
        url2 = choropleth_data_url + '/' + '_'.join([sc_conf, sc_init_inf, sc_vac_strategy, 'anal'])
        age = str(age_group) if age_group < len(age_group_id)-1 else 'all'
        url2 = url2 + '/' + age + '.csv'
        df_sc2 = pd.read_csv(url2)
        df_sc2_setting = [age_group, sc_conf, sc_init_inf, sc_vac_strategy]
        df_sc2_by_period = []
        for i in range(period_num):
            df_sc2_by_period.append(df_sc2[df_sc2['Period'] == i])

print('Data of choropleth has set.')