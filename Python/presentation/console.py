from domain.graph import Graph
from exception.exception import GraphError
import time


class Console:
    """
    The class that links the Graph class to the user. The user does not manipulate the graph directly, but rather
    through the Console class, which takes the input / prints the output and calls the respective methods.
    Each method asks for its specific input and checks whether the input is valid. If the input is not valid an
    error is raised and it's printed to the console.
    """

    def __init__(self):
        self.__graph = Graph()
        self.__commands = {
            1: self.__ui_number_vertices,
            2: self.__ui_list_vertices,
            3: self.__ui_is_edge_between_vertices,
            4: self.__ui_get_degree,
            5: self.__ui_list_outbound_edges,
            6: self.__ui_list_inbound_edges,
            7: self.__ui_get_cost_for_edge,
            8: self.__ui_update_cost_for_edge,
            9: self.__ui_add_edge,
            10: self.__ui_remove_edge,
            11: self.__ui_add_vertex,
            12: self.__ui_remove_vertex,
            13: self.__ui_load_from_file,
            14: self.__ui_save_to_file,
            15: self.__ui_create_random_graph,
            16: exit,
        }

    @staticmethod
    def __print_menu():
        """
        prints the menu to the console
        """
        print("==============================================")
        print("1. Get the number of vertices")
        print("2. Parse the set of vertices")
        print("3. Find if there is an edge between 2 vertices")
        print("4. Get the degree for a vertex")
        print("5. Parse the outbound edges of a vertex")
        print("6. Parse the inbound edges of a vertex")
        print("7. Get cost for edge")
        print("8. Update cost for edge")
        print("9. Add edge")
        print("10. Remove edge")
        print("11. Add vertex")
        print("12. Remove vertex")
        print("13. Load from file")
        print("14. Save to file")
        print("15. Create a random graph")
        print("16. Exit")

    def __ui_number_vertices(self):
        print(
            f"The total number of vertices in the graph is {self.__graph.no_vertices()}"
        )

    def __ui_list_vertices(self):
        print("The vertices in the graph are: ")
        for vertex in self.__graph.parse_vertices():
            print(vertex)

    def __ui_is_edge_between_vertices(self):
        start = int(input("Enter the start vertex: "))
        end = int(input("Enter the end vertex: "))
        result = self.__graph.exists_edge(start, end)
        if result:
            print("The edge between the vertices exists")
        else:
            print("There is no edge between the vertices")

    def __ui_get_degree(self):
        vertex = int(input("Enter vertex : "))
        degree = self.__graph.get_degree_for_vertex(vertex)
        print(
            f"The in-degree for vertex {vertex} is {degree[0]} and the out-degree is {degree[1]}"
        )

    def __ui_list_outbound_edges(self):
        vertex = int(input("Enter vertex: "))
        vertices = []
        for v in self.__graph.parse_outbound_edges(vertex):
            vertices.append(v)
        print(f"The outbound edges of vertex {vertex} are: {vertices}")

    def __ui_list_inbound_edges(self):
        vertex = int(input("Enter vertex: "))
        vertices = []
        for v in self.__graph.parse_inbound_edges(vertex):
            vertices.append(v)
        print(f"The inbound edges of vertex {vertex} are: {vertices}")

    def __ui_get_cost_for_edge(self):
        start = int(input("Enter the start vertex: "))
        end = int(input("Enter the end vertex: "))
        print(
            f"The cost for the ({start}, {end}) edge is {self.__graph.get_cost(start, end)}"
        )

    def __ui_update_cost_for_edge(self):
        start = int(input("Enter the start vertex: "))
        end = int(input("Enter the end vertex: "))
        cost = int(input("Enter the new edge cost: "))
        self.__graph.set_cost(start, end, cost)

    def __ui_add_edge(self):
        start = int(input("Enter start vertex: "))
        end = int(input("Enter end vertex: "))
        cost = int(input("Enter edge cost: "))
        self.__graph.add_edge(start, end, cost)

    def __ui_remove_edge(self):
        start = int(input("Enter the start vertex: "))
        end = int(input("Enter the end vertex: "))
        self.__graph.remove_edge(start, end)

    def __ui_add_vertex(self):
        vertex = int(input("Enter vertex: "))
        self.__graph.add_vertex(vertex)

    def __ui_remove_vertex(self):
        vertex = int(input("Enter vertex number: "))
        self.__graph.remove_vertex(vertex)

    def __ui_load_from_file(self):
        start_time = time.time()
        self.__graph.load_file()
        end_time = time.time()
        execution_time = end_time - start_time

        print(f"Execution Time: {execution_time:.6f} seconds")

        print("Graph loaded from file")

    def __ui_save_to_file(self):
        self.__graph.save_file()
        print("Graph saved to file")

    def __ui_create_random_graph(self):
        no_vertices = int(input("Enter the number of vertices: "))
        no_edges = int(input("Enter the number of edges: "))

        self.__graph.create_random(no_vertices, no_edges)

        print("Graph created successfully")

    def run(self):
        """
        The main function of the program. It loops infinitely with the following operations: asks for the command and
        performs the requested operation.
        In case of errors being raised by the program, the function catches them and prints them to the user
        """
        while True:
            self.__print_menu()
            choice = input("> ")

            # check if the choice is an int
            try:
                choice = int(choice)
            except ValueError:
                print("Invalid choice")
                continue

            if choice in self.__commands:
                try:
                    self.__commands[choice]()
                except ValueError as ve:
                    print(
                        "Invalid numerical value. Make sure you've entered an integer"
                    )
                except GraphError as ge:
                    print("Graph error: {}".format(ge))
            else:
                print("Invalid choice")
                continue

