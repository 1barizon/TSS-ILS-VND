import sys
import os
sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
from src.graph import GraphInstance
from src.algorithms.greedy import Guloso
from src.algorithms.vnd import run_vnd
import numpy as np

class ILS:
    def __init__(self, graph:GraphInstance, max_iter, shake_function, neighborhood_list, save_stats=False):
        self.graph = graph
        self.max_iter = max_iter
        self.shake = shake_function # algum neighborhood para fazer o shake
        self.neighborhoods = neighborhood_list # configuracoa do vnd para busca local 
        self.best_solution = None
        self.stats = []
        self.iter_limite = 100  # limite de iteracoes sem melhora
        self.iter_ult = 0 # ultima iteracao com uma melhora

    def run(self):
        self.best_solution = Guloso(self.graph)
        for i in range(self.max_iter):
            solution_shake = self.shake(self.best_solution)
            new_solution = run_vnd(self.graph , solution_shake, self.neighborhoods)
            if self.graph.is_solution(new_solution) and new_solution.sum() < self.best_solution.sum():
                self.best_solution = new_solution
                self.iter_ult = i
            elif new_solution.sum() >= self.best_solution.sum() and i - self.iter_ult > self.iter_limite:
                self.best_solution = Guloso(self.graph) # gerar nova solucao


        return self.best_solution 

                
                


        


        
    
