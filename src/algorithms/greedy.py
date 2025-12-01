from src.graph import GraphInstance
import random
import numpy as np
from src.algorithms.moves import calc_residual_degree


def Guloso_residual(Instancia:GraphInstance, alpha = 0.8):
    max_it = len(Instancia.reqs)
    solution = np.zeros(shape=max_it) 
    active = np.zeros(shape=max_it)
    is_solution = False
    while active.sum() < max_it or is_solution == False :
        candidates = calc_residual_degree(solution, Instancia, ascending=False) 
        cl = [no for no in candidates if active[no[0]] == 0]
        degree_limit = int(cl[-1][1] + alpha * (cl[0][1] - cl[-1][1]))
        rcl = [no for no in cl if no[1] >= degree_limit]
        pick = random.choice(rcl)
        solution[pick] = 1
        active = Instancia.propagate(solution)
        is_solution = Instancia.is_solution(solution)
    print(f"Solucao Gerada com guloso residual- tamanho: {solution.sum()}")
    return solution
