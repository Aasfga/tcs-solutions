#include<iostream>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Point_set_2.h>
#include<vector>
#include<set>
#include<tuple>
#include <algorithm>
#include <unordered_map>

struct point;
struct vertex;
struct edge;
struct graph;
struct graph_builder;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_with_info_2<int, K> Vb;
typedef CGAL::Triangulation_data_structure_2 <Vb> Tds;
typedef CGAL::Point_set_2 <K, Tds> Delaunay;
typedef Delaunay::Point Point;
typedef Delaunay::Vertex_handle Vertex_handle;

typedef std::pair<point, int> input_type;
typedef std::pair<int, int> int_pair;


//typename<...>
//struct gowno {
//
//};

struct point {
    double x = 0;
    double y = 0;

    point() = default;

    point(const point &other) = default;

    point(double x, double y) : x(x), y(y) {}
};

struct edge {
    int cost = 0;
    vertex *u = nullptr;
    vertex *v = nullptr;
    bool used = false;

    edge() = default;

    edge(int cost, vertex *u, vertex *v) : cost(cost), u(u), v(v) {}

    vertex *neighbour(vertex *x) {
        return x == u ? v : u;
    }

    vertex *operator()(vertex *x) {
        return x == u ? v : u;
    }
};

struct vertex : point {
    int id = -1;
    std::vector<edge *> edges;

    vertex() = default;

    vertex(const vertex &other) = default;

    vertex(int id, double x, double y) : id(id), point(x, y) {}

    void add_edge(edge *e) {
        edges.push_back(e);
    }
};

int distance(double a_x, double a_y, double b_x, double b_y) {
    double x = a_x - b_x;
    double y = a_y - b_y;
    return floor(sqrt(x * x + y * y) + 0.5);
}

int distance(point *a, point *b) {
    return distance(a->x, a->y, b->x, b->y);
}

