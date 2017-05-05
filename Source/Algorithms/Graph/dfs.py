"""
Fundamentals :: Algorithms :: Depth-First Search
Author: Quinn Mortimer.

This is a simple recursive Depth-First Search implementation.
It is inteded as a reference for those looking to brush up on important
algorithms.
"""
"""
The Graph, Node, and Edge included in this file were made to meet the needs
of the algorithm.
"""

class Vertex:
    """A minimal vertex with just an ID."""
    
    def __init__(self, vertex_id):
        self.id = vertex_id
        self.edges = []
    
    def __str__(self):
        return 'Vertex (id: {} Edges:[{}])'.format(self.id,
            ' '.join(str(e) for e in self.edges))

class Edge:
    """A simple directed edge."""
    
    def __init__(self, source, dest):
        self.source = source
        self.dest = dest
    
    def __str__(self):
        return 'Edge ({}->{})'.format(
            self.source.id, self.dest.id)

class Graph:
    """A graph class containging a map from vertex ids to vertices."""
    
    def __init__(self):
        """Creates an empty graph."""
        self.vertices = {}
    
    def __str__(self):
        return 'Vertices:\n\t{0}'.format(
            '\n\t'.join(str(v) for v in self.vertices.values()))

def dfs(g, start_id):
    """A recursive Depth-First Search implementation.
    
    This function has two responsibilites, created a set to store the
    visited vertex ids and call the DFS driver function on the starting
    vertex.
    
    :param g: The graph containing the start vertex.
    :param start_id: The id of the vertex to start from.
    """
    visited = set()
    _dfs_visit(g.vertices[start_id], visited)

def _dfs_visit(v, visited):
    """Visits a node, then recursively visits its neighbours.
    
    Once the node has been visited, this function loops through all edges
    going out from the current node. For each edge, if the destination has
    not been visited, then this calls itself on that vertex.
    
    DFS by itself doesn't have any specific side effects, but the algorithm
    can be adapted to take any action when visiting a vertex, before or
    after visiting the vertex's neighbours.
    
    :param v: The vertex to visit.
    :param visited: The set of previously visited vertices.
    """
    visited.add(v.id)
    
    for e in v.edges:
        if e.dest.id not in visited:
            _dfs_visit(e.dest, visited)
