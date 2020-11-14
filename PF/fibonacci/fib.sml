type 'a matrix = 'a * 'a * 'a * 'a;
type the_matrix = IntInf.int matrix;

fun mul_matrix((x1, x2, x3, x4): the_matrix) ((y1, y2, y3, y4): the_matrix) : the_matrix = let
   val z1 = x1*y1 + x2*y3
   val z2 = x1*y2 + x2*y4
   val z3 = x3*y1 + x4*y3
   val z4 = x3*y2 + x4*y4
in
   (z1, z2, z3, z4)
end

fun to_large ((x1, x2, x3, x4): int matrix): the_matrix = let
  val y1 = Int.toLarge x1
  val y2 = Int.toLarge x2
  val y3 = Int.toLarge x3
  val y4 = Int.toLarge x4
in
   (y1, y2, y3, y4)
end


fun 
   fib_helper 0 x (a1, a2, a3, a4): IntInf.int = a4 |
   fib_helper (n: int) (x: the_matrix) (acc:the_matrix): IntInf.int = let
      val new_acc = if(n mod 2 = 1) then mul_matrix acc x else acc
      val new_n = n div 2
      val new_x = mul_matrix x x
   in
      fib_helper new_n new_x new_acc
   end 

fun 
   fib 0 = Int.toLarge 0 |
   fib n = let
      val acc = to_large (1, 0, 1, 0)
      val x = to_large (1, 1, 1, 0)
   in 
      fib_helper n x acc
   end