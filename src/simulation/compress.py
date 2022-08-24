# from subprocess import call
import pandas as pd
import numpy as np
import functools
import os
import csv

def get_stack_data(fnames):
    # columns, arrs = None, []
    # for fname in fnames:
    #     with open(fname) as f:
    #         csv_reader = csv.reader(f, delimiter = ',')
    #         cnt = 0
    #         arr = []
    #         for row in csv_reader:
    #             if cnt == 0:
    #                 # print(row)
    #                 columns = row
    #             else:
    #                 arr.append(list(map(int, row)))
    #                 # print(type(row[0]))
    #                 # print(row)
    #                 pass
    #             cnt += 1
    #         print(np.array(arr).shape)
    #         arrs.append(np.array(arr, dtype = np.int32))
    # print(arrs)
    # print(np.stack(arrs, axis = 3).shape)
    # return columns, np.stack(arrs, axis = 3)
    dfs = [pd.read_csv(fname) for fname in fnames]
    arrs = [df.values for df in dfs]    
    arr_stack = np.stack(arrs, axis = 0)
    return dfs[0].columns, arr_stack

def compress(arr3d, call_back, dtype = np.float32):
    _, m, k = arr3d.shape
    com = np.empty((m, k), dtype = dtype)
    for i in range(m):
        for j in range(k):
            com[i, j] = call_back(arr3d[:, i, j])
    return com

def create_compress_file(cols, arrs, fname, call_back, dtype = np.float32):
    com = compress(arrs, call_back, dtype = dtype)
    df = pd.DataFrame(com, columns = cols)   
    df.to_csv(fname, index = False)

def get_percentile(arr, perc):
    return arr[int(np.ceil(len(arr) * perc))]

def main():
    # fnames = [f'res_{x}1.csv' for x in ['', '1', '2', '3', '4', '5']]

    dir = input()
    cnt = os.getcwd()

    os.chdir(dir)
    print(os.getcwd())
    fnames = []
    for filename in os.listdir('./'):
        f = os.path.join('./', filename)
        # checking if it is a file
        print(f)
        if os.path.isfile(f):
            # print(f)
            fnames.append(filename)
    cols, arrs = get_stack_data(fnames)
    os.chdir(cnt)

    create_compress_file(cols, arrs, os.path.join(dir, 'avg.csv'), np.mean)
    create_compress_file(cols, arrs, os.path.join(dir, 'std.csv'), np.std)

    create_compress_file(cols, arrs, os.path.join(dir, 'min.csv'), min, int)
    create_compress_file(cols, arrs, os.path.join(dir, 'max.csv'), max, int)

    create_compress_file(cols, arrs, os.path.join(dir, 'q1.csv'), functools.partial(get_percentile, perc = 0.25), int)
    create_compress_file(cols, arrs, os.path.join(dir, 'q3.csv'), functools.partial(get_percentile, perc = 0.75), int)

if __name__ == '__main__':
    main()
