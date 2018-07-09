from routing.algorithms.vectors import DistanceVector


def get_edge(r1, r2):
    return str((min(r1, r2), max(r1, r2)))


def vertexes_to_list(vertexes):
    result = {}
    for r in vertexes:
        result[r] = list(vertexes[r])
    return result


def list_to_vertexes(vertexes):
    result = {}
    for r in vertexes:
        result[r] = set(vertexes[r])
    return result


class GraphVector(DistanceVector):

    def __init__(self, router_id):
        super().__init__(router_id)
        self.vertexes = {router_id: set()}
        self.edges = {}
        self.paths = {router_id: None}
        self.changed = False

    def remove_link(self, link, time):
        edge = get_edge(self.router_id, link.dst)
        self.edges[edge] = (time, False)
        self.vertexes[self.router_id].add(link.dst)
        self.vertexes[link.dst].add(self.router_id)
        self.changed = True

    def add_link(self, link, time):
        edge = get_edge(self.router_id, link.dst)
        self.edges[edge] = (time, True)
        self.vertexes[self.router_id].add(link.dst)
        if link.dst not in self.vertexes:
            self.vertexes[link.dst] = set()
        self.vertexes[link.dst].add(self.router_id)
        self.changed = True

    def _concat_vertexes(self, vertexes):
        change = False
        for r in vertexes:
            if r not in self.vertexes:
                self.vertexes[r] = vertexes[r]
                change = True
            for n in vertexes[r]:
                if n not in self.vertexes[r]:
                    change = True
                    self.vertexes[r].add(n)
        return change

    def _concat_edges(self, edges):
        change = False
        for e in edges:
            if e not in self.edges:
                self.edges[e] = edges[e]
                change = True
            elif self.edges[e][0] < edges[e][0]:
                self.edges[e] = edges[e]
                change = True
        return change

    def handle_vector(self, link, vector):
        change = False
        vertexes, edges = vector
        vertexes = list_to_vertexes(vertexes)
        change = self._concat_vertexes(vertexes) or change
        change = self._concat_edges(edges) or change
        self.changed = change or self.changed
        return change

    #transform set into list
    def init_paths(self, links):
        self.paths = {self.router_id: None}
        for link in links:
            self.paths[link.dst] = link

    def _add_to_paths(self, new_paths):
        for r in new_paths:
            self.paths[r] = new_paths[r]

    def calc_dist(self):
        new_paths = {}
        crr_paths = self.paths.copy()

        while len(crr_paths) > 0:
            for r in crr_paths:
                for n in self.vertexes[r]:
                    if n in self.paths or n in new_paths:
                        continue
                    edge = get_edge(r, n)
                    if self.edges[edge][1]:
                        new_paths[n] = crr_paths[r]
            self._add_to_paths(new_paths)
            crr_paths = new_paths
            new_paths = {}

    def give_link(self, router, links):
        if self.changed:
            self.changed = False
            self.init_paths(links)
            self.calc_dist()
        if router not in self.paths:
            return None
        else:
            return self.paths[router]

    def give_vector(self):
        return vertexes_to_list(self.vertexes), self.edges
