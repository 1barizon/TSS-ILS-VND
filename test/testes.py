import sys
import os
from scipy.io import mmread
sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
from src.graph import GraphInstance
import networkx as nx
from src.algorithms.ils import ILS
from src.algorithms.functions import remove_fix, add_remove, remove_random, calc_residual_degree, clean_solution, Guloso, calculate_critial_nodes
import matplotlib.pyplot as plt
import numpy as np 
import time

file_path = os.path.join(os.path.dirname(__file__), '..', 'data', 'socfb-nips-ego.edges')
file_path = os.path.normpath(file_path)
file_path = os.path.abspath(file_path)
instancia = GraphInstance(file_path)




#ils = ILS(instancia, 1000,1000, remove_random, 0.2,(clean_solution, remove_fix, add_remove, clean_solution))
#ils.run()
init = time.time()
sol = Guloso(instancia, 0.9)
end = time.time()

print(sol.sum(), end-init)
#print(calculate_critial_nodes(sol, instancia))
#print(instancia.graph.number_of_nodes())

