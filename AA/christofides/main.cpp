//#include<iostream>
//#include<CGAL/Exact_predicates_inexact_constructions_kernel.h>
//#include<CGAL/Delaunay_triangulation_2.h>
//#include<CGAL/Triangulation_vertex_base_with_info_2.h>
//#include<CGAL/Point_set_2.h>
//#include<random>
//#include<vector>
//#include<set>
//#include<queue>
//#include<list>
//#include<tuple>
//#include<algorithm>
//
//typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
//typedef CGAL::Triangulation_vertex_base_with_info_2<int, K> Vb;
//typedef CGAL::Triangulation_data_structure_2<Vb> Tds;
//typedef CGAL::Point_set_2<K, Tds> Delaunay;
//typedef Delaunay::Point Point;
//typedef Delaunay::Vertex_handle Vertex_handle;
//
//typedef long long int big_int;
//typedef std::pair<Point, int> ordered_point;
//typedef std::pair<int, int> int_pair;
//
//
//#define RETRIES 10
//#define NEAREST 10
//
//#define KOWALSKI_EDGES 2
//#define VERY_BIG_INT (2ul << 60u)
//
//big_int distance(double a_x, double a_y, double b_x, double b_y) {
//    double x = a_x - b_x;
//    double y = a_y - b_y;
//    return floor(sqrt(x * x + y * y) + 0.5);
//}
//
//big_int distance(const Point &a, const Point &b) {
//    return distance(a.x(), a.y(), b.x(), b.y());
//}
//
//struct edge {
//    big_int cost = 0;
//    bool is_eulerian = false;
//    int u = -1;
//    int v = -1;
//    bool visited = false;
//
//    edge() = default;
//
//    edge(int u, int v, big_int cost, bool is_eulerian = false) : u(u), v(v), cost(cost), is_eulerian(is_eulerian) {}
//
//    int adj_vertex(int id) {
//        return id != u ? u : v;
//    }
//};
//
//
//struct vertex {
//    int id;
//    bool visited = false;
//    u_int counter = 0;
//    std::vector<edge *> edges;
//    std::vector<edge *> euler_edges;
//    double x;
//    double y;
//
//
//    edge *eulerian_edge() {
//        edge *result = nullptr;
//        for(auto e : euler_edges)
//            result = e->visited ? result : e;
//        return result;
//    }
//
//    bool is_odd() {
//        return euler_edges.size() % 2 == 1;
//    }
//
//    bool is_leaf() {
//        return euler_edges.size() == 1;
//    }
//
//    explicit vertex(int id, double x, double y) : id(id), x(x), y(y) {};
//
//    void add_edge(edge *e) {
//        edges.push_back(e);
//    }
//
//    void add_euler_edge(edge *e) {
//        edges.push_back(e);
//        euler_edges.push_back(e);
//    }
//};
//
//big_int distance(vertex &a, vertex &b) {
//    return distance(a.x, a.y, b.x, b.y);
//}
//
//big_int distance(vertex *a, vertex *b) {
//    return distance(*a, *b);
//}
//
//struct graph {
//    std::vector<vertex> vertices;
//    std::vector<edge> edges;
//    std::list<vertex *> matching;
//
//    graph(std::vector<ordered_point> &points, std::vector<int_pair> &pairs) {
//        edges.reserve(pairs.size() * KOWALSKI_EDGES);
//        for(auto p : pairs) {
//            int u = p.first;
//            int v = p.second;
//            big_int cost = distance(points[u].first, points[v].first);
//            edges.emplace_back(u, v, cost);
//        }
//        for(auto p : points) {
//            int id = p.second;
//            auto &point = p.first;
//            vertices.emplace_back(id, point.x(), point.y());
//        }
//        for(auto &e : edges) {
//            vertices[e.u].add_edge(&e);
//            vertices[e.v].add_edge(&e);
//        }
//    }
//
//
//};
//
//std::vector<ordered_point> read_points(int n) {
//    std::vector<ordered_point> points;
//    double x, y;
//
//    for(int i = 0; i < n; i++) {
//        std::cin >> y >> x;
//        points.emplace_back(Point(x, y), i);
//    }
//    return points;
//}
//
//std::vector<int_pair> create_delaunay_graph(Delaunay &delaunay) {
//    std::vector<int_pair> edges;
//    Delaunay::Finite_vertices_iterator v;
//    Delaunay::Vertex_circulator u, end;
//    for(v = delaunay.finite_vertices_begin(); v != delaunay.finite_vertices_end(); v++) {
//        u = v->incident_vertices();
//        end = u;
//        if(u == nullptr) {
//            continue;
//        }
//        do {
//            if(delaunay.is_infinite(u))
//                continue;
//            edges.emplace_back(v->info(), u->info());
//        } while(++u != end);
//    }
//    return edges;
//}
//
//std::vector<int_pair> create_nn_graph(Delaunay &delaunay) {
//    std::vector<int_pair> edges;
//    Delaunay::Finite_vertices_iterator v;
//    for(v = delaunay.finite_vertices_begin(); v != delaunay.finite_vertices_end(); v++) {
//        std::vector<Vertex_handle> neighbors;
//        delaunay.nearest_neighbors(v, 1 + NEAREST, back_inserter(neighbors));
//        for(auto &u : neighbors) {
//            edges.emplace_back(v->info(), u->info());
//        }
//    }
//    return edges;
//}
//
//void add_edge(std::set<int_pair> &edges, int_pair e) {
//    if(e.first < e.second)
//        edges.insert(e);
//    else if(e.first != e.second)
//        edges.emplace(e.second, e.first);
//}
//
//graph create_candidate_graph(std::vector<ordered_point> points) {
//    Delaunay delaunay;
//    std::set<int_pair> set_edges;
//    delaunay.insert(points.begin(), points.end());
//    auto nn_edges = create_nn_graph(delaunay);
//    for(auto e : nn_edges)
//        add_edge(set_edges, e);
//    auto delaunay_edges = create_delaunay_graph(delaunay);
//    for(auto e : delaunay_edges)
//        add_edge(set_edges, e);
//    std::vector<int_pair> edges(set_edges.begin(), set_edges.end());
//    return graph(points, edges);
//}
//
//void set_euler_edges(graph &g) {
//    for(auto &e : g.edges) {
//        if(!e.is_eulerian)
//            continue;
//        g.vertices[e.u].euler_edges.push_back(&e);
//        g.vertices[e.v].euler_edges.push_back(&e);
//    }
//}
//
//typedef std::function<bool(edge *, edge *)> mst_comparator;
//typedef std::priority_queue<edge *, std::vector<edge *>, mst_comparator> mst_queue;
//
//void create_mst(graph &g) {
//    mst_comparator cmp = [](edge *a, edge *b) { return a->cost > b->cost; };
//    mst_queue queue(cmp);
//
//    vertex *v = &g.vertices.front();
//    v->visited = true;
//    for(edge *e : v->edges)
//        queue.push(e);
//
//    edge *e;
//    while(!queue.empty()) {
//        e = queue.top();
//        v = &g.vertices[e->v];
//        queue.pop();
// TODO change

