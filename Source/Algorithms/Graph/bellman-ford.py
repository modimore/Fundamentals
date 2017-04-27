"""
Fundamentals :: Algorithms :: Dijkstra's Algorithm
Author: Quinn Mortimer.

This is an implementation of The Bellman-Ford shortest-path algorithm. Given a
graph and a node to start from, it finds the distance from that starting node
to all nodes in the graph.
It is inteded as a reference for those looking to brush up on important
algorithms.
"""
"""
The Graph, Vertex, and Edge included in this file were made to meet the needs
of the algorithm.
"""

class Vertex:
    """A minimal vertex with just an ID."""
    
    def __init__(self, vertex_id):
        self.id = vertex_id
    
    def __str__(self):
        return 'Vertex (id: {})'.format(self.id)

class Edge:
    """A simple weighted directed edge."""
    
    def __init__(self, source, dest, weight):
        self.source = source
        self.dest = dest
        self.weight = weight
    
    def __str__(self):
        return 'Edge ({}->{} weight:{})'.format(
            self.source.id, self.dest.id, self.weight)

class Graph:
    """A graph class containging a map of vertices and a list of edges."""
    
    def __init__(self):
        """Creates an empty graph."""
        self.vertices = {}
        self.edges = []
    
    def __str__(self):
        return 'Vertices:\n\t{0}\nEdges:\n\t{1}'.format(
            '\n\t'.join(str(v) for v in self.vertices.values()),
            '\n\t'.join(str(e) for e in self.edges))

class NegativeEdgeCycleError(Exception):
    """An exception to raise when a negative edge-weight cycle is found."""
    
    def __init__(self, a, b):
        """Initializes the exception with two nodes on the cycle path."""
        self.a = a
        self.b = b
    
    @property
    def message(self):
        """Produces a message with information about the cycle found."""
        return "Encountered a negative edge cycle with vertices {} and {}.".format(
            self.b, self.a)

def bellman_ford(g, start_id):
    """An implementation of the Bellman-Ford algorithm.
    
    Given a graph and a node to start from, this algorithms can find the
    distances to all nodes in the graph, and also the previous node on the
    path back to the starting node.
    
    Once for each vertex in the graph, this function loops through all edges
    in the graph. For each edge it sets the distance to the destination vertex
    from start to the minimum of its current distance from start and the
    distance from start along the path that reaches it via the edge's source
    vertex. At each step the distance of each vertex is either correct or an
    overestimate of the actual minimum distance.
    
    Negative edge-weight cycles would cause this algorithm to produce an
    incorrect result. The most that this algorithm can offer is to produce an
    error when a negative edge cycle is found. This implementation can not
    find cycles on paths not visitable from the starting node, because there
    is not a meaningful comparison that can be made between two infinities.
    
    :param g: The graph all vertices and edges are in.
    :param start_id: The id of the starting vertex.
    :return: Two dictionaries, one containing distances from start and one
        containing the neighbour on the path back to start.
    """
    # Initial distances are at positive infinity.
    dists = { key: float('Infinity') for key in g.vertices }
    # Initial previous vertices are all an invalid vertex.
    prevs = { key: None for key in g.vertices }
    
    # Corrects the distance to the starting vertex to 0.
    dists[start_id] = 0
    
    # Relax the edges once for each vertex in the graph except the start.
    for i in range(1, len(g.vertices)):
        for e in g.edges:
            dist_start_dest = dists[e.source.id] + e.weight
            if dists[e.dest.id] > dist_start_dest:
                dists[e.dest.id] = dist_start_dest
                prevs[e.dest.id] = e.source.id
                    
    # Check for negative weight cycles.
    # A cycle can be identified if a vertex is further away from
    # one of its neighbours than the length of the edge connecting them.
    for e in g.edges:
        if dists[e.source.id] + e.weight < dists[e.dest.id]:
            raise NegativeEdgeCycleError(e.source.id, e.dest.id)
    
    return dists, prevs

if __name__ == '__main__':
    from random import randrange
    
    num_nodes = 10
    edges_per_node = 1
    dist_min = -20
    dist_max = 0
    
    g = Graph()
    g.vertices.update({ i: Vertex(i) for i in range(0, num_nodes) })
    
    for i in range(num_nodes):
        
        for j in range(edges_per_node):
            dest = (i + randrange(1, num_nodes)) % num_nodes
            g.edges.append(
                Edge(g.vertices[i], g.vertices[dest],
                    randrange(dist_min, dist_max + 1)))
    
    print("Graph:", g, sep='\n')
    start = randrange(0, num_nodes)
    print("Start:", start)
    try:
        dists, prevs = bellman_ford(g, start)
        print("Distances and Previous Vertices:")
        for d in dists:
            print("Vertex {0} - Previous: {2}, Distance: {1}".format(
            d, dists[d], prevs[d]))
    except NegativeEdgeCycleError as e:
        print("[Error]", e.message)
