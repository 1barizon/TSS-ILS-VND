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
        self.sol = np.zeros(shape=(len(self.graph.nodes)), dtype=np.int32)
        self.adj_mat = nx.adjacency_matrix(self.graph)
        self.active= np.zeros(shape=(len(self.graph.nodes)), dtype=np.int32)

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
            return G
            
        except Exception as e:
            raise ValueError(f"Erro crítico ao ler {self.path}: {e}")
        
    def calc_req(self):
        for node, grau in self.graph.degree():
            req = (grau+1)//2
            self.graph.nodes[node]["requisito"] = req
            self.reqs.append(req)

    def step(self, no):
        self.sol[no] = 1
        self.propagate()

    def propagate(self):
        active = self.sol.copy().astype(np.float32) 
        changed = True
        while changed:
            neigh_sum = self.adj_mat @ active
            to_active = (neigh_sum >= self.reqs).astype(np.float32)
            new_active = np.maximum(active, to_active)
            changed = bool(np.any(new_active != active))
            active = new_active
        self.active = active
        