//        if(e->is_eulerian)
//            continue;
//        if(v->visited)
//            v = &g.vertices[e->u];
//        if(v->visited)
//            continue;
//        v->visited = true;
//        e->is_eulerian = true;
//
//        for(edge *next : v->edges)
//            if(!next->is_eulerian)
//                queue.push(next);
// TODO change

//    }
//    set_euler_edges(g);
//}
//
//void add_euler_edge(graph &g, vertex *u, vertex *v) {
//    big_int cost = distance(u, v);
//    g.edges.emplace_back(u->id, v->id, cost, true);
//    edge *new_edge = &g.edges.back();
//    u->add_euler_edge(new_edge);
//    v->add_euler_edge(new_edge);
//}
//
//void add_euler_edge(graph &g, int u_id, int v_id) {
//    add_euler_edge(g, &g.vertices[u_id], &g.vertices[v_id]);
//}
//
//
//void duplicate_leaf_edges(graph &g) {
//    for(auto &v : g.vertices) {
//        if(v.is_leaf()) {
//            edge *leaf_edge = v.euler_edges.front();
//            add_euler_edge(g, leaf_edge->u, leaf_edge->v);
//        }
//    }
//}
//
//void create_convex_hull(graph &g) {
//    std::vector<ordered_point> points;
//    for(auto &v : g.vertices) {
//        if(v.is_odd()) {
//            points.emplace_back(Point(v.x, v.y), v.id);
//        }
//    }
//    Delaunay delaunay;
//    delaunay.insert(points.begin(), points.end());
//    Delaunay::Vertex_circulator v, end;
//    v = delaunay.incident_vertices(delaunay.infinite_vertex());
//    end = v;
//    if(v == nullptr)
//        return;
//    do {
//        g.matching.push_back(&g.vertices[v->info()]);
//    } while(++v != end);
//}
//
//std::vector<vertex *> get_odd_vertices(graph &g) {
//    std::vector<vertex *> odd_vertices;
//    for(auto &v : g.vertices)
//        v.visited = false;
//    for(auto &v : g.matching)
//        v->visited = true;
//    for(auto &v : g.vertices) {
//        if(!v.visited && v.is_odd())
//            odd_vertices.push_back(&v);
//    }
//    return odd_vertices;
//}
//
//
//typedef std::pair<big_int, vertex *> far_item;
//typedef std::function<bool(far_item &, far_item &)> far_comparator;
//typedef std::priority_queue<far_item, std::vector<far_item>, far_comparator> far_queue;
//
//far_queue initialize_queue(graph &g, std::vector<vertex *> &odd_vertices) {
//    far_comparator cmp = [](far_item &a, far_item &b) { return a.first > b.first; };
//    far_queue queue(cmp);
//
//    for(vertex *v : odd_vertices) {
//        vertex *max_u = g.matching.front();
//        for(vertex *u : g.matching)
//            max_u = distance(v, u) > distance(v, max_u) ? u : max_u;
//        queue.emplace(distance(v, max_u), v);
//        v->counter++;
//    }
//
//    return queue;
//}
//
//
//void circular_inc(std::list<vertex *> &list, std::list<vertex *>::iterator &iterator) {
//    iterator++;
//    if(iterator == list.end())
//        iterator = list.begin();
//}
//
//void farthest_insertion(graph &g) {
//    std::vector<vertex *> odd_vertices = get_odd_vertices(g);
//    far_queue queue = initialize_queue(g, odd_vertices);
//
//    vertex *v;
//    while(!queue.empty()) {
//        v = queue.top().second;
//        queue.pop();
//
//        if(v->counter > 1) {
//            v->counter--;
//            continue;
//        }
//
//        auto i = g.matching.begin();
//        auto j = i;
//        circular_inc(g.matching, j);
//        auto end = g.matching.end();
//        auto min = i;
//        big_int min_cost = VERY_BIG_INT;
//        while(i != end) {
//            big_int crr_cost = distance(*i, v) + distance(v, *j) - distance(*i, *j);
//            if(crr_cost < min_cost) {
//                min_cost = crr_cost;
//                min = j;
//            }
//            i++;
//            circular_inc(g.matching, j);
//        }
//        g.matching.insert(min, v);
//
//        for(edge *e : v->euler_edges) {
//            vertex *u = &g.vertices[e->adj_vertex(v->id)];
//            if(u->visited)
//                continue;
//            u->counter++;
//            queue.emplace(distance(v, u), u);
//        }
//    }
//}
//
//void add_matching(graph &g) {
//    big_int costs[2] = {0, 0};
//    auto index = 0;
//    auto i = g.matching.begin();
//    auto j = i;
//    auto end = g.matching.end();
//    circular_inc(g.matching, j);
//
//    while(i != end) {
//        costs[index] += distance(*i, *j);
//        index = 1 - index;
//
//        i++;
//        circular_inc(g.matching, j);
//    }
//    i = g.matching.begin();
//    j = g.matching.end();
//    circular_inc(g.matching, j);
//    if(costs[1] < costs[0]) {
//        i++;
//        circular_inc(g.matching, j);
//    }
//    while(i != end) {
//        add_euler_edge(g, *i, *j);
//        i++;
//        i++;
//        circular_inc(g.matching, j);
//        circular_inc(g.matching, j);
//    }
//}
//
//void create_matching(graph &g) {
//    create_convex_hull(g);
//    farthest_insertion(g);
//    add_matching(g);
//}
//
//
//void dfs_eulerian_cycle(graph &g, std::stack<vertex *> &stack, vertex *v) {
//    for(edge *e = v->eulerian_edge(); e != nullptr; e = v->eulerian_edge()) {
//        e->visited = true;
//        vertex *u = &g.vertices[e->adj_vertex(v->id)];
//        dfs_eulerian_cycle(g, stack, u);
//    }
//    stack.push(v);
//}
//
//std::vector<vertex *> create_eulerian_cycle(graph &g) {
//    for(auto &e : g.edges) {
//        e.visited = false;
//    }
//    std::stack<vertex *> stack;
//    dfs_eulerian_cycle(g, stack, &g.vertices.front());
//    std::vector<vertex *> cycle;
//    while(!stack.empty()) {
//        cycle.push_back(stack.top());
//        stack.pop();
//    }
//    return cycle;
//}
//
//std::vector<vertex *> create_hamiltonian_cycle(std::vector<vertex *> &eulerian) {
//    for(auto v: eulerian) {
//        v->visited = false;
//    }
//    std::vector<vertex *> result;
//    for(auto v: eulerian) {
//        if(v->visited)
//            continue;
//        v->visited = true;
//        result.push_back(v);
//    }
//    result.push_back(result.front());
//    return result;
//}
//
//big_int calc_length(std::vector<vertex *> &cycle) {
//    big_int result = 0;
//    for(big_int i = 0; i + 1 < cycle.size(); i++) {
//        result += distance(cycle[i], cycle[i + 1]);
//    }
//    return result;
//}
//
//void shuffle_graph(graph &g) {
//    std::mt19937_64 generator(12345);
//    for(auto &v : g.vertices) {
//        std::shuffle(v.edges.begin(), v.edges.end(), generator);
//        std::shuffle(v.euler_edges.begin(), v.euler_edges.end(), generator);
//    }
//}
//
//void print_solution(std::vector<vertex *> &cycle, big_int length) {
//    for(big_int i = 0; i + 1 < cycle.size(); i++)
//        std::cout << cycle[i]->id << " ";
//    std::cout << "\n" << length << "\n";
//}
//
//
//std::vector<vertex *> find_best_salesman_cycle(graph &g, int retries) {
//    big_int max_length = VERY_BIG_INT;
//    std::vector<vertex *> max_cycle;
//    while(retries--) {
//        auto eulerian = create_eulerian_cycle(g);
//        auto cycle = create_hamiltonian_cycle(eulerian);
//        big_int length = calc_length(cycle);
//        if(length < max_length) {
//            max_length = length;
//            max_cycle = cycle;
//        }
//        shuffle_graph(g);
//    }
//    return max_cycle;
//}
//
//int main() {
//    int tasks, n;
//    std::cin >> tasks;
//
//    while(tasks--) {
//        std::cin >> n;
//        auto points = read_points(n);
//        auto graph = create_candidate_graph(points);
//        create_mst(graph);
////        duplicate_leaf_edges(graph);
//        create_matching(graph);
//        auto hamiltonian = find_best_salesman_cycle(graph, RETRIES);
//        print_solution(hamiltonian, calc_length(hamiltonian));
//    }
//}




