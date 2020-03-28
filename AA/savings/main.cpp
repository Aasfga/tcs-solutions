#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-msc51-cpp"

#include<iostream>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Point_set_2.h>
#include <random>
#include<vector>
#include<set>
#include<tuple>
#include <algorithm>
#include <unordered_map>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_with_info_2<int, K> Vb;
typedef CGAL::Triangulation_data_structure_2<Vb> Tds;
typedef CGAL::Point_set_2<K, Tds> Delaunay;
typedef Delaunay::Point Point;
typedef Delaunay::Vertex_handle Vertex_handle;

struct point;
struct vertex;
struct edge;
struct graph;
struct graph_builder;

typedef long long number;
typedef std::pair<point, int> input_type;
typedef std::pair<int, int> int_pair;

struct point {
    double x = 0;
    double y = 0;

    point() = default;

    point(const point &other) = default;

    point(double x, double y) : x(x), y(y) {}
};

struct edge {
    number cost = 0;
    vertex *u = nullptr;
    vertex *v = nullptr;

    edge() = default;

    edge(number cost, vertex *u, vertex *v) : cost(cost), u(u), v(v) {}

    vertex *operator()(vertex *x) {
        return x == u ? v : u;
    }
};

struct vertex : point {
    int id = -1;
    std::vector<edge *> edges;
    vertex *links[2] = {nullptr, nullptr};
    vertex *tail = nullptr;

    vertex() = default;

    vertex(const vertex &other) = default;

    vertex(int id, double x, double y) : id(id), point(x, y) {}

    void add_edge(edge *e) {
        edges.push_back(e);
    }

    vertex *next(vertex *last) {
        return links[last == links[0]];
    }

    void add_vertex(vertex *v) {
        links[links[1] == nullptr] = v;
    }

    bool is_end() {
        return links[0] == nullptr || links[1] == nullptr;
    }


};

number distance(double a_x, double a_y, double b_x, double b_y) {
    double x = a_x - b_x;
    double y = a_y - b_y;
    return floor(sqrt(x * x + y * y) + 0.5);
}

number distance(point *a, point *b) {
    return distance(a->x, a->y, b->x, b->y);
}

struct graph {
    std::vector<vertex> vertices;
    std::vector<edge> edges;


    void add_vertex(const vertex &x) {
        vertices.emplace_back(x);
    }

    void add_edge(int u_id, int v_id) {
        if(u_id == v_id)
            return;
        vertex *u = &vertices[u_id];
        vertex *v = &vertices[v_id];
        edges.emplace_back(distance(u, v), u, v);
        edge *e = &edges.back();
        u->add_edge(e);
        v->add_edge(e);
    }


    void clean() {
        for(auto &v : vertices) {
            v.links[0] = nullptr;
            v.links[1] = nullptr;
            v.tail = &v;
        }

    }

    friend std::ostream &operator<<(std::ostream &os, const graph &graph) {
        os << "Graph size:\n" <<
           "- vertices: " << graph.vertices.size() << "\n" <<
           "- edges: " << graph.edges.size() << "\n";
        os << "Edges:\n";
        for(const edge &e: graph.edges) {
            os << e.u->id << " " << e.v->id << " " << e.cost << "\n";
        }
        return os;
    }
};

struct graph_builder {
    typedef std::pair<Point, int> cgal_point;

    std::vector<input_type> points;
    std::set<int_pair> edges;
    Delaunay delaunay;

    graph_builder() = default;

    void add_edge(int u, int v) {
        if(u < v)
            edges.emplace(u, v);
        else
            edges.emplace(v, u);
    }

    graph_builder &add_points(std::vector<input_type> &input_points) {
        points = input_points;
        return *this;
    }

    graph_builder &create_delaunay_graph() {
        std::vector<cgal_point> d_points;
        point p;
        int id;
        for(input_type &pair : points) {
            std::tie(p, id) = pair;
            d_points.emplace_back(Point(p.x, p.y), id);
        }
        delaunay.insert(d_points.begin(), d_points.end());

        return *this;
    }

    graph_builder &add_delaunay_edges() {
        Delaunay::Finite_vertices_iterator v;
        Delaunay::Vertex_circulator u, end;
        for(v = delaunay.finite_vertices_begin(); v != delaunay.finite_vertices_end(); v++) {
            u = v->incident_vertices();
            end = u;
            if(u == nullptr) {
                continue;
            }
            do {
                if(delaunay.is_infinite(u))
                    continue;
                add_edge(v->info(), u->info());
            } while(++u != end);
        }
        return *this;
    }


    graph_builder &add_nearest_edges(unsigned int amount) {
        Delaunay::Finite_vertices_iterator v;
        for(v = delaunay.finite_vertices_begin(); v != delaunay.finite_vertices_end(); v++) {
            std::vector<Vertex_handle> neighbors;
            delaunay.nearest_neighbors(v, 1 + amount, back_inserter(neighbors));
            for(auto &u : neighbors) {
                add_edge(v->info(), u->info());
            }
        }
        return *this;
    }

