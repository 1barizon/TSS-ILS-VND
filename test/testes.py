import sys
import os
from scipy.io import mmread
sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
from src.graph import GraphInstance
import networkx as nx
from src.algorithms.greedy import Guloso
from src.algorithms.moves import remove_fix, add_remove, remove_random, reverseMDG
from src.algorithms.ils import ILS
import matplotlib.pyplot as plt
import numpy as np 

file_path = os.path.join(os.path.dirname(__file__), '..', 'data', 'socfb-nips-ego.edges')
file_path = os.path.normpath(file_path)
file_path = os.path.abspath(file_path)
instancia = GraphInstance(file_path)

sol = Guloso(instancia)


ils = ILS(instancia, 1000, remove_random, [remove_fix, add_remove,reverseMDG])
print(ils.run().sum())