#include<iostream>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Point_set_2.h>
#include <random>
#include<vector>
#include<set>
#include<queue>
#include<list>
#include<tuple>
#include <algorithm>

#define DEBUG_CONVEX_HULL false

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_with_info_2<int, K> Vb;
typedef CGAL::Triangulation_data_structure_2<Vb> Tds;
typedef CGAL::Point_set_2<K, Tds> Delaunay;
typedef Delaunay::Point Point;
typedef Delaunay::Vertex_handle Vertex_handle;

typedef unsigned long long big_int;
typedef std::pair<Point, int> ordered_point;
typedef std::pair<int, int> int_pair;

#define SHUFFLES 25
#define NEAREST 20
#define KOWALSKI_EDGES 2

big_int distance(double a_x, double a_y, double b_x, double b_y) {
    double x = a_x - b_x;
    double y = a_y - b_y;
    return floor(sqrt(x * x + y * y) + 0.5);
}

big_int distance(Point &a, Point &b) {
    return distance(a.x(), a.y(), b.x(), b.y());
}

struct edge {
    big_int cost = 0;
    bool is_tree = false;
    int u = -1;
    int v = -1;
    bool visited = false;

    edge() = default;

    edge(int u, int v, big_int cost, bool is_tree = false) : u(u), v(v), cost(cost), is_tree(is_tree) {}

