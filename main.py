import time
import sys
from collections import defaultdict


class LinkedList:
    def __init__(self, value):
        self.value = value
        self.next = None

def read_data(filename):
    inbound = defaultdict(lambda: None)
    outbound = defaultdict(lambda: None)
    costs = {}

    try:
        with open(filename, "r") as file:
            vertices, edges = map(int, file.readline().split())
            print("File opened")
            print("Reading from file...")

            for _ in range(edges):
                parent, child, cost = map(int, file.readline().split())
                costs[(parent, child)] = cost

                # Add to outbound adjacency list
                outbound_node = LinkedList(child)
                outbound_node.next = outbound[parent]
                outbound[parent] = outbound_node

                # Add to inbound adjacency list
                inbound_node = LinkedList(parent)
                inbound_node.next = inbound[child]
                inbound[child] = inbound_node

            print(parent, child, cost)

            return inbound, outbound, costs, vertices, edges
    except FileNotFoundError:
        print(f"Failed to open file {filename}")
        return None, None, None, None, None


def main():
    start_time = time.time()
    if len(sys.argv) < 2:
        print("No file specified. Exiting...")
        return

    filename = sys.argv[1]
    inbound, outbound, costs, vertices, edges = read_data(filename)

    end_time = time.time()
    print(
        f"Process took {int((end_time - start_time) * 1000)} milliseconds to perform."
    )


if __name__ == "__main__":
    main()
