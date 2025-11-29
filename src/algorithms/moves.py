from src.graph import GraphInstance
import random

# funcoes de pertubacoes

def remove_fix(solution, graph:GraphInstance):
    new_solution = solution.copy() 
    while graph.is_solution(new_solution):
        index_sol = [i for i in range(len(solution)) if new_solution[i] == 1]
        node_to_remove = random.choice(index_sol)
        new_solution[node_to_remove] = 0
    out_index = [i for i in range(len(solution)) if new_solution[i] == 0]
    for node in out_index: # talvez adicionar e aceitar somente o que melhora ? ou aceitar piora? 
        new_solution[node] = 1 
        if graph.is_solution(new_solution):
            return new_solution