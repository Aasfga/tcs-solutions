(* Utility functions *)
fun
  min _ [] = raise Domain |
  min cmp (x::xs) = foldl (fn (a, b) => if cmp(a, b) = LESS then a else b) x (x::xs)

fun
  max _ [] = raise Domain |
  max cmp (x::xs) = foldl (fn (a, b) => if cmp(a, b) = GREATER then a else b) x (x::xs)

fun median cmp x y z =
  if cmp(x, y) = GREATER andalso cmp(x, z) = GREATER then
    max cmp [y, z]
  else if cmp(x, y) = LESS andalso cmp(x, z) = LESS then
    min cmp [y, z]
  else
    x

fun
to_order x = 
	if x < 0 then
		LESS
	else if x = 0 then
		EQUAL
	else
		GREATER
(* ******************** *)


(* 2-3 Tree *)
datatype 'a node = 
	Empty |
	Two of 'a node * 'a * 'a node |
	Three of 'a node * 'a * 'a node * 'a * 'a node;

datatype 'b Propagate = 
	Good of 'b | 
	PropagateUp of 'b;
(* datatype 'a Propagate = 
	Ok of 'a node | 
	Push of 'a * 'a node * 'a node; *)

fun
tree_insert' (cmp, value, Empty) = Good (Two(Empty, value, Empty)) |
tree_insert' (cmp, value, Two(Empty, x, Empty)) = 
	if cmp(value, x) = EQUAL then
		Good (Two(Empty, value, Empty))
	else let
		val a = min cmp [x, value];
		val b = max cmp [x, value];
	in
		Good (Three(Empty, a, Empty, b, Empty))
	end |
tree_insert' (cmp, value, Three(Empty, x, Empty, y, Empty)) =
	if cmp(value, x) = EQUAL then
		Good (Three(Empty, value, Empty, y, Empty))
	else if cmp(value, y) = EQUAL then 
		Good (Three(Empty, x, Empty, value, Empty))
	else let 
	 val all = [x, y, value];
	 val a = min cmp all;
	 val b = median cmp x y value;
	 val c = max cmp all;
	in
		PropagateUp (Two(Two(Empty, a, Empty), b, Two(Empty, c, Empty)))
	end |
tree_insert' (cmp, value, Two(left, x, right)) = 
	if cmp(value, x) = EQUAL then
		Good (Two(left, x, right))
	else let
		val index = if cmp(value, x) = LESS then 0 else 1;
		val result = tree_insert'(cmp, value, if index = 0 then left else right);
	in case result of
		Good new_node => 
			if index = 0 then 
				Good (Two(new_node, x, right))
			else	
				Good (Two(left, x, new_node))
	| PropagateUp (Two(new_left, new_value, new_right)) => 
			let
				val a = min cmp [x, new_value];
				val b = max cmp [x, new_value];
			in
				if index = 0 then
					Good (Three(new_left, a, new_right, b, right))
				else
					Good (Three(left, a, new_left, b, new_right))
			end
	end |
tree_insert' (cmp, value, Three(left, x, middle, y, right)) =
	if cmp(value, x) = EQUAL then
		Good (Three(left, value, middle, y, right))
	else if cmp(value, y) = EQUAL then 
		Good (Three(left, x, middle, value, right))
	else let
		val index = if cmp(value, x) = LESS then 0 else if cmp(value, y) = LESS then 1 else 2;
		val result = tree_insert'(cmp, value, case index of 0 => left |	1 => middle |	_ => right);
	in case result of 
		Good new_node =>
			if index = 0 then
				Good (Three(new_node, x, middle, y, right))
			else if index = 1 then
				Good (Three(left, x, new_node, y, right))
			else
				Good (Three(left, x, middle, y, new_node))
	|	PropagateUp (Two(new_left, new_value, new_right)) =>
			if index = 0 then
				PropagateUp (Two (Two(new_left, new_value, new_right), x, Two(middle, y, right)))
			else if index = 1 then 
				PropagateUp (Two (Two(left, x, new_left), new_value, Two(new_right, y, right)))
			else 
				PropagateUp (Two (Two(left, x, middle), y, Two(new_left, new_value, new_right)))
	end

fun 
tree_insert (cmp, value, tree) = let
  val result = tree_insert'(cmp, value, tree)
in
  case result of
    Good tree => tree
  | PropagateUp (Two(left, value, right)) => Two(left, value, right)
end

fun 
tree_lookup (cmp, value, Empty) = NONE |
tree_lookup (cmp, value, Two (left, x, right)) =
  let 
		val compare = cmp(value, x);
  in
		if compare = LESS then
			tree_lookup(cmp, value, left)
		else if compare = GREATER then
			tree_lookup(cmp, value, right)
		else 
			SOME x
	end |
tree_lookup (cmp, value, Three (left, x, middle, y, right)) =
	let 
		val compare_1 = cmp(value, x);
		val compare_2 = cmp(value, y);
	in
		if compare_1 = LESS then
			tree_lookup(cmp, value, left)
		else if compare_1 = EQUAL then
			SOME x
		else if compare_2 = LESS then
			tree_lookup(cmp, value, middle)
		else if compare_2 = EQUAL then
			SOME y
		else
			tree_lookup(cmp, value, right)
	end

functor TFrame (
 	structure Spec:SPEC
	) = 
struct
	type 'vt frame = 'vt Spec.entryT node;

	val key_cmp = Spec.Key.cmp;
	val extract_key = Spec.extractKey;
	val update_entry = Spec.updateE;
	val lookup_entry = Spec.lookupE;

	val empty = Empty;
	fun lookup (key,  tree) = 
		let
			fun compare (k, e) = key_cmp(k, extract_key e);
		in
			lookup_entry (tree_lookup(compare, key, tree))
		end
	fun insert (entry,  tree) = 
		let
			fun cmp(x, y) = key_cmp(extract_key x, extract_key y);
		in
			tree_insert(cmp, entry, tree)
		end
end

functor DSpec (
	structure KeyS:COMPARABLE
	) : SPEC = 
struct
	structure Key = KeyS;
	type 'vT entryT = Key.t * 'vT;
	type 'vT resultT = 'vT option;
	
	
	fun extractKey (k, _) = k;

	(* By default tree is replacing existing elements *)
	fun updateE _ = raise Domain;

	fun
		lookupE NONE = NONE |
		lookupE (SOME(k, v)) = SOME v;
end

functor SSpec (
      structure KeyS : COMPARABLE
) : SPEC =
struct
	structure Key = KeyS;

	type 'vT entryT = Key.t;
	type 'vT resultT = bool;

	fun extractKey x = x;

	(* By default tree is replacing existing elements *)
	fun updateE _ = raise Domain;

	fun
	lookupE NONE = false |
	lookupE (SOME _) = true
end