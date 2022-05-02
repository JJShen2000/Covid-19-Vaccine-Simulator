import csv
import os
from shutil import copyfile

id_city = {}
id_census_tracts = {}

dump_dir = '../../data/init_data/'

def create_dump_dir():
    if not os.path.isdir('../../data'):
        os.mkdir('../../data')
        os.mkdir('../../data/init_data')
        os.mkdir('../../data/sim_data')
        os.mkdir('../../data/sim_data/graph')
        os.mkdir('../../data/sim_data/conf')
        os.mkdir('../../data/sim_data/init_infectors')
        os.mkdir('../../data/sim_data/vaccine_strat')
        os.mkdir('../../data/vis_data')

def get_census_tracts():
    with open('../../input/census/age_population_raw.csv') as rcsv:
        data = csv.reader(rcsv)
        next(data)

        for row in data:
            if int(row[1]) % 1000 == 0 and row[2] != '總計' and row[2] != '福建省' and row[2] != '臺灣省':
                id_city[row[1]] = row[2]
            if int(row[1]) % 1000 != 0 and row[2] != '總計' and row[2] != '福建省' and row[2] != '臺灣省':
                id_census_tracts[row[1]] = row[2]	

        with open(dump_dir + 'city.csv', 'w') as csvf:
            writer = csv.writer(csvf)

            for id in id_city:
                writer.writerow([id_city[id]])

        with open(dump_dir + 'census_tracts.csv', 'w') as csvf:
            writer = csv.writer(csvf)

            for id in id_census_tracts:
                city = id_city[str(int(id) // 1000 * 1000)]
                writer.writerow([city, id_census_tracts[id]])

def get_age_population():
    with open('../../input/census/age_population_raw.csv') as rcsv:
        data = csv.reader(rcsv)
        next(data)
        
        s = {}
        for row in data:
            if (id_city.get(row[1]) != None) or row[2] == '總計' or row[2] == '福建省' or row[2] == '臺灣省':
                continue
            for col in range(4, 4+101):
                key = id_city[str(int(row[1]) // 1000 * 1000)] + ',' + row[2] + ',' + str(col-4)
                if s.get(key) != None:
                    s[key] = s[key] + int(row[col].replace(',', ''))
                else:
                    s[key] = int(row[col].replace(',', ''))

        with open(dump_dir + 'age_population.csv', 'w') as csvf:
            writer = csv.writer(csvf)

            for k in s:
                city, loc, age = k.split(',')
                writer.writerow([city, loc, age, s[k]])

def get_town_population():
    with open('../../input/census/town_population_raw.csv', encoding='big5') as rcsv:
        data = csv.reader(rcsv)
        next(data)
        next(data)

        with open(dump_dir + 'town_population.csv', 'w') as csvf:
            writer = csv.writer(csvf)

            for row in data:
                writer.writerow([row[1], row[3], row[4], row[7], row[8], row[11]])

def get_city_to_city_commute():
    with open('../../input/census/city_to_city_commute_raw.csv', encoding='utf-8') as rcsv:
        data = csv.reader(rcsv)

        with open(dump_dir + 'city_to_city_commute.csv', 'w') as csvf:
            writer = csv.writer(csvf)

            for row in data:
                for i in range(len(row)):
                    if row[i] == '-':
                        row[i] = '0'
                    row[i] = row[i].replace(' ', '')
                    
                writer.writerow(row)

def get_contact_prob():
    copyfile('../../input/census/contact_prob.csv', dump_dir + 'contact_prob.csv')

def get_configurations():
    for file in os.listdir("../../input/covid_param/"):
        copyfile("../../input/covid_param/" + file, '../../data/sim_data/conf/' + file)

def get_vaccine_strategy():
    for file in os.listdir("../../input/vaccine_strat/"):
        copyfile("../../input/vaccine_strat/" + file, '../../data/sim_data/vaccine_strat/' + file)

if __name__ == '__main__':
    create_dump_dir()
    get_census_tracts()
    get_age_population()
    get_town_population()
    get_city_to_city_commute()
    get_contact_prob()
    get_configurations()
    get_vaccine_strategy()