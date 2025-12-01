from src.graph import GraphInstance

import random
import numpy as np




def Guloso(Instancia:GraphInstance, alpha = 0.8, solution_actual = np.array([])):
    max_it = len(Instancia.reqs)
    if solution_actual.sum() > 0:
        solution = solution_actual
    else:
        solution = np.zeros(shape=max_it) 
    active = Instancia.propagate(solution)
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



def calc_residual_degree(solution, graph:GraphInstance, ascending: bool = True):
    active = graph.propagate(solution)
    active_set = {i for i, v in enumerate(active) if v}
    residual = []
    for node in graph.graph.nodes():
        neighbors = graph.graph.neighbors(node)
        neighbors_not_active = sum(1 for nbr in neighbors if nbr not in active_set)
        residual.append([node, neighbors_not_active])
    return sorted(residual, key=lambda x: x[1], reverse=not ascending)

# trocar recalcular a cada troca
def clean_solution(solution, graph:GraphInstance):
    new_solution = solution.copy()
    while True:
        candidates = calc_residual_degree(new_solution, graph, ascending=True)
        candidates_in_solution = [node for node, _ in candidates if new_solution[node] == 1]
        removed = False
        for node in candidates_in_solution:
            solution_ = new_solution.copy()
            solution_[node] = 0
            if graph.is_solution(solution_):
                new_solution = solution_
                removed = True
                break
        if not removed:
            break
    return new_solution

# refatorar para remover quem e gordura em termo de requisito
def calculate_critial_nodes(solution, graph:GraphInstance):
    active = graph.propagate(solution)
    active_set = {i for i, v in enumerate(active) if v}
    current_input = {v: 0 for v in graph.graph.nodes()} 

    solution_index = [i for i in range(len(solution)) if solution[i] == 1]
    for node in graph.graph.nodes():
        for neighbor in graph.graph.neighbors(node):
            current_input[neighbor] += 1

    candidates_score = []

    for node in solution_index:
        critical_score = 0
        for neighbor in graph.graph.neighbors(node):
            if neighbor in active_set:
                req = graph.reqs[neighbor]
                if current_input[neighbor] == req:
                    critical_score += 1

        candidates_score.append((node, critical_score))
    candidates_score.sort(key=lambda x: x[1])
    return candidates_score


def remove_fix(solution, graph:GraphInstance):
    candidates = calculate_critial_nodes(solution, graph)
    new_solution = solution.copy()

    for node, score in candidates:
        if score == 0:
            new_solution[node] = 0 # remove no que o score e zero
            return new_solution # frist improvement        
        if score > 3:
            break # muitos nos criticos1

        sol_ = new_solution.copy()
        sol_[node] = 0 # solucao esta invalida 

        fixed_sol = Guloso(graph, 1.0, sol_)
        if fixed_sol.sum() <= new_solution.sum():
            print("nova_solucao")
            return fixed_sol








def add_remove(solution, graph:GraphInstance):
    new_solution = solution.copy()
    out_index = [i for i in range(len(solution)) if new_solution[i] == 0]
    node_to_add = random.choice(out_index)  # devo adici new_solution = solution.copy() 
    node_degree_list = calc_residual_degree(new_solution, graph, ascending=True)
    print(node_degree_list)
    node_to_remove = node_degree_list[0][0]
    neighbors = graph.graph.neighbors(node_to_remove)
    print(list(neighbors))
    candidates = [no for no in node_degree_list if no[0] in list(neighbors)]
    print(candidates)
    new_solution[node_to_add] = 1 
    while True: 
        sol_ = new_solution.copy()
        index_sol = [i for i in range(len(solution)) if new_solution[i] == 1]
        if not index_sol:
            break
        node_to_remove = random.choice(index_sol)
        sol_[node_to_remove] = 0
        if graph.is_solution(sol_):
            new_solution = sol_
        else:
            break
    return new_solution

def remove_random(solution, graph:GraphInstance):
    new_solution = solution.copy()
    index_sol = [i for i in range(len(solution)) if new_solution[i] == 1]
    node_to_remove = random.choice(index_sol)
    new_solution[node_to_remove] = 0
    while graph.is_solution(new_solution) == False:
        out_index = [i for i in range(len(solution)) if new_solution[i] == 0]
        node_to_add= random.choice(out_index)
        new_solution[node_to_add] = 1
    return new_solution

def reverseMDG(solution, graph:GraphInstance):
    new_solution = solution.copy()
    i = 0
    while True:
        index_sol = [i for i in range(len(solution)) if new_solution[i] == 1]
        ordered_nodes = sorted(index_sol ,key=lambda x: graph.graph.degree(x))
        solution_ = new_solution.copy()
        solution_[ordered_nodes[0]] = 0
        if graph.is_solution(solution_):
            new_solution = solution_
        else:
            break
    return new_solution




