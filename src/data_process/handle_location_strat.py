import os
import glob
import pandas as pd

repo_dir_path = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', '..')
# repo_dir_path = os.path.join('..', '..')

# vac_stract_path = os.path.join(repo_dir_path, 'input', 'vaccine_strat')
vac_stract_path = os.path.join(repo_dir_path, 'data', 'sim_data', 'vaccine_strat')
city_path = os.path.join(repo_dir_path, 'data', 'init_data', 'city.csv')
tract_path = os.path.join(repo_dir_path, 'data', 'init_data', 'census_tracts.csv')

file_list = glob.glob(os.path.join(vac_stract_path, '*'))

# city name => a list of ids of tracts
city_to_tract_dict = {}

def generate_city_to_tract_dict():
    city_df = pd.read_csv(city_path, header=None)

    # city name => city id
    city_dict = {v: k for k, v in city_df.to_dict()[0].items()}

    # for i in range(len(city_dict)):
    #     city_to_tract_dict[i] = []

    tract_df = pd.read_csv(tract_path, header=None)

    # tract id => city name
    tract_dict = tract_df.to_dict()[0]
    for tract_id in tract_dict:
        # city_id = city_dict[tract_dict[tract_id]]
        if tract_dict[tract_id] not in city_to_tract_dict:
            city_to_tract_dict[tract_dict[tract_id]] = [tract_id]
        else:
            city_to_tract_dict[tract_dict[tract_id]].append(tract_id)


def get_tracts_list(city_list):
    tracts_list = []
    for c in city_list:
        for t in city_to_tract_dict[c]:
            tracts_list.append(t)
    return tracts_list
    

if __name__ == '__main__':
    generate_city_to_tract_dict()

    for file_name in file_list:

        with open(file_name) as f:
            lines = f.readlines()
            if (lines[0] == 'location_first\n'):
                city_list = lines[1].split()[1:]
                location_priority = 'location_priority ' + str(get_tracts_list(city_list))[1:-1].replace(',', '') + '\n'
                lines[1] =location_priority

                with open(file_name, 'w+') as f:
                    f.write(''.join(lines))