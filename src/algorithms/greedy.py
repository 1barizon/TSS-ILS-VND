from src.graph import GraphInstance
import random
import numpy as np


def Guloso(Instancia:GraphInstance, alpha = 0.8):
    max_it = len(Instancia.reqs)
    ordered_nodes = sorted(Instancia.graph.nodes, key=lambda x:Instancia.graph.nodes[x]['requisito'], reverse=True)
    solution = np.zeros(shape=max_it) 
    active = 0
    is_solution = False
    while active < max_it or is_solution == False :
        cl = [node for node in ordered_nodes if solution[node] == 0]
        req_limite = int(Instancia.reqs[cl[-1]]+ alpha * (Instancia.reqs[cl[0]] - Instancia.reqs[cl[-1]]))
        rcl = [no for no in cl if Instancia.reqs[no] >= req_limite]
        pick = random.choice(rcl)
        solution[pick] = 1
        active = Instancia.propagate(solution)
        is_solution = Instancia.is_solution(solution)
    print(f"Solucao Gerada com guloso - tamanho: {solution.sum()}")
    return solution
