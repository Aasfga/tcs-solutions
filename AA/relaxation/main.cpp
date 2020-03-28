#include<iostream>
#include<vector>
#include<set>
#include<tuple>
#include <algorithm>
#include <functional>
#include <cmath>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Point_set_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_with_info_2<int, K> Vb;
typedef CGAL::Triangulation_data_structure_2<Vb> Tds;
typedef CGAL::Point_set_2<K, Tds> Delaunay;
typedef Delaunay::Point Point;
typedef Delaunay::Vertex_handle Vertex_handle;

struct vertex;

typedef int number;
typedef double float_number;
typedef std::pair<int, int> int_pair;
typedef std::pair<vertex *, vertex *> edge;
typedef std::function<number(vertex *, vertex *)> distance_function;
typedef std::vector<vertex *> vertex_container;
typedef std::vector<edge> edge_container;

#define DEBUG false
#define PRINT_VERTICES false

double lambda = 0.98;
double equation_constant = 10;
int iterations = 500;
int nearest_neighbors = 10;
int debug_delta = 20;
double p = 0.6;

struct vertex {
    int id = -1;
    double x = 0;
    double y = 0;

    vertex_container edges;

    float_number pi = 0;
    number degree[2] = {0, 0};

    vertex_container mst_edges;
    number rank = 0;
    vertex *parent = nullptr;

    vertex(int id, double x, double y) : id(id), x(x), y(y) {
        refresh_vertex();
    }

    void add_edge(vertex *e) {
        edges.push_back(e);
    }

    void add_mst_edge(vertex *e) {
        mst_edges.push_back(e);
    }

    void refresh_vertex() {
        parent = this;
        rank = 0;
        mst_edges.clear();
    }

    bool is_leaf() {
        assert(!mst_edges.empty());
        return mst_edges.size() <= 1;
    }

    vertex *leaf_edge() {
        assert(is_leaf());
        return mst_edges[0];
    }

    number calc_degree() {
        return mst_edges.size() - 2;
    }

    float_number calc_new_pi(float_number step) {
        return pi + step * (p * degree[0] + (1.0 - p) * degree[1]);
    }
};

number euclidean_distance(vertex *a, vertex *b) {
    double x = a->x - b->x;
    double y = a->y - b->y;

    return floor(sqrt(x * x + y * y) + 0.5);
}

number pi_distance(vertex *a, vertex *b) {
    return euclidean_distance(a, b) + a->pi + b->pi;
}

number length_cycle(vertex_container &vertices, const distance_function &distance) {
    int result = 0;
    for(int i = 0; i + 1 < vertices.size(); i++) {
        result += distance(vertices[i], vertices[i + 1]);
    }
    return result + distance(vertices.back(), vertices.front());
}

number length_1tree(vertex_container &vertices, edge e, const distance_function &distance) {
    number sum = 0;

    for(vertex *v : vertices) {
        for(vertex *u : v->mst_edges) {
            sum += distance(v, u);
        }
    }
    sum /= 2;
    return sum + distance(e.first, e.second);
}

struct graph_builder {
    typedef std::pair<Point, int> cgal_point;
    Delaunay delaunay;
    vertex_container vertices;
    std::set<int_pair> edges;

    graph_builder() = default;

    void add_edge(int u, int v) {
        if(u < v)
            edges.emplace(u, v);
        else if(u > v)
            edges.emplace(v, u);
    }

    void add_edge(int_pair pair) {
        vertex *u = vertices[pair.first];
        vertex *v = vertices[pair.second];
        v->add_edge(u);
        u->add_edge(v);
    }

    graph_builder &read_points() {
        int n;
        double x, y;

        std::cin >> n;
        for(int i = 0; i < n; i++) {
            std::cin >> y >> x;
            vertices.push_back(new vertex(i, x, y));
        }
        return *this;
    }

