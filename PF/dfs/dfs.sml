(* **************************************** *)
infixr  3 \>     fun f \> y = f y;
fun get array index = Array.sub(array, index);
fun update array index x = Array.update(array, index, x);
fun from_list list = Array.fromList list;
fun array_foldl f init array = Array.foldl (fn (x, acc) => f x acc) init array;
fun create n v = Array.array(n, v);
fun id x = x;

(* **************************************** *)


fun dfs list = let
  val adj_array = from_list list;
  val visits_array = create (length list) false;
  val ids = List.tabulate(length list, id);

  fun helper v result = let
    val visited = get visits_array v;
    val friends = get adj_array v;
  in
    if visited then 
      result
    else (
      update visits_array v true;
      foldl (fn (u, res) => helper u res) (v::result) friends
    )
  end
in
  rev \> foldl (fn (u, res) => helper u res) [] ids
end