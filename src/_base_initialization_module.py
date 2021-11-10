class _base_initialization_module:
    def __init__(self, fname):
        self.data = load_data()
        
        pass
        
    def load_data(self, fname):
        '''
        '''
        pass
    
    def build_graph(self):
        ''' Build the graph.
        Return: the graph
        '''
        G = nx.MultiDiGraph()
        _assign_attr_in_each_tract(G)
        _assign_worker_edge_between_tract(G)
        _assign_initial_infectors(G)
        return G
    
    def _assign_attr_in_each_tract(self, G):
        '''
        - 建人數的node
        - 根據年齡分布給age
        - 根據contact group給group attribute
        - 根據contact matrix給contact group內的node建邊
        '''
        pass
    
    def _assign_worker_edge_between_tract(self, G):
        '''
        - % of worker with diff day and night?
        - distribution to assign location of day and night?
        '''
        pass
    
    def _assign_initial_infectors(self, G):
        pass
