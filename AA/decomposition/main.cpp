#include <iostream>
#include <utility>
#include <vector>
#include <map>
#include <cmath>
#include <algorithm>
#include <set>
#include <list>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Point_set_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_with_info_2<int, K> Vb;
typedef CGAL::Triangulation_data_structure_2<Vb> Tds;
typedef CGAL::Point_set_2<K, Tds> delaunay;
typedef delaunay::Point cgal_point;
typedef delaunay::Vertex_handle vertex_handle;


struct box;
struct point;
struct vertex;
struct edge;

typedef double float_number;
typedef int number;
typedef std::pair<number, number> number_pair;
typedef std::pair<number, vertex *> cost_pair;

#define OPT_COUNTER 25
#define ROUNDS 1
#define SIZE_C 2
#define NEAREST 10


struct point {
    number id;
    float_number x;
    float_number y;

    point() = default;

    point(number id, float_number x, float_number y) : id(id), x(x), y(y) {}
};


struct edge {
    vertex *u;
    vertex *v;
    bool used = false;

    edge(vertex *u, vertex *v, number cost) : u(u), v(v) {}

    vertex *adj_vertex(vertex *x) {
        return x == u ? v : u;
    }
};

struct vertex : public point {
    number box_id;
    std::vector<edge *> edges;
    bool is_ending = false;
    number counter = 0;
    bool used = false;
    number splay_id;

    vertex(number id, number box_id, float_number x, float_number y) : point(id, x, y), box_id(box_id) {}
};

number distance(vertex *a, vertex *b) {
    double x = a->x - b->x;
    double y = a->y - b->y;
    return std::floor(std::sqrt(x * x + y * y) + 0.5);
}

struct box {
    number id;
    std::vector<vertex *> vertices;
    std::vector<vertex *> convex_hull;
    std::map<number, cost_pair> costs;
    number counter = 0;

    number get_cost(box *b) {
        return costs[b->id].first;
    }

    vertex *get_link(box *b) {
        return costs[b->id].second;
    }
};

number distance(box *a, box *b) {
    return a->get_cost(b);
}

struct graph {
    std::vector<vertex> vertices;
    std::vector<edge *> edges;
    std::vector<box> boxes;

    void add_edge(int u_id, int v_id) {
        vertex *u = &vertices[u_id];
        vertex *v = &vertices[v_id];
        edges.push_back(new edge(u, v, distance(u, v)));
        edge *e = edges.back();
        u->edges.push_back(e);
        v->edges.push_back(e);
    }

    void clean_edges() {
        for(edge *e : edges)
            delete e;
        edges.clear();
    }

    virtual ~graph() {
        for(edge *e : edges)
            delete e;
    }
};

struct edge_builder {
    typedef std::pair<cgal_point, int> info_point;

    std::vector<vertex *> &vertices;
    graph &g;

    std::set<number_pair> edges;
    delaunay del_graph;

    edge_builder(std::vector<vertex *> &vertices, graph &g) : vertices(vertices), g(g) {
        std::vector<info_point> points;
        points.reserve(vertices.size());
        for(vertex *v : vertices) {
            points.emplace_back(cgal_point(v->x, v->y), v->id);
        }
        del_graph.insert(points.begin(), points.end());
    }

    void add_edge(int u, int v) {
        if(u < v)
            edges.emplace(u, v);
        else if(u > v)
            edges.emplace(v, u);
    }

    edge_builder &add_delaunay_edges() {
        delaunay::Finite_vertices_iterator v;
        delaunay::Vertex_circulator u, end;
        for(v = del_graph.finite_vertices_begin(); v != del_graph.finite_vertices_end(); v++) {
            u = v->incident_vertices();
            end = u;
            if(u == nullptr) {
                continue;
            }
            do {
                if(del_graph.is_infinite(u))
                    continue;
                add_edge(v->info(), u->info());
            } while(++u != end);
        }
        return *this;
    }

    edge_builder &add_nearest_edges(unsigned int amount) {
        delaunay::Finite_vertices_iterator v;
        for(v = del_graph.finite_vertices_begin(); v != del_graph.finite_vertices_end(); v++) {
            std::vector<vertex_handle> neighbors;
            del_graph.nearest_neighbors(v, 1 + amount, back_inserter(neighbors));
            for(auto &u : neighbors) {
                add_edge(v->info(), u->info());
            }
        }
        return *this;
    }

