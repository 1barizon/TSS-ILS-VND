from src.graph import GraphInstance
import random


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
def remove_fix(solution, graph:GraphInstance):
    pass

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




