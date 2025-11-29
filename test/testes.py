import sys
import os
from scipy.io import mmread
sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
from src.graph import GraphInstance
import networkx as nx
from src.algorithms.greedy import Guloso



file_path = os.path.join(os.path.dirname(__file__), '..', 'data', 'football.gml')
file_path = os.path.normpath(file_path)
file_path = os.path.abspath(file_path)
instancia = GraphInstance(file_path)


Guloso(instancia)