    int neighbour(int id) {
        return id != u ? u : v;
    }
};

struct vertex {
    int id;
    bool visited = false;
    u_int q_counter = 0;
    std::vector<edge *> neighbours;
    std::vector<edge *> tree_neighbours;
    double x;
    double y;


    edge *eulerian_edge() {
        edge *result = nullptr;
        for(auto e : tree_neighbours)
            result = e->visited ? result : e;
        return result;
    }

    explicit vertex(int id, double x, double y) : id(id), x(x), y(y) {};

    void add_edge(edge *e) {
        neighbours.push_back(e);
    }
};

big_int distance(vertex &a, vertex &b) {
    return distance(a.x, a.y, b.x, b.y);
}

struct graph {
    std::vector<vertex> vertices;
    std::vector<edge> edges;
    std::list<vertex *> odd_matching;

    graph(std::vector<ordered_point> &points, std::vector<int_pair> &pairs) {
        edges.reserve(pairs.size() * KOWALSKI_EDGES);
        for(auto p : pairs) {
            int u = p.first;
            int v = p.second;
            big_int cost = distance(points[u].first, points[v].first);
            edges.emplace_back(u, v, cost);
        }
        for(auto p : points) {
            int id = p.second;
            auto &point = p.first;
            vertices.emplace_back(id, point.x(), point.y());
        }
        for(auto &e : edges) {
            vertices[e.u].add_edge(&e);
            vertices[e.v].add_edge(&e);
        }
    }
};

