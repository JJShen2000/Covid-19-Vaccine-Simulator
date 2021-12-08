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
        os.mkdir('../../data/vis_data')

def get_census_tracts():
    with open('./raw_data/age_population_raw.csv') as rcsv:
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
    with open('./raw_data/age_population_raw.csv') as rcsv:
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
    with open('./raw_data/town_population_raw.csv', encoding='big5') as rcsv:
        data = csv.reader(rcsv)
        next(data)
        next(data)

        with open(dump_dir + 'town_population.csv', 'w') as csvf:
            writer = csv.writer(csvf)

            for row in data:
                writer.writerow([row[1], row[3], row[4], row[7], row[8], row[11]])

def get_city_to_city_commute():
    with open('./raw_data/city_to_city_commute_raw.csv', encoding='utf-8') as rcsv:
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
    copyfile('./raw_data/contact_prob.csv', dump_dir + 'contact_prob.csv')


if __name__ == '__main__':
    create_dump_dir()
    get_census_tracts()
    get_age_population()
    get_town_population()
    get_city_to_city_commute()
    get_contact_prob()