    void add_edges() {
        for(number_pair pair: edges) {
            g.add_edge(pair.first, pair.second);
        }
    }

};

std::pair<float_number, float_number> axis_limits(std::vector<point> &points, bool is_x) {
    float_number min_value = std::numeric_limits<float_number>::max();
    float_number max_value = std::numeric_limits<float_number>::min();
    float_number crr_value;
    for(auto &p : points) {
        crr_value = is_x ? p.x : p.y;
        min_value = std::min(min_value, crr_value);
        max_value = std::max(max_value, crr_value);
    }
    return {min_value, max_value};
}

struct graph_builder {
    std::vector<point> points;
    std::vector<number> box_ids;

    float_number min_x, max_x, delta_x;
    float_number min_y, max_y, delta_y;
    number k;
    u_long size;

    explicit graph_builder(std::vector<point> points) {
        this->points = std::move(points);
        this->size = this->points.size();
        this->box_ids.resize(this->size);
    }

    void set_limits() {
        k = std::ceil(std::pow(points.size(), 0.25));
        std::tie(min_x, max_x) = axis_limits(points, true);
        std::tie(min_y, max_y) = axis_limits(points, false);
        delta_x = (max_x - min_x) / k;
        delta_y = (max_y - min_y) / k;
    }

    void set_box_ids() {

        for(auto &p : points) {
            int i = std::floor((p.x - min_x) / delta_x);
            i = i >= k ? k - 1 : i;
            int j = std::floor((p.y - min_y) / delta_y);
            j = j >= k ? k - 1 : j;
            box_ids[p.id] = i * k + j;
        }
    };

    void add_vertices(graph &g) {
        for(auto &p : points) {
            g.vertices.emplace_back(p.id, box_ids[p.id], p.x, p.y);
        }
    }

    void add_boxes(graph &g) {
        std::vector<box> boxes(k * k);
        for(int i = 0; i < k * k; i++)
            boxes[i].id = i;
        for(vertex &v : g.vertices) {
            boxes[v.box_id].vertices.push_back(&v);
        }
        std::copy_if(boxes.begin(), boxes.end(), std::back_inserter(g.boxes),
                     [](box &b) { return !b.vertices.empty(); });
    }

    static std::vector<vertex *> create_convex_hull(std::vector<vertex *> &vertices, graph &g) {
        std::vector<vertex *> result;
        if(vertices.size() == 1) {
            result = vertices;
            return result;
        }
        auto builder = edge_builder(vertices, g);
        auto &del_graph = builder.del_graph;
        auto vc = del_graph.incident_vertices(del_graph.infinite_vertex());
        auto done = vc;
        if(vc != 0) {
            do {
                result.push_back(&g.vertices[vc->info()]);
            } while(++vc != done);
        }
        return result;
    }

    static void add_convex_hulls(graph &g) {
        for(box &b : g.boxes) {
            b.convex_hull = create_convex_hull(b.vertices, g);
        }
    }

    static void add_costs(box &a, box &b) {
        number min_cost = std::numeric_limits<number>::max();
        number crr_cost;
        vertex *min_a, *min_b;
        for(vertex *v_a : a.convex_hull) {
            for(vertex *v_b : b.convex_hull) {
                crr_cost = distance(v_a, v_b);
                if(crr_cost < min_cost) {
                    min_cost = crr_cost;
                    min_a = v_a;
                    min_b = v_b;
                }
            }
        }
        a.costs[b.id] = {min_cost, min_a};
        b.costs[a.id] = {min_cost, min_b};
    }

    static void add_costs(graph &g) {
        int max_i = g.boxes.size();
        for(int i = 0; i < max_i; i++) {
            for(int j = i + 1; j < max_i; j++) {
                add_costs(g.boxes[i], g.boxes[j]);
            }
        }
    }

    static void add_edges(graph &g) {
        for(box &b : g.boxes) {
            edge_builder(b.vertices, g).add_nearest_edges(NEAREST)
                                       .add_delaunay_edges()
                                       .add_edges();
        }
    }