std::vector<ordered_point> read_points(int n) {
    std::vector<ordered_point> points;
    double x, y;

    for(int i = 0; i < n; i++) {
        std::cin >> y >> x;
        points.emplace_back(Point(x, y), i);
    }
    return points;
}

std::vector<int_pair> create_delaunay_graph(Delaunay &delaunay) {
    std::vector<int_pair> edges;
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
            edges.emplace_back(v->info(), u->info());
        } while(++u != end);
    }
    return edges;
}

std::vector<int_pair> create_nn_graph(Delaunay &delaunay) {
    std::vector<int_pair> edges;
    Delaunay::Finite_vertices_iterator v;
    for(v = delaunay.finite_vertices_begin(); v != delaunay.finite_vertices_end(); v++) {
        std::vector<Vertex_handle> neighbors;
        delaunay.nearest_neighbors(v, 1 + NEAREST, back_inserter(neighbors));
        for(auto &u : neighbors) {
            edges.emplace_back(v->info(), u->info());
        }
    }
    return edges;
}

void add_edge(std::set<int_pair> &edges, int_pair e) {
    if(e.first < e.second)
        edges.insert(e);
    else if(e.first != e.second)
        edges.emplace(e.second, e.first);
}

graph create_candidate_graph(std::vector<ordered_point> points) {
    Delaunay delaunay;
    std::set<int_pair> set_edges;
    delaunay.insert(points.begin(), points.end());
    auto nn_edges = create_nn_graph(delaunay);
    for(auto e : nn_edges)
        add_edge(set_edges, e);
    auto delaunay_edges = create_delaunay_graph(delaunay);
    for(auto e : delaunay_edges)
        add_edge(set_edges, e);
    std::vector<int_pair> edges(set_edges.begin(), set_edges.end());
    return graph(points, edges);
}

void set_tree_edges(graph &g) {
    for(auto &e : g.edges) {
        if(!e.is_tree)
            continue;
        g.vertices[e.u].tree_neighbours.push_back(&e);
        g.vertices[e.v].tree_neighbours.push_back(&e);
    }
}

void create_mst(graph &g) {
    auto cmp = [](edge *a, edge *b) { return a->cost > b->cost; };
    std::priority_queue<edge *, std::vector<edge *>, decltype(cmp)> queue(cmp);

    g.vertices.front().visited = true;
    for(auto e : g.vertices.front().neighbours)
        queue.push(e);

    while(!queue.empty()) {
        edge *e = queue.top();
        vertex *v = &g.vertices[e->v];
        queue.pop();

        if(v->visited)
            v = &g.vertices[e->u];
        if(v->visited)
            continue;

        v->visited = true;
        e->is_tree = true;
        for(auto n : v->neighbours)
            queue.push(n);
    }
    set_tree_edges(g);
}

