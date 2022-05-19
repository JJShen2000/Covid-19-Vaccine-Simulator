import matplotlib.pyplot as plt
import numpy as np
import os
from pandas import *
import csv

vis_dir = '../../data/vis_data/'

def draw(dn, n):
    plt.figure(n)
    plt.xlabel("Day")
    plt.ylabel("Number")
    title = dn.replace(vis_dir, '').replace('/', '').replace('.', '').replace('_', ' ')
    plt.title(title)

    lw = 3.5
    hd = []
    yd = {}
    b = 0
    
    filenum = 0
    exposed = 0

    for file in os.listdir(dn):
        isum = 0
        data = read_csv(dn + file)
        data.columns = [c.strip().replace(' ', '_') for c in data.columns]
        hd = data.columns.to_list()
        if b == 0:
            b = 1
            for h in hd:
                yd[h] = np.array([0 for i in range(180)])
        exposed += np.array(data["Exposed_Number"]).sum()
        #if np.array(data["Exposed_Number"]).sum() < 100000:
        #    continue
        filenum += 1
        for h in hd:
            yd[h] += np.array(data[h])

    xd = np.array([i for i in range(1, 181)])
    for h in hd:
        #if h != "New_Added_Exposed":
        if h != "Dead_Number":
            continue
        yd[h] = yd[h] / filenum
        plt.plot(xd, yd[h], label=h, linewidth=lw)
    
    print(title+":", yd["New_Added_Exposed"].sum(), yd["Dead_Number"].max())

    legend = []
    lg = plt.legend(loc=0, prop={'size': 12})
    for l in lg.get_lines():
        l.set_linewidth(lw)

np.set_printoptions(suppress=True)
if os.path.isdir(vis_dir):
    cnt = 1
    for d in os.listdir(vis_dir):
        pd = vis_dir+d+'/'
        draw(pd, cnt)
        cnt += 1

plt.show()