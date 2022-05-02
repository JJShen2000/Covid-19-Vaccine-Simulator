import random
import pandas as pd
import numpy
import os
pd.options.mode.chained_assignment = None

census_tracts_path = '../../data/init_data/census_tracts.csv'
for file in os.listdir("../../input/init_infectors/"):
    tract_init_infectors_path = '../../input/init_infectors/' + file
    output_tract_init_infectors_path = '../../data/sim_data/init_infectors/' + file

    df_tract_infectors = pd.read_csv(tract_init_infectors_path, header=None)

    df_tract_infectors = pd.concat([pd.DataFrame([[len(df_tract_infectors), 0]]), df_tract_infectors])

    df_tracts = pd.read_csv(census_tracts_path, header=None)
    tract_dict = dict()
    for i in range(len(df_tracts)):
        tract_name = df_tracts.iloc[i, 0] + df_tracts.iloc[i, 1]
        tract_dict[tract_name] = i

    def tract_str2id(x):
        return tract_dict[x]

    df_tract_infectors[0][1:] = df_tract_infectors[0][1:].apply(tract_str2id)
    tmp = df_tract_infectors.to_numpy()

    with open(output_tract_init_infectors_path, 'w') as f:
        f.write(str(tmp[0][0]) + '\n')
        for i in range(1, len(tmp)):
            f.write(str(tmp[i][0]) + " " + str(tmp[i][1]) + '\n')
#    df_tract_infectors.to_csv(output_tract_init_infectors_path, index = False, header=None)

