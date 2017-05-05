"""
Fundamentals :: Algorithms :: Breadth-First Search
Author: Quinn Mortimer.

This is a simple iterative Breadth-First Search implementation.
It is inteded as a reference for those looking to brush up on important
algorithms.
"""
"""
The Graph, Node, and Edge included in this file were made to meet the needs
of the algorithm.
"""

from queue import Queue

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

def bfs(g, start_id):
    """An iterative Breadth-First Search implementation.
    
    This implementation maintains a queue of vertices to visit
    and a set of vertices which have already been added to the
    visit queue.
    
    BFS by itself doesn't have any side effects, but the algorithm
    can be adapted to do something while visiting a vertex.
    
    :param g: The graph containing the start vertex.
    :param start_id: The id of the vertex to start from.
    """
    visit_queue = Queue()
    vertices_seen = set()
    
    visit_queue.put(g.vertices[start_id])
    vertices_seen.add(start_id)
    
    while not visit_queue.empty():
        v = visit_queue.get()
        
        for e in v.edges:
            if e.dest.id not in vertices_seen:
                visit_queue.put(e.dest)
                vertices_seen.add(e.dest.id)
