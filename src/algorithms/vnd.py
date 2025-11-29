from src.graph import GraphInstance


def run_vnd(graph :GraphInstance, current_solution,neighborhood_list):
    # neighborhood_list: Uma lista de funcoes [func1, func2, func3]
    best_solution = current_solution
    k = 0

    while k < len(neighborhood_list):
        neighborhood_func = neighborhood_list[k]
        new_solution = neighborhood_func(best_solution, graph) # alterou solucao dentro de graph

        if (graph.is_solution(new_solution)) and new_solution.sum() < best_solution.sum(): 
        # se a solucao e valida e melhor que a anterior
            best_solution = new_solution
            k = 0
        else:
            k+=1

    return best_solution

    