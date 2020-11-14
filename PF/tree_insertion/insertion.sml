datatype 'a node = Two of 'a node * 'a * 'a node |
                 Three of 'a node * 'a * 'a node * 'a * 'a node |
                 Empty;

datatype 'a insert_result = Ok of 'a node | Push of 'a * 'a node * 'a node;

fun 
  min _ [] = raise Domain |
  min cmp (x::xs) = foldl (fn (a, b) => if cmp(a, b) <= 0 then a else b) x (x::xs) 

fun 
  max _ [] = raise Domain |
  max cmp (x::xs) = foldl (fn (a, b) => if cmp(a, b) >= 0 then a else b) x (x::xs)

fun median cmp x y z = 
  if cmp(x, y) > 0 andalso cmp(x, z) > 0 then
    max cmp [y, z]
  else if cmp(x, y) < 0 andalso cmp(x, z) < 0 then
    min cmp [y, z]
  else
    x



fun
  insert_helper (_, value, Empty) = Ok (Two(Empty, value, Empty)) |
  insert_helper (cmp, value, Two(Empty, x, Empty)) = let
    val a = min cmp [x, value]
    val b = max cmp [x, value]
  in
    Ok (Three(Empty, a, Empty, b, Empty))
  end |
  insert_helper (cmp, value, Three(Empty, x, Empty, y, Empty)) = let
    val all = [x, y, value];
    val a = min cmp all;
    val b = median cmp x y value;
    val c = max cmp all;
  in
    Push (b, Two(Empty, a, Empty), Two(Empty, c, Empty))
  end |
  insert_helper (cmp, value, Two(left, x, right)) = let
    val index = if cmp(value, x) <= 0 then 0 else 1;
    val next_node = if index = 0 then left else right;
    val result = insert_helper(cmp, value, next_node);
  in
    case result of
      Ok new_node => Ok(if index = 0 then Two(new_node, x, right) else Two(left, x, new_node))
    | Push (new_value, new_left, new_right) => let
        val a = min cmp [x, new_value];
        val b = max cmp [x, new_value];
      in
        Ok(if index = 0 then Three(new_left, a, new_right, b, right) else Three(left, a, new_left, b, new_right))
      end
  end |
  insert_helper (cmp, value, Three(left, x, middle, y, right)) = let
    val index = if cmp(value, x) <= 0 then 0 else if cmp(value, y) <= 0 then 1 else 2;
    val next_node = if index = 0 then left else if index = 1 then middle else right
    val result = insert_helper(cmp, value, next_node);
  in
    case result of
      Ok new_node => 
        let
          val new_tree = if index = 0 then
            Three(new_node, x, middle, y, right)
          else if index = 1 then
            Three(left, x, new_node, y, right)
          else
            Three(left, x, middle, y, new_node)
        in
          Ok new_tree
        end
    | Push (new_value, new_left, new_right) => 
        if index = 0 then
          Push (x, Two(new_left, new_value, new_right), Two(middle, y, right))
        else if index = 1 then
          Push (new_value, Two(left, x, new_left), Two(new_right, y, right))
        else
          Push (y, Two(left, x, middle), Two(new_left, new_value, new_right))
  end

fun insert cmp (value, tree) = let
  val result = insert_helper(cmp, value, tree)
in
  case result of
    Ok tree => tree 
  | Push (value, left, right) => Two(left, value, right)
end