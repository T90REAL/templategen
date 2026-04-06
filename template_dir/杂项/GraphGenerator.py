import random
from random import randint


class DirectedGraph:
    def __init__(self):
        self.graph = {}

    def add_edge(self, start, end, weight):
        if start not in self.graph:
            self.graph[start] = {}
        self.graph[start][end] = weight

    def generate_random_graph(self, num_vertices, num_edges):
        for _ in range(num_edges):
            start = random.randint(0, num_vertices - 1)
            end = random.randint(0, num_vertices - 1)
            weight = random.randint(1, 10)
            self.add_edge(start, end, weight)

    def print_graph(self):
        for start in self.graph:
            for end in self.graph[start]:
                # print(f"{start} -> {end} : {self.graph[start][end]}")
                print(start, end, self.graph[start][end])


# 生成一个带有n个顶点，m条边的图
n, m = randint(1, 1000), randint(1, 1000)
dg = DirectedGraph()
dg.generate_random_graph(n, m)
print(n, m)
dg.print_graph()
