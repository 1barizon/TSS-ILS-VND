from scipy.io import mmread
import os
import numpy as np
import networkx as nx




def load_graph(path):
    """
    Carrega grafos de arquivos .txt, .edges, .edge, .csv, .gml ou mtx.
    Detecta automaticamente:
    1. Delimitadores: Espaço, Vírgula (CSV) ou Tabulação.
    2. Comentários: '#' ou '%'.
    3. Cabeçalhos de texto (ex: id_1, id_2) e os remove.
    """
    if not os.path.exists(path):
        raise FileNotFoundError(f"Arquivo não encontrado: {path}")

    # Se for GML, usa o leitor específico
    if path.endswith('.gml'):
        G = nx.read_gml(path, label='id')
    
    if path.endswith(".mtx"):
        sparse_matrix = mmread(path)
        G = nx.from_scipy_sparse_array(sparse_matrix)
        G = G.to_undirected()
        G.remove_edges_from(nx.selfloop_edges(G))


    # Configurações padrão
    delimiter = None 
    comment_char = '#'
    has_header = False # Nova flag para controle
    
    # 1. ESPIAR O ARQUIVO (Detecção de metadados e cabeçalho)
    with open(path, 'r') as f:
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

    print(f"Lendo {path} | Delim: '{'espaço' if delimiter is None else delimiter}' | Header: {has_header}")

    # 2. LER O ARQUIVO (Pulando o cabeçalho se necessário)
    try:
        with open(path, 'r') as f:
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
    except Exception as e:
        raise ValueError(f"Erro crítico ao ler {path}: {e}")

    # carregado qualquer tipo de grafo    

    out_dir = os.path.abspath(os.path.join(os.path.dirname(path), '..', 'data', 'clean_data'))
    os.makedirs(out_dir, exist_ok=True)
    out_name = os.path.splitext(os.path.basename(path))[0] + '.txt'
    out_path = os.path.join(out_dir, out_name)

    n = G.number_of_nodes()
    m = G.number_of_edges()
    with open(out_path, 'w') as f:
        f.write(f"{n} {m}\n")
        for u, v in sorted(G.edges()):
            f.write(f"{u} {v}\n")

    return out_path






dir_path = os.path.abspath(os.path.join(os.path.dirname(__file__), '..', 'data'))
# Se o diretório não existir, cria para evitar erro ao listar
if not os.path.exists(dir_path):
    print(f"Aviso: diretório de dados não encontrado em {dir_path}. Criando...")
    os.makedirs(dir_path, exist_ok=True)

arquivos_instancias = os.listdir(dir_path)
files = []
instancias = []
for file in arquivos_instancias:
    full_path = os.path.join(dir_path, file)
    # pular diretórios e arquivos ocultos
    if not os.path.isfile(full_path) or file.startswith("."):
        continue
    try:
        out_path = load_graph(full_path)
        files.append(out_path)
        instancias.append(file)
    except Exception as e:
        # não interrompe o processamento de outros arquivos
        print(f"Erro ao processar {full_path}: {e}")
        