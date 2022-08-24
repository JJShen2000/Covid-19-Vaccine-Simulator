# from subprocess import call
import pandas as pd
import numpy as np
import functools
import os

def get_stack_data(fnames):
    for fname in fnames:
        print(fname)
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
    fnames = []
    for filename in os.listdir(dir):
        f = os.path.join(dir, filename)
        # checking if it is a file
        if os.path.isfile(f):
            # print(f)
            fnames.append(f)

    cols, arrs = get_stack_data(fnames)

    create_compress_file(cols, arrs, os.path.join(dir, 'avg.csv'), np.mean)
    create_compress_file(cols, arrs, os.path.join(dir, 'std.csv'), np.std)

    create_compress_file(cols, arrs, os.path.join(dir, 'min.csv'), min, int)
    create_compress_file(cols, arrs, os.path.join(dir, 'max.csv'), max, int)

    create_compress_file(cols, arrs, os.path.join(dir, 'q1.csv'), functools.partial(get_percentile, perc = 0.25), int)
    create_compress_file(cols, arrs, os.path.join(dir, 'q3.csv'), functools.partial(get_percentile, perc = 0.75), int)

if __name__ == '__main__':
    main()