void add_euler_edge(vertex &v, edge *e) {
    v.neighbours.push_back(e);
    v.tree_neighbours.push_back(e);
}

void duplicate_leaf_edges(graph &g) {
    for(auto &v : g.vertices) {
        if(v.tree_neighbours.size() != 1)
            continue;
        edge *e = v.tree_neighbours.front();
        vertex &u = g.vertices[e->neighbour(v.id)];
        g.edges.emplace_back(e->u, e->v, e->cost, e->is_tree);
        edge &new_e = g.edges.back();
        add_euler_edge(v, &new_e);
        add_euler_edge(u, &new_e);
    }
}

void create_convex_hull(graph &g) {
    std::vector<ordered_point> points;
    for(auto &v : g.vertices) {
        if(v.tree_neighbours.size() % 2 == 1 || DEBUG_CONVEX_HULL)
            points.emplace_back(Point(v.x, v.y), v.id);
    }
    Delaunay delaunay;
    delaunay.insert(points.begin(), points.end());
    Delaunay::Vertex_circulator v, end;
    v = delaunay.incident_vertices(delaunay.infinite_vertex());
    end = v;
    if(v == nullptr)
        return;
    do {
        g.odd_matching.push_back(&g.vertices[v->info()]);
    } while(++v != end);
}

void add_matching_edge(graph &g, int u_id, int v_id) {
    big_int cost = distance(g.vertices[u_id], g.vertices[v_id]);
    g.edges.emplace_back(u_id, v_id, cost, true);
    edge *new_edge = &g.edges.back();
    add_euler_edge(g.vertices[u_id], new_edge);
    add_euler_edge(g.vertices[v_id], new_edge);
}

std::vector<vertex *> farthest_prepare(graph &g) {
    std::vector<vertex *> odd_vertices;
    for(auto &v : g.vertices)
        v.visited = false;
    for(auto &v : g.odd_matching)
        v->visited = true;
    for(auto &v: g.vertices) {
        if(!v.visited && v.tree_neighbours.size() % 2 == 1)
            odd_vertices.push_back(&v);
    }
    return odd_vertices;
}


void farthest_insertion(graph &g) {
    typedef std::pair<big_int, vertex *> q_item;
    std::vector<vertex *> odd_vertices = farthest_prepare(g);

    auto cmp = [](q_item &a, q_item &b) { return a.first > b.first; };
    std::priority_queue<q_item, std::vector<q_item>, decltype(cmp)> queue(cmp);

    for(auto v : odd_vertices) {
        big_int max_cost = -1;
        vertex *max_u = g.odd_matching.front();
        for(auto u : g.odd_matching) {
            if(distance(*v, *u) > max_cost) {
                max_cost = distance(*v, *u);
                max_u = u;
            }
        }
        queue.emplace(distance(*v, *max_u), v);
        v->q_counter += 1;
    }
    vertex *v;
    while(!queue.empty()) {
        v = queue.top().second;
        queue.pop();
        if(v->q_counter > 1) {
            v->q_counter--;
            continue;
        }
        auto first_iter = g.odd_matching.begin();
        auto second_iter = g.odd_matching.begin();
        second_iter++;
        second_iter = second_iter == g.odd_matching.end() ? g.odd_matching.begin() : second_iter;
        big_int min_cost = 0 - 1;
        auto min_iter = first_iter;
        while(first_iter != g.odd_matching.end()) {
            auto a = *first_iter;
            auto b = *second_iter;

            if(distance(*a, *v) + distance(*v, *b) - distance(*a, *b) < min_cost) {
                min_cost = distance(*a, *v) + distance(*v, *b) - distance(*a, *b);
                min_iter = first_iter;
            }

            first_iter++;
            second_iter++;
            second_iter = second_iter == g.odd_matching.end() ? g.odd_matching.begin() : second_iter;
        }
        g.odd_matching.insert(++min_iter, v);
        v->visited = true;
        for(auto &e : v->tree_neighbours) {
            auto &u = g.vertices[e->neighbour(v->id)];
            if(u.tree_neighbours.size() % 2 == 0 || u.visited)
                continue;
            u.q_counter++;
            queue.emplace(distance(u, *v), &u);
        }
    }
}

