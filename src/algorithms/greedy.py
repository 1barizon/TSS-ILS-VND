from src.graph import GraphInstance
import random


def Guloso(Instancia:GraphInstance, alpha = 0.8):
    max_it = len(Instancia.sol)
    ordered_nodes = sorted(Instancia.graph.nodes, key=lambda x:Instancia.graph.nodes[x]['requisito'], reverse=True)
    while Instancia.active.sum() < max_it:
        cl = [node for node in ordered_nodes if Instancia.sol[node] == 0]
        req_limite = int(Instancia.reqs[cl[-1]]+ alpha * (Instancia.reqs[cl[0]] - Instancia.reqs[cl[-1]]))
        rcl = [no for no in cl if Instancia.reqs[no] >= req_limite]
        pick = random.choice(rcl)
        Instancia.step(pick)
        #print(f"No adicionado: {pick} | nos ativos: {Instancia.active.sum()}")

    print(f"Solucao Gerada com guloso - tamanho: {Instancia.sol.sum()}")
