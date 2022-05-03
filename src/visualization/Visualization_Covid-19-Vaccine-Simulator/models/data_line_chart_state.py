import pandas as pd
import os

line_chart_by_state_data_url = 'https://raw.githubusercontent.com/JJShen2000/Visualization-Data_Covid-19-Vaccine-Simulator/main/data_line_chart_by_state/'

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

# Init the setting of scenario
sc_setting = [4, scenario_conf_keys[0], scenario_init_infectors_keys[0], scenario_vaccine_strategies_keys[0]]

# Init df
url = line_chart_by_state_data_url + '/' + '_'.join([sc_setting[1], sc_setting[2], sc_setting[3], 'state'])
url = url + '/all.csv'
df = pd.read_csv(url)

now_age = 4
classes = list(df.columns)
classes.remove('Period')
classes.remove('Town')
classes.remove('Country')

classes_num = []
classes_ratio = []

for c in classes:
    if 'ratio' in c or 'Ratio' in c:
        classes_ratio.append(c)
    else:
        classes_num.append(c)

tracts = df.Country.unique()

df_census_tracts = pd.read_csv(os.path.join('census_tracts.csv'), header=None) 
tract_option_dict = {k.replace('臺', '台'): list(v) for k, v in df_census_tracts.groupby(0, sort=False)[1]}
for c in tract_option_dict:
    tract_option_dict[c].insert(0,'全部')
tract_option_dict['全台'] = ['全台']

# def update_age(age, scenario):
#     global df
#     global now_age
#     global now_scenario
#     if age != now_age or scenario != now_scenario:
#         if age != 4:
#             df = pd.read_csv(os.path.join(data_dir_path[:-1]+ scenario,str(age)+'.csv'))
#         else:
#             df = pd.read_csv(os.path.join(data_dir_path[:-1]+ scenario,'all.csv'))

#         now_age = age
#         now_scenario = scenario

def update_df_by_age_scenario(age_group, sc_conf, sc_init_inf, sc_vac_strategy):
    global df
    global sc_setting

    if sc_vac_strategy == 'no_vaccine' and sc_conf[-3:] == 'vac':
        sc_conf = sc_conf[:-8]
    
    if [age_group, sc_conf, sc_init_inf, sc_vac_strategy] != sc_setting:
        url1 = line_chart_by_state_data_url + '/' + '_'.join([sc_conf, sc_init_inf, sc_vac_strategy, 'state'])
        age = str(age_group) if age_group < len(age_group_id)-1 else 'all'
        url1 = url1 + '/' + age + '.csv'
        df = pd.read_csv(url1)
        sc_setting = [age_group, sc_conf, sc_init_inf, sc_vac_strategy]

print('Data of line chart-by-states has set.')