    graph build() {
        graph g;

        set_limits();
        set_box_ids();
        add_vertices(g);
        add_boxes(g);
        add_convex_hulls(g);
        add_costs(g);
        add_edges(g);
        return g;
    }
};

std::vector<box *> farthest_insertion(std::vector<box *> &boxes) {
    typedef std::pair<number, box *> item;
    std::vector<box *> result;
    std::list<box *> cycle;
    if(boxes.size() <= 2) {
        std::copy(boxes.begin(), boxes.end(), std::back_inserter(result));
        return result;
    }
    cycle.push_back(boxes[0]);
    cycle.push_back(boxes[1]);
    std::priority_queue<item> queue;
    for(box *b : boxes) {
        if(b == boxes[0] || b == boxes[1])
            continue;
        number max_cost = std::numeric_limits<number>::min();
        for(box *r : cycle) {
            number crr_cost = distance(r, b);
            if(crr_cost > max_cost)
                max_cost = crr_cost;
        }
        queue.emplace(max_cost, b);
        b->counter++;
    }

    box *b;
    while(!queue.empty()) {
        b = queue.top().second;
        queue.pop();
        b->counter--;
        if(b->counter >= 1)
            continue;


        auto i = cycle.begin();
        auto j = cycle.begin();
        j++;
        auto min_i = i;
        number min_cost = std::numeric_limits<number>::max();
        while(i != cycle.end()) {
            box *x = *i;
            box *y = *j;
            number crr_cost = distance(x, b) + distance(b, y) - distance(x, y);
            if(crr_cost < min_cost) {
                min_cost = crr_cost;
                min_i = i;
            }
            i++;
            j++;
            if(j == cycle.end())
                j = cycle.begin();
        }
        cycle.insert(++min_i, b);
        for(box *x : boxes) {
            if(x->counter == 0)
                continue;
            x->counter++;
            queue.emplace(distance(b, x), x);
        }
    }
    std::copy(cycle.begin(), cycle.end(), std::back_inserter(result));
    return result;
}


std::vector<box *> farthest_insertion(std::vector<box> &boxes) {
    std::vector<box *> pointers;
    pointers.reserve(boxes.size());
    for(box &b : boxes)
        pointers.push_back(&b);
    return farthest_insertion(pointers);
}

void set_vertices_path(std::vector<box *> boxes) {
    if(boxes.size() == 1)
        return;
    int size = boxes.size();
    for(int i = 0; i < size; i++) {
        box *a = boxes[i];
        box *b = boxes[(i + 1) % size];
        a->get_link(b)->is_ending = true;
        b->get_link(a)->is_ending = true;
    }
}

number e_distance(vertex *v, vertex *u) {
    if(v->is_ending && u->is_ending)
        return -1;
    return distance(v, u);
}

std::vector<vertex *> farthest_insertion(std::vector<vertex *> &vertices, vertex *start, vertex *end) {
    typedef std::pair<number, vertex *> item;
    std::vector<vertex *> result;
    std::list<vertex *> cycle;
    if(vertices.size() <= 2) {
        std::copy(vertices.begin(), vertices.end(), std::back_inserter(result));
        return result;
    }
    cycle.push_back(start);
    cycle.push_back(end);
    std::priority_queue<item> queue;
    for(vertex *v : vertices) {
        if(v == start || v == end)
            continue;
        number max_cost = std::numeric_limits<number>::min();
        for(vertex *r : cycle) {
            number crr_cost = e_distance(r, v);
            if(crr_cost > max_cost)
                max_cost = crr_cost;
        }
        queue.emplace(max_cost, v);
        v->counter++;
    }

    vertex *v;
    while(!queue.empty()) {
        v = queue.top().second;
        queue.pop();
        v->counter--;
        if(v->counter >= 1)
            continue;


        auto i = cycle.begin();
        auto j = cycle.begin();
        j++;
        auto min_i = i;
        number min_cost = std::numeric_limits<number>::max();
        while(i != cycle.end()) {
            vertex *x = *i;
            vertex *y = *j;
            number crr_cost = e_distance(x, v) + e_distance(v, y) - e_distance(x, y);
            if(crr_cost < min_cost) {
                min_cost = crr_cost;
                min_i = i;
            }
            i++;
            j++;
            if(j == cycle.end())
                j = cycle.begin();
        }
        cycle.insert(++min_i, v);
        for(edge *e : v->edges) {
            vertex *u = e->adj_vertex(v);
            if(u->counter == 0)
                continue;
            u->counter++;
            queue.emplace(e_distance(v, u), u);
        }
    }

    std::copy(cycle.begin(), cycle.end(), std::back_inserter(result));
    return result;
}

