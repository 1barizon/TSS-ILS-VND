import sys
import os
from scipy.io import mmread
sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
from src.graph import GraphInstance
import networkx as nx
from src.algorithms.greedy import Guloso
from src.algorithms.moves import remove_fix, add_remove

import matplotlib.pyplot as plt
import numpy as np 

file_path = os.path.join(os.path.dirname(__file__), '..', 'data', 'football.gml')
file_path = os.path.normpath(file_path)
file_path = os.path.abspath(file_path)
instancia = GraphInstance(file_path)


sol = Guloso(instancia)
stats = [[], []]
for i in range(100):
    mov1 = remove_fix(sol, instancia)
    mov2 = add_remove(sol, instancia)
    stats[0].append(mov1.sum()-sol.sum())
    stats[1].append(mov2.sum()-sol.sum())



count_neg1 = 0
for i in stats[0]:
    if i < 0:
        count_neg1 += 1


count_neg2 = 0
for i in stats[1]:

    if i < 0:
        count_neg2 += 1

print(count_neg1, count_neg2)
