from src.graph import GraphInstance
import random

# funcoes de pertubacoes


def remove_fix(solution, graph:GraphInstance):
    new_solution = solution.copy() 
    while graph.is_solution(new_solution):
        index_sol = [i for i in range(len(solution)) if new_solution[i] == 1]
        node_to_remove = random.choice(index_sol)
        new_solution[node_to_remove] = 0
    while graph.is_solution(new_solution) == False:
        out_index = [i for i in range(len(solution)) if new_solution[i] == 0]
        node_to_add = random.choice(out_index)  # devo adicionar de forma gulosa ou aleatoria ?
        new_solution[node_to_add] = 1 
    return new_solution


def add_remove(solution, graph:GraphInstance):
    new_solution = solution.copy()
    out_index = [i for i in range(len(solution)) if new_solution[i] == 0]
    node_to_add = random.choice(out_index)  # devo adicionar de forma gulosa ou aleatoria ?
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

    