void add_matching(graph &g) {
    big_int n = g.odd_matching.size();
    big_int matchings[2] = {0, 0};
    int m = 0;
    auto first_iter = g.odd_matching.begin();
    auto second_iter = g.odd_matching.begin();
    second_iter++;
    second_iter = second_iter == g.odd_matching.end() ? g.odd_matching.begin() : second_iter;

    while(n--) {
        matchings[m] += distance(**first_iter, **second_iter);
        first_iter++;
        second_iter++;
        second_iter = second_iter == g.odd_matching.end() ? g.odd_matching.begin() : second_iter;
        m = 1 - m;
    }
    first_iter = g.odd_matching.begin();
    second_iter = g.odd_matching.begin();
    second_iter++;
    second_iter = second_iter == g.odd_matching.end() ? g.odd_matching.begin() : second_iter;
    if(matchings[0] > matchings[1]) {
        first_iter++;
        second_iter++;
        second_iter = second_iter == g.odd_matching.end() ? g.odd_matching.begin() : second_iter;
    }
    n = g.odd_matching.size() / 2;
    while(n--) {
        add_matching_edge(g, (*first_iter)->id, (*second_iter)->id);
        first_iter++;
        first_iter++;
        second_iter++;
        second_iter++;
        second_iter = second_iter == g.odd_matching.end() ? g.odd_matching.begin() : second_iter;
    }
}

void create_odd_matching(graph &g) {
    create_convex_hull(g);
    farthest_insertion(g);
    add_matching(g);
}


void dfs_eulerian_cycle(graph &g, std::vector<vertex *> &cycle, vertex *v) {
    for(edge *e = v->eulerian_edge(); e != nullptr; e = v->eulerian_edge()) {
        e->visited = true;
        vertex *u = &g.vertices[e->neighbour(v->id)];
        dfs_eulerian_cycle(g, cycle, u);
    }
    cycle.push_back(v);
}

std::vector<vertex *> create_eulerian_cycle(graph &g) {
    for(auto &e : g.edges) {
        e.visited = false;
    }
    std::vector<vertex *> cycle;
    dfs_eulerian_cycle(g, cycle, &g.vertices.front());
    return cycle;
}

std::vector<vertex *> create_hamiltonian_cycle(std::vector<vertex *> &eulerian) {
    for(auto v: eulerian) {
        v->visited = false;
    }
    std::vector<vertex *> result;
    for(auto v: eulerian) {
        if(v->visited)
            continue;
        v->visited = true;
        result.push_back(v);
    }
    result.push_back(eulerian.front());
    return result;
}

big_int calc_cycle_length(std::vector<vertex *> &cycle) {
    big_int result = 0;
    for(big_int i = 0; i + 1 < cycle.size(); i++) {
        result += distance(*cycle[i], *cycle[i + 1]);
    }
    return result;
}

void shuffle_graph(graph &g) {
    static std::mt19937_64 generator(1234);
    for(auto &v : g.vertices) {
        std::shuffle(v.neighbours.begin(), v.neighbours.end(), generator) ;
        std::shuffle(v.tree_neighbours.begin(), v.tree_neighbours.end(), generator);
    }
}

void print_solution(std::vector<vertex *> &cycle, big_int length) {
    for(big_int i = 0; i + 1 < cycle.size(); i++)
        std::cout << cycle[i]->id << " ";
    std::cout << "\n" << length << "\n";
}

int main() {
    int tasks, n;
    std::cin >> tasks;

    while(tasks--) {
        std::cin >> n;
        auto points = read_points(n);
        auto graph = create_candidate_graph(points);
        create_mst(graph);
//        duplicate_leaf_edges(graph);
        create_odd_matching(graph);
        big_int max_length = -1;
        std::vector<edge *> tree_edges;
        for(auto &e : graph.edges) {
            if(e.is_tree)
                tree_edges.push_back(&e);
        }
        std::vector<vertex *> max_cycle;
        for(int i = 0; i < SHUFFLES; i++) {
            auto eulerian = create_eulerian_cycle(graph);
            auto hamiltonian = create_hamiltonian_cycle(eulerian);
            big_int cycle_length = calc_cycle_length(hamiltonian);
            if(max_length > cycle_length) {
                max_length = cycle_length;
                max_cycle = hamiltonian;
            }
            shuffle_graph(graph);
        }
        print_solution(max_cycle, max_length);
    }
}
