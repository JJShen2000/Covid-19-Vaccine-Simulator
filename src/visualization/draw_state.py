import matplotlib.pyplot as plt
import sys
import numpy as np
import os
from pandas import *
import csv

vis_dir = '../../data/vis_data/'
state_map = {'E': 'New_Added_Exposed', 'D': 'Dead_Number'}

def draw(dn, n, states):
#    plt.figure(n,figsize=(19,10))
    plt.figure(n)
    plt.xlabel("Day")
    plt.ylabel("Number")
    title = dn.replace(vis_dir, '').replace('/', '').replace('.', '')
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
                yd[h] = np.array([0 for i in range(360)])
        exposed += np.array(data["Exposed_Number"]).sum()
        filenum += 1
        for h in hd:
            yd[h] += np.array(data[h])
    
    for h in hd:
        yd[h] = yd[h] / filenum
    
    xd = np.array([i for i in range(1, 361)])
    for s in states:
        if s in state_map.keys():
            h = state_map[s]
            plt.plot(xd, yd[h], label=h, linewidth=lw)

    print(title+":", yd["New_Added_Exposed"].sum() / filenum, yd["Dead_Number"].max())

    legend = []
    lg = plt.legend(loc=0, prop={'size': 12})
    for l in lg.get_lines():
        l.set_linewidth(lw)

#    plt.savefig(title+".jpg")

if len(sys.argv) > 1 and sys.argv[1] != '-d':
    draw_states = sys.argv[1]
else:
    draw_states = "ED"

np.set_printoptions(suppress=True)
if os.path.isdir(vis_dir):
    cnt = 1
    for d in os.listdir(vis_dir):
        pd = vis_dir+d+'/'
        draw(pd, cnt, draw_states)
        cnt += 1

plt.show()
