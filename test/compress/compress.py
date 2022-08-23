from subprocess import call
import pandas as pd
import numpy as np
import functools

def get_stack_data(fnames):
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
    fnames = [f'res_{x}1.csv' for x in ['', '1', '2', '3', '4', '5']]

    cols, arrs = get_stack_data(fnames)

    create_compress_file(cols, arrs, 'avg.csv', np.mean)
    create_compress_file(cols, arrs, 'std.csv', np.std)

    create_compress_file(cols, arrs, 'perc45.csv', functools.partial(get_percentile, perc = 0.45), int)
    create_compress_file(cols, arrs, 'perc55.csv', functools.partial(get_percentile, perc = 0.55), int)

if __name__ == '__main__':
    main()
    
