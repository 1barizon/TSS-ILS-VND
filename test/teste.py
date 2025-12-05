import sys
import os
from scipy.io import mmread
sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
from py_test.graph import GraphInstance
import networkx as nx
from py_test.algorithms.ils import ILS
from py_test.algorithms.functions import remove_fix, add_remove, remove_random, calc_residual_degree, clean_solution, Guloso, calculate_critial_nodes
import matplotlib.pyplot as plt
import numpy as np 

file_path = os.path.join(os.path.dirname(__file__), '..', 'data', 'football.gml')
file_path = os.path.normpath(file_path)
file_path = os.path.abspath(file_path)
instancia = GraphInstance(file_path)




#ils = ILS(instancia, 1000,1000, remove_random, 0.1,(clean_solution, remove_fix, add_remove))
#ils.run()

