"""
Fundamentals :: Algorithms :: Dijkstra's Algorithm
Author: Quinn Mortimer.

This is an implementation of Dijkstra's shortest-path algorithm.
It is inteded as a reference for those looking to brush up on important
algorithms.
"""
"""
The Graph, Node, and Edge included in this file were made to meet the needs
of the algorithm.
"""

from queue import PriorityQueue

class Node:
    """A simple node with an id and list of edges."""
    
    def __init__(self, node_id):
        """Creates a node with a known ID and an empty edge list."""
        self.id = node_id
        self.edges = []
    
    def __str__(self):
        """Provides a readable string with information about this node.
        
        The nodes ID, along with the weights and destination node ID of
        all edges will be placed in the string.
        """
        return 'Node (id: {}, Edges: [{}])'.format(
            self.id, ' '.join([str(edge) for edge in self.edges]))

class Edge:
    """A simple weighted directed edge from one node to another."""
    
    def __init__(self, source, dest, weight):
        self.source = source
        self.dest = dest
        self.weight = weight
    
    def __str__(self):
        return '({}->{} weight={})'.format(
            self.source.id, self.dest.id, self.weight)

class Graph:
    """A minimal graph class containing a mapping of Node IDs to Nodes."""
    
    def __init__(self):
        """Creates a graph with no nodes."""
        self.nodes = {}
    
    def __str__(self):
        """Provides a string with info about a node on each line."""
        return '\n'.join(str(v) for v in self.nodes.values())

def dijkstra(g, startID, endID):
    """An implementation of Dijkstra's shortest-path algorithm.
    
    The version presented here uses a well-known optimization that uses a
    priority queue not present in Dijkstra's original version.
    
    :param g: The graph that that all nodes are in.
    :param startID: The id of the node to start from.
    :param endID: The id of the node to try to get to.
    :return: A tuple, where the first element is the path from start to end,
        and the second element is the distance of this path.
        Both elements will be None if there is no path from start to end.
    """
    
    # A mapping from a node's key to the previous node on the path from start.
    prevs = { key: None for key in g.nodes }
    # A mapping from a node's key to its distance from start.
    dists = { key: None for key in g.nodes }
    
    # Set the distance from the first node to zero.
    dists[startID] = 0
    start = g.nodes[startID]
    
    # Create a priority queue for the nodes.
    nodePQ = PriorityQueue()
    # Add the starting node to it with priority 0.
    nodePQ.put((0, start.id))
    
    # Process nodes from the queue until it is empty
    while not nodePQ.empty():
        # Find the node to process based on the ID at the front of the queue.
        currentNodeID = nodePQ.get()[1]
        currentNode = g.nodes[currentNodeID]
        
        # If the current node is the end node, the path can be completed.
        # We don't have to process the rest of the queue in this case.
        if currentNodeID == endID:
            break
        
        # Iterating over the edges in the current node...
        for edge in currentNode.edges:
            # ...if the edges's destination has not been visited
            if prevs[edge.dest.id] is None:
                # Set the destination's distance from the starting node,
                dist = dists[currentNode.id] + edge.weight
                dists[edge.dest.id] = dist
                # set the destination's previous node to the current node,
                prevs[edge.dest.id] = currentNode
                # put the destination node in the queue with its distance
                # from the starting node as it's priority.
                nodePQ.put((dist, edge.dest.id))
    
    # If the end node is never reached, return none for path and distance.
    if prevs[endID] == None:
        return None, None
    
    # Start constructing the path from the start node to the end node.
    path_back = [endID]
    currentNode = g.nodes[endID]
    # Add each node on the way back to the start node to the path.
    while currentNode.id != startID:
        currentNode = prevs[currentNode.id]
        path_back.append(currentNode.id)
    # Return the path backward reversed and the distance to the end node.
    return path_back[::-1], dists[endID]

if __name__ == '__main__':
    from random import randrange
    
    num_nodes = 10
    edges_per_node = 3
    dist_min = 1
    dist_max = 20
    
    g = Graph()
    g.nodes.update({ i: Node(i) for i in range(0, num_nodes) })
    
    for i in range(num_nodes):
        source = g.nodes[i]
        for j in range(edges_per_node):
            dest = g.nodes[(i + randrange(1, num_nodes)) % num_nodes]
            source.edges.append(
                Edge(source, dest, randrange(dist_min, dist_max + 1)))
    
    print("Graph:", g, sep='\n')
    start = randrange(0, num_nodes)
    end = (start + randrange(1, num_nodes)) % num_nodes
    path, dist = dijkstra(g, start, end)
    print("Start:", start, "End:", end)
    print("Path:", path)
    print("Distance:", dist)
