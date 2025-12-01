import sys
import os
from scipy.io import mmread
sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
from src.graph import GraphInstance
import networkx as nx
from src.algorithms.greedy import Guloso_residual
from src.algorithms.moves import remove_fix, add_remove, remove_random, reverseMDG, calc_residual_degree, clean_solution
import matplotlib.pyplot as plt
import numpy as np 

file_path = os.path.join(os.path.dirname(__file__), '..', 'data', 'football.gml')
file_path = os.path.normpath(file_path)
file_path = os.path.abspath(file_path)
instancia = GraphInstance(file_path)
print(len(instancia.graph.nodes()))
sol = Guloso_residual(instancia)
sol = clean_solution(sol, instancia)
print(sol.sum())