std::vector<vertex *> get_solution(std::vector<box *> &boxes) {
    std::vector<vertex *> result;
    int size = boxes.size();
    for(int i = 0; i < size; i++) {
        box *prev = boxes[(i - 1 + size) % size];
        box *crr = boxes[i];
        box *next = boxes[(i + 1) % size];
        vertex *start = crr->get_link(prev);
        vertex *end = crr->get_link(next);
        auto sub_cycle = farthest_insertion(crr->vertices, start, end);
        result.insert(result.end(), sub_cycle.begin(), sub_cycle.end());
    }
    return result;
}


std::vector<vertex *> remove_duplicates(std::vector<vertex *> &vertices) {
    std::vector<vertex *> result;

    for(vertex *v : vertices) {
        if(!v->used)
            result.push_back(v);
        v->used = true;
    }

    return result;
}

number calc_length(std::vector<vertex *> &vertices) {
    int size = vertices.size();
    number length = 0;
    for(int i = 0; i < size; i++) {
        length += distance(vertices[i], vertices[(i + 1) % size]);
    }
    return length;
}

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

    explicit splay_tree(std::vector<vertex *> &vertices) : size(vertices.size()) {
        nodes.reserve(size);
        for(int i = 0; i < size; i++) {
            nodes.emplace_back(vertices[i], size - i, nullptr);
            vertices[i]->splay_id = i;
        }
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
        node *n = &nodes[v->splay_id];
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

    explicit tour(std::vector<vertex *> &vertices) : tree(vertices) {}

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

    explicit lk_optimizer(std::vector<vertex *> &vertices) : t(vertices) {
        for(vertex *v : vertices)
            this->vertices.push_back(v);
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
            vertex *mid = e->adj_vertex(last);
            if(mid->counter == 0)
                continue;
            mid->counter--;
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
        bool optimized = true;
        int size = vertices.size();
        for(int x = 0; x < size * SIZE_C && optimized; x++) {
            optimized = false;
            int max_i = vertices.size();
            for(int i = 0; i < max_i && !optimized; i++) {
                optimized = simulation(rounds) || optimized;
                t.shift_elements();
                clean_edges();
            }
        }
    }

    std::vector<vertex *> get_solution() {
        auto path = t.tree.get_sorted_elements();
        return path;
    }
};

std::vector<point> read_points() {
    int n;
    float_number x, y;
    std::vector<point> points;
    std::cin >> n;
    for(int i = 0; i < n; i++) {
        std::cin >> x >> y;
        points.emplace_back(i, x, y);
    }
    return points;
}

void prepare_graph(graph &g) {
    std::vector<vertex *> pointers;
    for(vertex &v : g.vertices) {
        v.is_ending = false;
        pointers.push_back(&v);
        v.edges.clear();
        v.counter = OPT_COUNTER;
    }
    g.clean_edges();
    g.boxes.clear();
    edge_builder(pointers, g).add_delaunay_edges()
                             .add_nearest_edges(NEAREST)
                             .add_edges();
}

void print_solution(std::vector<vertex *> &vertices) {
    number length = calc_length(vertices);
    for(vertex *v : vertices)
        std::cout << v->id << " ";
    std::cout << "\n" << length << "\n";
}

int main() {
    int tasks;
    std::cin >> tasks;

    while(tasks--) {
        graph g = graph_builder(read_points()).build();
        auto boxes = farthest_insertion(g.boxes);
        set_vertices_path(boxes);
        auto vertices = get_solution(boxes);
        vertices = remove_duplicates(vertices);
        prepare_graph(g);
        auto optimizer = lk_optimizer(vertices);
        optimizer.solve(ROUNDS);
        auto solution = optimizer.get_solution();
        print_solution(solution);
    }
}