int calc_length(std::vector<vertex *> &vertices) {
    int result = 0;
    for(int i = 0; i + 1 < vertices.size(); i++) {
        result += distance(vertices[i], vertices[i + 1]);
    }
    return result;
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

    graph_builder &add_points(std::vector<input_type> input_points) {
        points = std::move(input_points);
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

struct node {
    node *_left = nullptr;
    node *_right = nullptr;
    node *parent = nullptr;
    vertex *value;
    int size;
    bool reversed = false;

    node(vertex *value, int size, node *parent) : value(value), size(size), parent(parent) {}

    void reverse() {
        if(!reversed)
            return;
        if(_left != nullptr)
            _left->reversed = !_left->reversed;
        if(_right != nullptr)
            _right->reversed = !_right->reversed;
        std::swap(_left, _right);
        reversed = false;
    }

    node *get_left() {
        reverse();
        return _left;
    }

    node *get_right() {
        reverse();
        return _right;
    }

    void set_left(node *n) {
        reverse();
        _left = n;
    }

    void set_right(node *n) {
        reverse();
        _right = n;
    }

    int left_size() {
        return get_left() == nullptr ? 0 : get_left()->size;
    }

    int right_size() {
        return get_right() == nullptr ? 0 : get_right()->size;
    }

    void update_size() {
        size = left_size() + right_size() + 1;
    }

    bool is_left(node *n) {
        return get_left() == n;
    }

    bool is_right(node *n) {
        return get_right() == n;
    }
};

struct splay_tree {
    std::vector<node> nodes;
    node *root;
    int size;

    explicit splay_tree(std::vector<vertex> &vertices) : size(vertices.size()) {
        nodes.reserve(size);
        for(int i = 0; i < size; i++)
            nodes.emplace_back(&vertices[i], size - i, nullptr);
        for(int i = 0; i + 1 < size; i++)
            nodes[i].set_right(&nodes[i + 1]);
        for(int i = 1; i < size; i++) {
            nodes[i].parent = &nodes[i - 1];
        }
        root = &nodes[0];
    }

    static bool is_root(node *n) {
        return n->parent == nullptr;
    }

    static bool is_zig_zig(node *x) {
        if(is_root(x))
            return false;
        node *p = x->parent;
        if(is_root(p))
            return false;
        node *g = p->parent;
        if(g->is_left(p) && p->is_left(x))
            return true;
        return g->is_right(p) && p->is_right(x);
    }

    static bool is_zig_zag(node *x) {
        if(is_root(x))
            return false;
        node *p = x->parent;
        if(is_root(p))
            return false;
        node *g = p->parent;

        if(g->is_right(p) && p->is_left(x))
            return true;
        return g->is_left(p) && p->is_right(x);
    }

    static void replace_child(node *n, node *old_node, node *new_node) {
        new_node->parent = n;
        if(n != nullptr) {
            if(n->is_left(old_node))
                n->set_left(new_node);
            else if(n->is_right(old_node))
                n->set_right(new_node);
        }
    }

    static void remove_child(node *parent, node *child) {
        child->parent = nullptr;
        if(parent->is_left(child))
            parent->set_left(nullptr);
        else if(parent->is_right(child))
            parent->set_right(nullptr);
    }

    static void add_left_child(node *parent, node *child) {
        if(child != nullptr)
            child->parent = parent;
        parent->set_left(child);
        parent->update_size();
    }

    static void add_right_child(node *parent, node *child) {
        if(child != nullptr)
            child->parent = parent;
        parent->set_right(child);
        parent->update_size();

    }

    static void left_zig(node *x) {
        node *p = x->parent;
        replace_child(p->parent, p, x);
        node *a = x->get_left();
        x->set_left(p);
        p->parent = x;
        p->set_right(a);
        if(a != nullptr)
            a->parent = p;
        p->update_size();
        x->update_size();
    }

    static void right_zig(node *x) {
        node *p = x->parent;
        replace_child(p->parent, p, x);
        node *b = x->get_right();
        x->set_right(p);
        p->parent = x;
        p->set_left(b);
        if(b != nullptr)
            b->parent = p;
        p->update_size();
        x->update_size();
    }

    static void zig_zig(node *x) {
        node *p = x->parent;
        if(p->is_left(x)) {
            right_zig(p);
            right_zig(x);
        } else if(p->is_right(x)) {
            left_zig(p);
            left_zig(x);
        }
    }

    static void zig_zag(node *x) {
        node *p = x->parent;

        if(p->is_left(x)) {
            right_zig(x);
            left_zig(x);
        } else if(p->is_right(x)) {
            left_zig(x);
            right_zig(x);
        }
    }

    static node *find(node *n, int index) {
        node *crr = n;
        crr->reverse();

        while(crr->left_size() + 1 != index) {
            if(index <= crr->left_size())
                crr = crr->get_left();
            else {
                index -= crr->left_size() + 1;
                crr = crr->get_right();
            }
            if(crr->reversed)
                crr->reverse();
        }

        return crr;
    }

    void refresh_root() {
        while(root->parent != nullptr)
            root = root->parent;
    }

    static void update_family_sizes(node *n) {
        node *crr = n;
        while(crr != nullptr) {
            crr->update_size();
            crr = crr->parent;
        }
    }

    static void reverse_family(node *n) {
        if(n == nullptr)
            return;
        reverse_family(n->parent);
        n->reverse();
    }

    static void splay(node *n) {
        while(!is_root(n)) {
            node *parent = n->parent;
            if(is_zig_zig(n))
                zig_zig(n);
            else if(is_zig_zag(n))
                zig_zag(n);
            else if(parent->is_left(n))
                right_zig(n);
            else if(parent->is_right(n))
                left_zig(n);
            else
                assert(false);
        }
    }

    void splay(int index) {
        node *n = find(root, index);
        splay(n);
        refresh_root();
    }

    void reverse_prefix(int j) {
        if(j < size) {
            splay(j + 1);
            node *left_child = root->get_left();
            left_child->reversed = !left_child->reversed;
        } else {
            splay(size);
            root->reversed = !root->reversed;
        }
    }

    vertex *get(int i) {
        splay(i);
        return root->value;
    }

    static node *first_node(node *n) {
        node *first = find(n, 1);
        node *parent = n->parent;
        n->parent = nullptr;
        splay(first);
        replace_child(parent, n, first);
        return first;
//
//        while(first->get_left() != nullptr)
//            first = first->get_left();
//        return first;
    }

    vertex *first() {
        return get(1);
    }

    static node *last_node(node *n) {
        node *last = find(n, n->size);
        node *parent = n->parent;
        n->parent = nullptr;
        splay(last);
        replace_child(parent, n, last);
        return last;
    }

    vertex *last() {
        return get(size);
    }


    int get_index(vertex *v) {
        node *n = &nodes[v->id];
        reverse_family(n);
        splay(n);
        refresh_root();
        return n->left_size() + 1;
    }

    void reverse(int i, int j) {
        reverse_prefix(j);
        reverse_prefix(j - i + 1);
        reverse_prefix(j);
    }

    static void shift_right(node *n) {
        if(n->size == 1)
            return;
        node *parent = n->parent;
        n->parent = nullptr;

        node *last = last_node(n);
        splay(last);
        node *vice_last = last->get_left();
        while(vice_last->get_right() != nullptr)
            vice_last = vice_last->get_right();
        splay(vice_last);
        remove_child(vice_last, last);
        node *first = first_node(vice_last);
        add_left_child(first, last);
        assert(first->parent == nullptr);
        update_family_sizes(last);
        replace_child(parent, n, first);
    }

    void shift_right(int i) {
        if(i == 1)
            shift_right(root);
        else {
            splay(i - 1);
            shift_right(root->get_right());
        }
        refresh_root();
    }

    void shift_right() {
        shift_right(1);
    }


    void shift_left(int i) {
        reverse(i, size);
        shift_right(i);
        reverse(i, size);
    }

    void shift_left() {
        shift_left(size);
    }

    vertex *next(vertex *v) {
        int index = get_index(v);
        if(index == size)
            return get(1);
        return get(index + 1);
    }

    vertex *prev(vertex *v) {
        int index = get_index(v);
        if(index == 1)
            return get(size);
        return get(index - 1);
    }

    std::vector<vertex *> get_sorted_elements() {
        std::vector<vertex *> result;
        for(int i = 1; i <= size; i++)
            result.push_back(get(i));
        return result;
    }
};

struct move_reverse {
    int start;
    int end;
};

struct move_last {
    int index;
};

struct move_nothing {
};

enum move_type {
    REVERSE,
    LAST,
    NOTHING
};

struct move {
    move_type type;
    union {
        move_reverse reverse;
        move_last last;
        move_nothing nothing;
    };
};

struct tour {
    splay_tree tree;

    explicit tour(std::vector<vertex> &vertices) : tree(vertices) {}

    void shift_elements() {
        tree.shift_right(1);
    }

    void reverse_elements(int start, int end) {
        tree.reverse(start, end);
    }

    void make_last(int index) {
        tree.shift_left(index);
    }

    void reverse_move(move_reverse m) {
        reverse_elements(m.start, m.end);
    }

    void reverse_move(move_last m) {
        tree.shift_right(m.index);
    }

    void apply_move(move m) {
        if(m.type == REVERSE)
            reverse_elements(m.reverse.start, m.reverse.end);
        else if(m.type == LAST)
            make_last(m.last.index);
    }

    void reverse_move(move m) {
        if(m.type == REVERSE)
            reverse_move(m.reverse);
        else if(m.type == LAST)
            reverse_move(m.last);
    }

    vertex *last_vertex() {
        return tree.last();
    }

    vertex *first_vertex() {
        return tree.first();
    }

    vertex *next_vertex(vertex *v) {
        return tree.next(v);
    }

    vertex *prev_vertex(vertex *v) {
        return tree.prev(v);
    }

    int get_index(vertex *v) {
        return tree.get_index(v);
    }
};


struct lk_optimizer {
    typedef std::pair<move, int> saving_move;
    typedef std::pair<edge *, int> saving_edge;

    std::vector<vertex *> vertices;
    std::vector<edge *> used_edges;
    tour t;
    int tour_length;

    explicit lk_optimizer(std::vector<vertex> &vertices) : t(vertices) {
        for(auto &v : vertices)
            this->vertices.push_back(&v);
        tour_length = calc_length(this->vertices);
        tour_length += distance(t.first_vertex(), t.last_vertex());
    }

    void clean_edges() {
        for(edge *e : used_edges)
            e->used = false;
        used_edges.clear();
    }

    struct move_container {
        bool is_opt;
        edge *e;
        int savings;
        vertex *mid;
        vertex *next;
        vertex *last;
    };


    static int opt_savings(vertex *first, vertex *prev, vertex *mid, vertex *next, vertex *last) {
        if(next == last || prev == last)
            return INT32_MIN;
        return distance(mid, next) + distance(last, first) - distance(mid, last) - distance(next, first);
    }

    static int insertion_savings(vertex *first, vertex *prev, vertex *mid, vertex *next, vertex *last) {
        if(mid == first)
            return INT32_MIN;
        return distance(prev, mid) + distance(mid, next) + distance(first, last) - distance(mid, last) -
               distance(prev, next) - distance(mid, first);
    }

    move_container get_best_move(vertex *active) {
        int max_savings = INT32_MIN;
        edge *max_edge = nullptr;
        bool is_opt = false;
        vertex *max_mid = nullptr;
        vertex *max_next = nullptr;
        vertex *max_last = nullptr;


        vertex *first = t.first_vertex();
        vertex *last = active;
        for(edge *e : active->edges) {
            if(e->used)
                continue;
            vertex *mid = e->neighbour(last);
            vertex *next = t.next_vertex(mid);
            vertex *prev = t.prev_vertex(mid);

            int opt = opt_savings(first, prev, mid, next, last);
            int insertion = insertion_savings(first, prev, mid, next, last);

            if(max_savings <= opt || max_savings <= insertion) {
                max_savings = std::max(opt, insertion);
                max_edge = e;
                is_opt = opt >= insertion;
                max_mid = mid;
                max_next = next;
                max_last = last;
            }
        }
        return {is_opt, max_edge, max_savings, max_mid, max_next, max_last};
    }

    saving_move create_opt_move(edge *e, int savings, vertex *next, vertex *last) {
        e->used = true;
        used_edges.push_back(e);
        return {{REVERSE, {t.get_index(next), t.get_index(last)}}, savings};
    }

    saving_move create_insertion_move(edge *e, int savings, vertex *mid) {
        e->used = true;
        used_edges.push_back(e);
        return {{LAST, {t.get_index(mid)}}, savings};
    }

    saving_move pick_best_move() {
        vertex *active = t.last_vertex();
        auto container = get_best_move(active);
        if(container.e == nullptr)
            return {{NOTHING, {}}, 0};
        if(container.is_opt)
            return create_opt_move(container.e, container.savings, container.next, container.last);
        return create_insertion_move(container.e, container.savings, container.mid);
    }

    void reverse_moves(std::vector<saving_move> &moves, int min_cost, int crr_cost) {
        for(auto i = moves.rbegin(); i != moves.rend(); i++)
            t.reverse_move(i->first);
        crr_cost = tour_length;
        for(auto i = moves.begin(); i != moves.end() && crr_cost != min_cost; i++) {
            t.apply_move(i->first);
            crr_cost -= i->second;
        }
        tour_length = crr_cost;
    }

    bool simulation(int rounds) {
        std::vector<saving_move> moves;
        int min_cost = tour_length;
        int crr_cost = tour_length;
        move m;
        int s;

        for(int i = 0; i < rounds; i++) {
            std::tie(m, s) = pick_best_move();
            moves.emplace_back(m, s);
            t.apply_move(m);
            crr_cost -= s;
            min_cost = std::min(crr_cost, min_cost);
        }

        bool result = tour_length != min_cost;
        reverse_moves(moves, min_cost, crr_cost);
        return result;
    }

    void solve(int rounds) {
        bool optimized;
        return;
        do {
            optimized = false;
            int max_i = vertices.size();
            for(int i = 0; i < max_i && !optimized; i++) {
                optimized = simulation(rounds) || optimized;
                t.shift_elements();
                clean_edges();
            }
        } while(optimized);
    }

    std::vector<vertex *> get_solution() {
        auto path = t.tree.get_sorted_elements();
        path.push_back(path.front());
        return path;
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

void print_solution(std::vector<vertex *> &cycle, int length) {
    for(int i = 0; i + 1 < cycle.size(); i++)
        std::cout << cycle[i]->id << " ";
    std::cout << "\n" << length << "\n";
}

int main() {
    int tasks;
    std::cin >> tasks;

    while(tasks--) {
        auto graph = graph_builder().add_points(read_input())
                                    .create_delaunay_graph()
                                    .add_delaunay_edges()
                                    .add_nearest_edges(12)
                                    .build();
        auto optimizer = lk_optimizer(graph.vertices);
        optimizer.solve(0);
        auto solution = optimizer.get_solution();
        print_solution(solution, calc_length(solution));
    }
}