    graph_builder &create_delaunay_graph() {
        std::vector<cgal_point> d_points;
        for(vertex *v : vertices) {
            d_points.emplace_back(Point(v->x, v->y), v->id);
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

    graph_builder &add_all_edges() {
        for(vertex *v : vertices) {
            for(vertex *u : vertices) {
                add_edge(v->id, u->id);
            }
        }
        return *this;
    }

    vertex_container build() {
        for(const int_pair &pair : edges) {
            add_edge(pair);
        }
        return std::move(vertices);
    }
};

vertex *get_root(vertex *v) {
    vertex *root = v;

    while(root->parent != root)
        root = root->parent;

    vertex *crr = v;
    while(crr != root) {
        vertex *next = crr->parent;
        crr->parent = root;
        crr = next;
    }
    return root;
}

void connect(vertex *v, vertex *u) {
    v->add_mst_edge(u);
    u->add_mst_edge(v);
    vertex *first = get_root(v);
    vertex *second = get_root(u);
    if(first->rank < second->rank)
        std::swap(first, second);
    first->rank++;
    second->parent = first;
}

void create_mst(vertex_container &vertices, edge_container &edges, const distance_function &distance) {
    for(vertex *v : vertices)
        v->refresh_vertex();
    std::sort(edges.begin(), edges.end(), [distance](edge &a, edge &b) {
        return distance(a.first, a.second) < distance(b.first, b.second);
    });

    for(edge e : edges) {
        vertex *u, *v;
        std::tie(u, v) = e;
        if(get_root(u) != get_root(v)) {
            connect(v, u);
        }
    }
}

edge_container get_edges(vertex_container &vertices) {
    std::set<edge> edges;
    for(vertex *v : vertices) {
        for(vertex *u : v->edges) {
            if(v->id < u->id)
                edges.emplace(v, u);
            else if(u->id < v->id)
                edges.emplace(u, v);
        }
    }
    edge_container result;
    std::copy(edges.begin(), edges.end(), std::back_inserter(result));
    return result;
}

edge find_best_1tree(vertex_container &vertices, edge_container &edges, const distance_function &distance) {
    create_mst(vertices, edges, distance);
    std::vector<vertex *> leafs;

    for(vertex *v : vertices) {
        if(v->is_leaf())
            leafs.push_back(v);
    }

    vertex *max_leaf = nullptr;
    vertex *max_u = nullptr;
    number max_distance = std::numeric_limits<number>::min();

    for(vertex *leaf : leafs) {
        vertex *min_u = nullptr;
        number min_distance = std::numeric_limits<number>::max();
        for(vertex *v : leaf->edges) {
            if(v == leaf->leaf_edge())
                continue;
            float_number crr_distance = distance(leaf, v);
            if(crr_distance < min_distance) {
                min_u = v;
                min_distance = crr_distance;
            }
        }
        if(min_distance > max_distance) {
            max_leaf = leaf;
            max_u = min_u;
            max_distance = min_distance;
        }
    }
    return {max_leaf, max_u};
}

float_number get_initial_step(vertex_container &vertices, edge_container &edges) {
    edge e = find_best_1tree(vertices, edges, euclidean_distance);
    number x = length_cycle(vertices, euclidean_distance);
    number y = length_1tree(vertices, e, euclidean_distance);
    number n = vertices.size();
    return equation_constant * (x - y) / n;
}

number calc_lower_bound(std::vector<vertex *> &vertices, edge e, const distance_function &distance) {
    float_number pi_length = length_1tree(vertices, e, distance);
    float_number pi_sum = 0;
    for(vertex *v : vertices)
        pi_sum += v->pi;
    return std::ceil(pi_length - 2.0 * pi_sum);
}

void print_iteration(int i, vertex_container &vertices, edge_container &edges, float_number step) {
    if(i % debug_delta != 0)
        return;
    edge e = find_best_1tree(vertices, edges, pi_distance);
    number lower_bound = calc_lower_bound(vertices, e, pi_distance);
    printf("%i:\nlower_bound= %i step=%.2f\n", i, lower_bound, step);
    if(!PRINT_VERTICES)
        return;
    for(vertex *v : vertices)
        printf("%.1f ", v->pi);
    std::cout << "\n";
    for(vertex *v : vertices)
        std::cout << v->degree[0] << " ";
    std::cout << "\n";
}

void add_mst_edge(edge e) {
    e.first->add_mst_edge(e.second);
    e.second->add_mst_edge(e.first);
}

void perform_simulation_step(vertex_container &vertices, edge_container &edges, float_number step) {
    edge best_edge = find_best_1tree(vertices, edges, pi_distance);
    add_mst_edge(best_edge);

    for(vertex *v : vertices) {
        v->degree[1] = v->degree[0];
        v->degree[0] = v->calc_degree();
        v->pi = v->calc_new_pi(step);
    }
}


number lower_bound(vertex_container &vertices) {
    edge_container edges = get_edges(vertices);
    float_number step = get_initial_step(vertices, edges);

    for(u_long i = 0; i < iterations; i++) {
        if(DEBUG) print_iteration(i, vertices, edges, step);

        perform_simulation_step(vertices, edges, step);
        step *= lambda;
    }
    edge e = find_best_1tree(vertices, edges, pi_distance);
    return calc_lower_bound(vertices, e, pi_distance);
}

void clear_vertices(vertex_container &vertices) {
    for(vertex *v : vertices) {
        delete v;
    }
    vertices.clear();
}

int main() {
    int tasks;
    std::cin >> tasks;

    while(tasks--) {
        auto vertices = graph_builder().read_points()
                                       .create_delaunay_graph()
                                       .add_delaunay_edges()
                                       .add_nearest_edges(nearest_neighbors)
                                       .build();
        std::cout << lower_bound(vertices) << "\n";
        clear_vertices(vertices);
    }
}