import copy

from settings.settings import Settings
from random import randint
from exception.exception import GraphError


class MyIterator:
    def __init__(self, values):
        self.__values = tuple(values)
        self.__index = 0

    def __iter__(self):
        return self

    def __next__(self):
        if self.__index >= len(self.__values):
            raise StopIteration
        value = self.__values[self.__index]
        self.__index += 1
        return value


class Graph:
    def __init__(self):
        # each node is a key, and we keep a list of inbound edges to that node
        self.__inbound = {}
        # each node is a key, and we keep a list of outbound edges from that node
        self.__outbound = {}
        # each edge is a key, and we keep the cost of that edge as the value
        self.__cost = {}
        # presume that the graph cannot be saved in the normal format
        self.__modified = True

    def __is_vertex(self, vertex: int) -> bool:
        """
        check if a vertex is in the graph
        :param vertex: the vertex's number
        :return: True if vertex is in the graph, False otherwise
        """
        return vertex in self.__inbound

    def __is_edge(self, edge: tuple) -> bool:
        """
        check if an edge is in the graph
        :param edge: a tuple with the start vertex and end vertex
        :return: True if edge is in the graph, False otherwise
        """
        if not self.__is_vertex(edge[0]) or not self.__is_vertex(edge[1]):
            return False
        return edge in self.__cost

    def add_vertex(self, vertex: int) -> None:
        """
        adds a vertex to the graph. If the vertex exists, it does nothing
        :param vertex: the vertex's number
        """
        if not self.__is_vertex(vertex):
            self.__inbound[vertex] = []
            self.__outbound[vertex] = []

    def __add_edge_with_creation(self, edge: tuple, value: int) -> None:
        """
        adds an edge to the graph. If the edge exists, it overwrites the value of the edge. Used for creating the graph,
        because it adds the vertices that don't exist
        :param value: the cost of the edge
        :param edge: a tuple with the start vertex and end vertex
        """
        if self.__is_edge(edge):
            # if we already have this edge overwrite the value and return
            self.__cost[edge] = value
            return

        if not self.__is_vertex(edge[0]):
            # if we don't have the first vertex add it
            self.add_vertex(edge[0])

        if not self.__is_vertex(edge[1]):
            # if we don't have the second vertex add it
            self.add_vertex(edge[1])

        # add the inbound vertex
        self.__inbound[edge[1]].append(edge[0])

        # add the outbound vertex
        self.__outbound[edge[0]].append(edge[1])

        # add the cost
        self.__cost[edge] = value

    def no_vertices(self) -> int:
        """
        :return: the number of vertices in the graph as int
        """
        return len(self.__inbound)

    def no_edges(self) -> int:
        """
        :return: the number of edges in the graph as int
        """
        return len(self.__cost)

    def parse_vertices(self):
        """
        Function for iterating over the vertices in the graph
        :return: an iterator over the vertices in the graph
        """
        return MyIterator(self.__inbound.keys())

    def parse_outbound_edges(self, vertex: int):
        """
        Function for iterating over the outbound edges for a given vertex
        :param vertex: the vertex's number
        :return: an iterator
        """
        if not self.__is_vertex(vertex):
            raise GraphError("The vertex is not in the graph")
        return MyIterator(self.__outbound[vertex])

    def parse_inbound_edges(self, vertex: int):
        """
        Function for iterating over the inbound edges for a given vertex
        :param vertex: the vertex's number
        :return: an iterator
        """
        if not self.__is_vertex(vertex):
            raise GraphError("The vertex is not in the graph")
        return MyIterator(self.__inbound[vertex])

    def exists_edge(self, start: int, end: int) -> bool:
        """
        checks if there is an edge between start and end
        :param start: the start vertex's number
        :param end: the end vertex's number
        :return: True if the edge exists, False otherwise
        """
        if not self.__is_vertex(start) or not self.__is_vertex(end):
            raise GraphError("The start / end points are not in the graph")
        return self.__is_edge((start, end))

    def get_degree_for_vertex(self, vertex: int) -> tuple:
        """
        returns the indegree and outdegree of a vertex
        :param vertex: the vertex's number
        :return: a tuple having on the first position the indegree and on the second position the outdegree
        """
        if not self.__is_vertex(vertex):
            raise GraphError("The vertex is not in the graph")
        return len(self.__inbound[vertex]), len(self.__outbound[vertex])

    def get_cost(self, start: int, end: int) -> int:
        """
        returns the cost of a vertex between start and end
        :param start: the start vertex's number
        :param end: the end vertex's number
        :return: an integer representing the cost
        """
        if not self.__is_edge((start, end)):
            raise GraphError("The edge is not in the graph")
        return self.__cost[(start, end)]

    def set_cost(self, start: int, end: int, value: int):
        """
        set the cost of a vertex between start and end
        :param start: the start vertex's number
        :param end: the end vertex's number
        :param value: the new value for the edge
        """
        if not self.__is_edge((start, end)):
            raise GraphError("The edge is not in the graph")
        self.__cost[(start, end)] = value

    def add_edge(self, start: int, end: int, value: int):
        """
        adds an edge to the graph. If the edge exists, it overwrites the value of the edge
        :param start: the start vertex's number
        :param end: the end vertex's number
        :param value: the cost of the edge
        """
        if not self.__is_vertex(start) or not self.__is_vertex(end):
            raise GraphError("The start / end vertices are not in the graph")

        edge = (start, end)
        self.__add_edge_with_creation(edge, value)

    def remove_edge(self, start: int, end: int):
        """
        removes an edge from the graph
        :param start: the start vertex's number
        :param end: the end vertex's number
        """
        if not self.__is_edge((start, end)):
            raise GraphError("The edge is not in the graph")

        edge = (start, end)
        self.__cost.pop(edge)

        self.__inbound[end].remove(start)
        self.__outbound[start].remove(end)

        # the graph was modified, and we won't be able to store it in the normal format
        self.__modified = True

    def remove_vertex(self, vertex: int):
        """
        removes a vertex from the graph
        :param vertex: the vertex's number
        """
        if not self.__is_vertex(vertex):
            raise GraphError("The vertex is not in the graph")

        # go through all the edges in which vertex is the start vertex
        for v in self.__outbound[vertex]:
            # remove the edge and the cost
            self.__cost.pop((vertex, v))
            # remove the vertex as an inbound for v
            self.__inbound[v].remove(vertex)

        # go through all the edges in which vertex is the end vertex
        for v in self.__inbound[vertex]:
            # remove the edge and the cost
            self.__cost.pop((v, vertex))
            # remove the vertex as an outbound for v
            self.__outbound[v].remove(vertex)

        self.__inbound.pop(vertex)
        self.__outbound.pop(vertex)

        # the graph was modified, and we won't be able to store it in the normal format
        self.__modified = True

    def create_random(self, no_vertices: int, no_edges: int):
        """
        creates a random graph. Overwrites the old graph
        :param no_vertices: the number of vertices in the new graph
        :param no_edges: the number of edges in the new graph
        """
        # delete the old graph
        self.__inbound = {}
        self.__outbound = {}
        self.__cost = {}
        self.__modified = False

        # add all the vertices
        for i in range(no_vertices):
            self.add_vertex(i)

        for i in range(no_edges):
            while True:
                # repeat this until we have a valid edge
                start = randint(0, no_vertices - 1)
                end = randint(0, no_vertices - 1)
                cost = randint(-5, 255)

                if not self.__is_edge((start, end)):
                    self.__add_edge_with_creation((start, end), cost)
                    break

    def load_file(self):
        """
        opens the file given in the settings.properties and reads the content of the file and stores the graph
        """
        s = Settings()

        with open(s.get_input(), "r") as f:
            for line in f:
                # read each line
                parts = line.split()

                if len(parts) == 2:
                    # we are given the no of vertices and the no of edges
                    for i in range(int(parts[0])):
                        self.add_vertex(i)
                    # this means that the graph was given in the normal format, therefore can be saved as such
                    self.__modified = False
                    continue

                elif len(parts) == 3:
                    # we are given an edge
                    edge = (int(parts[0]), int(parts[1]))
                    self.__add_edge_with_creation(edge, int(parts[2]))
                    continue

                else:
                    # we are given the isolated vertices
                    for i in range(len(parts)):
                        if parts[i] != -1:
                            self.add_vertex(int(parts[i]))

    def save_file(self):
        """
        opens the file given in the settings.properties and writes the graph to the file
        """
        s = Settings()

        if not self.__modified:
            # the graph wasn't modified, therefore we can save the graph in the original format (having the no of
            # vertices and edges at the top of the file)
            with open(s.get_output(), "w") as f:
                f.write(f"{self.no_vertices()} {self.no_edges()}\n")
                for edge in self.__cost:
                    f.write(f"{edge[0]} {edge[1]} {self.__cost[edge]}\n")
        else:
            # write the information in the special format with all the edges listed and at the end the isolated vertices
            with open(s.get_output(), "w") as f:
                # write all the edges
                for edge in self.__cost:
                    f.write(f"{edge[0]} {edge[1]} {self.__cost[edge]}\n")

                # create a list with all the isolated nodes
                isolated_nodes = []
                for node in self.__inbound:
                    if self.__inbound[node] == [] and self.__outbound[node] == []:
                        isolated_nodes.append(node)

                # in order to prevent confusion, the program won't allow storing a number of isolated edges that is equal
                # to 2 / 3. We fill the rest of the list with -1 so the program knows that those aren't real nodes, just fillers
                if len(isolated_nodes) == 2:
                    isolated_nodes.append(-1)
                    isolated_nodes.append(-1)

                if len(isolated_nodes) == 3:
                    isolated_nodes.append(-1)

                # write all the isolated nodes
                string = ""
                for node in isolated_nodes:
                    string += f"{node} "
                string = string.strip()
                f.write(string)

    def copy(self):
        """
        returns a deep copy of the graph
        :return: a new graph object independent of the original one
        """
        return copy.deepcopy(self)
