structure Queue =
struct
  exception Empty;
  type 'a Queue = 'a list * 'a list;


  val empty = (nil, nil);

  fun 
  push x (tail, []) = ([x], (rev tail)) |
  push x (tail, head) = (x::tail, head);

  fun 
  pop ([], []) = raise Empty |
  pop (tail, []) = pop ([], rev tail) |
  pop (tail, h::hs) = (h, (tail, hs));
end


datatype 'a Tree = 
Leaf | 
Branch of 'a Tree * 'a * 'a Tree


fun 
bfs result (nil, nil) = result |
bfs result queue =
  let
    val (node, queue) = Queue.pop queue;
    val push_many = foldl (fn (x, q) => Queue.push x q)
  in
    case node of
      Leaf => bfs (Leaf::result) queue 
    | Branch (left, x, right) => bfs (node::result) (push_many queue [left, right])
  end

fun 
helper queue [] _ =
  let
    val (result, _) = Queue.pop queue;
  in
    result 
  end |
helper queue (Leaf::nodes) i = helper (Queue.push Leaf queue) nodes i |
helper queue ((Branch(_, x, _))::nodes) i = let
  val (right, queue_1) = Queue.pop queue;
  val (left, queue_2) = Queue.pop queue_1;
  val new_node = Branch (left, (x, i), right);
in
  helper (Queue.push new_node queue_2) nodes (i - 1)
end

fun bfnum tree =
  let
    val bfs_result = bfs [] (Queue.push tree Queue.empty);
    val n_nodes = length (List.filter (fn x => x <> Leaf) bfs_result)
  in
    helper Queue.empty bfs_result (n_nodes - 1)
  end

(* fun give_tree () = (
Branch(
  Branch(
    Branch(
      Leaf,
    10,
      Leaf),
  20,
    Branch(
      Leaf,
    30,
      Leaf)),
40,
  Branch(
    Branch(
      Leaf,
    50,
      Leaf),
  60,
    Branch(
      Leaf,
        70,
      Leaf)))); *)