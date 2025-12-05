from py_test.graph import GraphInstance
import random
import numpy as np




def Guloso(Instancia:GraphInstance, alpha = 0.8, solution_actual = np.array([])):
    max_it = len(Instancia.reqs)
    if solution_actual.sum() > 0:
        solution = solution_actual
    else:
        solution = np.zeros(shape=max_it) 
    is_solution, active = Instancia.is_solution(solution)
    is_solution = False
    while is_solution == False :
        candidates = calc_residual_degree(solution, Instancia, ascending=False) 
        cl = [no for no in candidates if active[no[0]] == 0]
        degree_limit = int(cl[-1][1] + alpha * (cl[0][1] - cl[-1][1]))
        rcl = [no for no in cl if no[1] >= degree_limit]
        pick = random.choice(rcl)
        solution[pick] = 1
        is_solution, active = Instancia.is_solution(solution)
    #print(f"Solucao Gerada com guloso residual- tamanho: {solution.sum()}")
    return solution

def calc_residual_degree(solution, graph:GraphInstance, ascending: bool = True):
    active = graph.propagate(solution)
    active_set = {i for i, v in enumerate(active) if v}
    residual = []
    for node in graph.graph.nodes():
        neighbors = graph.graph.neighbors(node)
        neighbors_not_active = sum(1 for nbr in neighbors if nbr not in active_set) # soma de vizinhos nao ativos
        residual.append([node, neighbors_not_active])
    return sorted(residual, key=lambda x: x[1], reverse=not ascending)

# trocar recalcular a cada troca
def clean_solution(solution, graph:GraphInstance):
    new_solution = solution.copy()
    removed = True
    while removed:
        candidates = calculate_critial_nodes(new_solution, graph)
        removed = False
        for node, _ in candidates:
            if new_solution[node] == 1:
                solution_ = new_solution.copy()
                solution_[node] = 0
                is_valid, _ = graph.is_solution(solution_)
                if is_valid:
                    new_solution = solution_
                    removed = True
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
            return fixed_sol

# refatorar 
def add_remove(solution, graph:GraphInstance):
    new_solution = solution.copy()
    node_degree_list = calc_residual_degree(new_solution, graph, ascending=False) # grau residual em ordem decrescente
    for node in node_degree_list: # adicionar um no que esta fora e que tem um grande grau residual
        if new_solution[node[0]] == 0:
            new_solution[node[0]] = 1
            break
    
    node_degree_list_new = calc_residual_degree(new_solution, graph, ascending=True)
    for node in node_degree_list_new:
        if new_solution[node[0]] == 1:
            new_solution[node[0]] = 0
            if graph.is_solution(new_solution):
                continue
            else:
                new_solution[node[0]] = 1
    return new_solution

def remove_random(solution, graph:GraphInstance, strength=0.15):
    new_solution = solution.copy()
    sol_list = [i for i in range(len(solution)) if solution[i] == 1] 
    num_to_remove = max(1, int(len(sol_list) * strength))
    nodes_to_remove = random.sample(sol_list, num_to_remove)
    for node in nodes_to_remove:
        new_solution[node] = 0
    new_solution = Guloso(graph, 0.8, new_solution)
    return new_solution

def swap_1_1(solution, graph:GraphInstance):
    new_solution = solution.copy()
    candidates_out = calculate_critial_nodes(new_solution, graph)