    graph build() {
        graph g;
        for(input_type pair : points) {
            point p;
            int id;
            std::tie(p, id) = pair;
            vertex v = vertex(id, p.x, p.y);
            g.add_vertex(v);
        }

        g.edges.reserve(edges.size() + 2);
        for(int_pair pair: edges) {
            g.add_edge(pair.first, pair.second);
        }
        return g;
    }
};


struct tsp_savings_solver {
    typedef std::pair<number, edge *> item;
    typedef std::priority_queue<item> priority_queue;
    priority_queue queue;
    graph &g;
    vertex *root = nullptr;

    explicit tsp_savings_solver(graph &g) : g(g) {}


    number savings(vertex *u, vertex *v) {
        return distance(u, root) + distance(root, v) - distance(u, v);
    }

    void initialize() {
        static std::mt19937_64 generator(4321);
        std::uniform_int_distribution<unsigned int> dist(0, g.vertices.size() - 1);
        root = &g.vertices[dist(generator)];

        for(auto &v : g.vertices)
            v.tail = &v;

        for(auto &e : g.edges) {
            if(e.u == root || e.v == root)
                continue;
            number s = savings(e.u, e.v);
            queue.emplace(s, &e);
        }
    }


    static bool can_be_connected(vertex *u, vertex *v) {
        return u->is_end() && v->is_end() && v->tail != u;
    }

    static void connect(vertex *u, vertex *v) {
        u->add_vertex(v);
        v->add_vertex(u);
        vertex *u_tail = u->tail;
        vertex *v_tail = v->tail;
        u->tail->tail = v_tail;
        v->tail->tail = u_tail;
    }

    void connect_using_candidate_graph() {
        while(!queue.empty()) {
            edge *e = queue.top().second;
            queue.pop();
            if(!can_be_connected(e->u, e->v))
                continue;
            connect(e->u, e->v);
        }
    }

    void connect_using_all_edges() {
        std::vector<vertex *> endings;

        for(auto &v : g.vertices) {
            if(&v != root && v.is_end())
                endings.push_back(&v);
        }

        bool action = true;
        while(action) {
            action = false;
            for(auto v : endings) {
                if(!v->is_end())
                    continue;
                number max_s = -1;
                vertex *max_u = nullptr;
                for(auto u : endings) {
                    if(u == v || !u->is_end() || v->tail == u)
                        continue;
                    if(savings(u, v) > max_s) {
                        max_u = u;
                        max_s = savings(u, v);
                    }
                }
                if(max_u == nullptr)
                    continue;
                connect(max_u, v);
                action = true;
            }
        }
    }

    std::vector<vertex *> retrieve_result() {
        std::vector<vertex *> result;
        int i = 0;
        while(!g.vertices[i].is_end() || &g.vertices[i] == root)
            i++;
        root->add_vertex(&g.vertices[i]);
        g.vertices[i].add_vertex(root);

        vertex *prev = nullptr;
        vertex *crr = root;
        while(crr != nullptr) {
            result.push_back(crr);
            prev = crr->next(prev);
            std::swap(prev, crr);
        }
        result.push_back(root);

        return result;
    }

    std::vector<vertex *> solve() {
        initialize();
        connect_using_candidate_graph();
        connect_using_all_edges();
        return retrieve_result();
    }
};


std::vector<input_type> read_input() {
    int n;
    double x, y;
    std::vector<input_type> input;

    std::cin >> n;
    for(int i = 0; i < n; i++) {
        std::cin >> y >> x;
        input.emplace_back(point(x, y), i);
    }
    return input;
}

number calc_cycle_length(std::vector<vertex *> &cycle) {
    number result = 0;
    for(number i = 0; i + 1 < cycle.size(); i++) {
        result += distance(cycle[i], cycle[i + 1]);
    }
    return result;
}

void print_solution(std::vector<vertex *> &cycle, number length) {
    for(number i = 0; i + 1 < cycle.size(); i++)
        std::cout << cycle[i]->id << " ";
    std::cout << "\n" << length << "\n";
}


int main() {
    int tasks;
    std::cin >> tasks;

    int nearest_amount = 45;
    int shuffles = 4;

    while(tasks--) {
        std::vector<input_type> input = read_input();
        graph g = graph_builder().add_points(input)
                                 .create_delaunay_graph()
                                 .add_delaunay_edges()
                                 .add_nearest_edges(nearest_amount)
                                 .build();
        std::vector<vertex *> min_cycle;
        number min_length = INT32_MAX;
        for(int i = 0; i < shuffles; i++) {
            g.clean();
            auto result = tsp_savings_solver(g).solve();
            if(calc_cycle_length(result) < min_length) {
                min_cycle = result;
                min_length = calc_cycle_length(result);
            }
        }
        print_solution(min_cycle, min_length);
    }
}


