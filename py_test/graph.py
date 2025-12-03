import networkx as nx
import os
from scipy.io import mmread
import numpy as np


class GraphInstance:
    def __init__(self, filepath:str):
        print(f"Carregando grafo: {filepath}...")
        self.path = filepath
        print(self.path)
        self.graph = self.load_graph()
        self.reqs = []
        self.calc_req()
        self.adj_mat = nx.adjacency_matrix(self.graph)

    def load_graph(self):
        """
        Carrega grafos de arquivos .txt, .edges, .edge, .csv, .gml ou mtx.
        Detecta automaticamente:
        1. Delimitadores: Espaço, Vírgula (CSV) ou Tabulação.
        2. Comentários: '#' ou '%'.
        3. Cabeçalhos de texto (ex: id_1, id_2) e os remove.
        """
        if not os.path.exists(self.path):
            raise FileNotFoundError(f"Arquivo não encontrado: {self.path}")

        # Se for GML, usa o leitor específico
        if self.path.endswith('.gml'):
            return nx.read_gml(self.path, label='id')
        
        if self.path.endswith(".mtx"):
            sparse_matrix = mmread(self.path)
            G = nx.from_scipy_sparse_array(sparse_matrix)
            g = G.to_undirected()
            G.remove_edges_from(nx.selfloop_edges(G))
            return G


        # Configurações padrão
        delimiter = None 
        comment_char = '#'
        has_header = False # Nova flag para controle
        
        # 1. ESPIAR O ARQUIVO (Detecção de metadados e cabeçalho)
        with open(self.path, 'r') as f:
            for line in f:
                line = line.strip()
                if not line: continue 
                
                # Se for comentário explícito, detectamos o caractere e seguimos
                if line.startswith(('%', '#')):
                    comment_char = line[0]
                    continue
                
            
                # Detecta separador
                if ',' in line: delimiter = ','
                elif '\t' in line: delimiter = '\t'
                else: delimiter = None # Espaço

                # Teste de conversão: Tenta converter o primeiro elemento para int
                parts = line.split(delimiter) if delimiter else line.split()
                try:
                    int(parts[0]) # Se conseguir virar número, é dado!
                    has_header = False
                except ValueError:
                    has_header = True
                
                break 

        print(f"Lendo {self.path} | Delim: '{'espaço' if delimiter is None else delimiter}' | Header: {has_header}")

        # 2. LER O ARQUIVO (Pulando o cabeçalho se necessário)
        try:
            with open(self.path, 'r') as f:
                if has_header:
                    next(f)
                G = nx.read_edgelist(
                    f, 
                    nodetype=int,          
                    comments=comment_char, 
                    delimiter=delimiter,   
                    create_using=nx.Graph(), 
                    data=False             
                )
            
            G.remove_edges_from(nx.selfloop_edges(G))
            G = nx.convert_node_labels_to_integers(G, first_label=0, ordering="sorted")
            return G
            
        except Exception as e:
            raise ValueError(f"Erro crítico ao ler {self.path}: {e}")
        
    def calc_req(self):
        for node, grau in self.graph.degree():
            req = (grau+1)//2
            self.graph.nodes[node]["requisito"] = req
            self.reqs.append(req)

    def propagate(self, solution):
        active = solution.copy().astype(np.float32) 
        changed = True
        while changed:
            neigh_sum = self.adj_mat @ active
            to_active = (neigh_sum >= self.reqs).astype(np.float32)
            new_active = np.maximum(active, to_active)
            changed = bool(np.any(new_active != active))
            active = new_active
        return active

    def propagate_optimized(self, solution):
        # boolean active mask
        active = solution.copy().astype(bool)
        reqs = np.asarray(self.reqs)
        # initial neighbor counts from the initial active set
        neigh_counts = self.adj_mat.dot(active.astype(np.int32)).astype(np.int32)
        # find nodes that should be activated due to current counts
        to_activate = np.flatnonzero((~active) & (neigh_counts >= reqs))

        while to_activate.size > 0:
            # activate nodes and increment neighbor counts incrementally
            for u in to_activate:
                active[u] = True
                for v in self.graph.adj[u]:
                    neigh_counts[v] += 1
            to_activate = np.flatnonzero((~active) & (neigh_counts >= reqs))

        return active.astype(np.float32)

    def is_solution(self, solution):
        active = self.propagate(solution)
        return active.sum() == len(self.reqs) , active