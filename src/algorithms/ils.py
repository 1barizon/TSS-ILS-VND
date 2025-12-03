import sys
import os
sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
from src.graph import GraphInstance
from src.algorithms.functions import Guloso
from src.algorithms.vnd import run_vnd
import numpy as np

class ILS:
    def __init__(self, graph:GraphInstance, max_iter, max_time, shake_function, shake_strength, neighborhood_list, save_stats=False):
        self.graph = graph
        self.max_iter = max_iter
        self.shake = shake_function # algum neighborhood para fazer o shake
        self.neighborhoods = neighborhood_list # configuracoa do vnd para busca local 
        self.best_solution = None
        self.best_of_all = None
        self.best_history = []  # list of (iter, sum)
        self.actual_solution = None
        self.stats = []
        self.iter_limite = 1000  # limite de iteracoes sem melhora
        self.iter_ult = 0 # ultima iteracao com uma melhora
        self.save_stats = save_stats
        self.max_time = max_time
        self.shake_strength = shake_strength

    def run(self):
        current_solution = Guloso(self.graph)
        self.best_solution = current_solution.copy()
        if self.best_of_all is None:
            self.best_of_all = current_solution.copy()
        iter_sem_melhora = 0
        for i in range(self.max_iter):
            solution_shake = self.shake(current_solution, self.graph, self.shake_strength)
            new_solution = run_vnd(self.graph, solution_shake, self.neighborhoods)
            if self.graph.is_solution(new_solution) and new_solution.sum() < self.best_solution.sum():
                print(f"Nova melhor encontrada: {new_solution.sum()}")
                self.best_solution = new_solution.copy()
                iter_sem_melhora = 0 # Resetamos o contador pq achamos algo bom!
                if self.best_solution.sum() < self.best_of_all.sum():
                    self.best_of_all = self.best_solution.copy()
            else:
                iter_sem_melhora += 1
            if self.graph.is_solution(new_solution):
                current_solution = new_solution 
            if iter_sem_melhora > self.iter_limite:
                print(f"Estagnou por {self.iter_limite} iterações. Reiniciando com Guloso...")
                current_solution = Guloso(self.graph) # Reinicia só a corrente
                self.best_solution = current_solution.copy() 
                iter_sem_melhora = 0

        print(f"Melhor de todas encontrada: {self.best_of_all.sum()}")
        return self.best_of_all


        


        
    
