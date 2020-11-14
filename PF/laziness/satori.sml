(* Strumień stały (x, x, x, ...) w pamięci O(1) *)
(* val sconst = fn : 'a -> 'a stream *)
fun sconst x = smemo (fn y => (x, y))


(* N-ty element strumienia, numerowanie od 0 *)
(* val snth = fn : int -> 'a stream -> 'a *)
fun 
snth 0 s = shd s |
snth n s = snth (n-1) (stl s)

(* Lista pierwszych N elementów strumienia *)
(* val stake = fn : int -> 'a stream -> 'a list *)
fun 
stake 0 s = [] |
stake n s = (shd s)::(stake (n-1) (stl s))

(* 'smap f s' to strumień zmapowany przez f:
    [f s_0, f s_1, f s_2, ...] *)
(* val smap = fn : ('a -> 'b) -> 'a stream -> 'b stream *)
fun smap f s = smemo (fn _ => 
let 
  val (x, xs)= seval s
in
  (f x, smap f xs)
end)

(* 'smap1 f s' to strumień ze zmapowanym pierwszym elementem:
    [f s_0, s_1, s_2, ...] *)
(* val smap1 = fn : ('a -> 'a) -> 'a stream -> 'a stream *)
fun smap1 f s = smemo (fn _ => 
let
  val (x, xs) = seval s
in
  (f x, xs)
end)

(* 'snat s z' to strumień [z, s z, s (s z), s (s (s z)), ...] *)
(* val snat = fn : ('a -> 'a) -> 'a -> 'a stream *)
fun snat s x = smemo (fn _ => (x, snat s (s x)))

(* 'stab f' to strumień [f 0, f 1, f 2, ...] *)
(* val stab = fn : (int -> 'a) -> 'a stream *)
fun stab f = smap f (snat (fn x => x + 1) 0)

(* 'szip s t' to strumień [(s_0, t_0), (s_1, t_1), ...] *)
(* val szip = fn : 'a stream -> 'b stream -> ('a * 'b) stream *)
fun szip x_stream y_stream = smemo (fn _ =>
let
  val (x, xs) = seval x_stream;
  val (y, ys) = seval y_stream;
in
  ((x, y), szip xs ys)
end)

(* 'szipwith f s t' to strumień [f (s_0, t_0), f (s_1, t_1), ...] *)
(* val szipwith = fn : ('a * 'b -> 'c) -> 'a stream -> 'b stream -> 'c stream *)
fun szipwith f x_stream y_stream = smap f (szip x_stream y_stream)

fun sadd x stream = smemo (fn _ => (x, stream))

(* 'sfoldl f start s' to strumień:
  [start, f(start, s_0), f(f(start, s_0), s_1), f(f(f(start, s_0), s_1), s_2), ...] *)
(* val sfoldl = fn : ('a * 'b -> 'a) -> 'a -> 'b stream -> 'a stream *)
fun sfoldl f a x_stream = let
  fun sfoldl' b y_stream = smemo (fn _ => let
    val (y, ys) = seval y_stream;
    val result = f(b, y);
  in
    (result, sfoldl' result ys)
  end)
in
  smemo (fn _ => (a, sfoldl' a x_stream))
end

(* 'srev s' to strumień:
  [[s_0], [s_1, s_0], [s_2, s_1, s_0], ...] *)
(* val srev = fn : 'a stream -> 'a list stream *)
fun srev x_stream = 
let
  fun add(xs, x) = x::xs
in
  stl (sfoldl add [] x_stream)
end

(* 'sfilter p s' to strumień s po usunięciu elementów,
   dla których predykat p jest fałszywy. Jeśli w strumieniu
   s jest skończenie wiele (n) elementów spełniających p,
   to próba odczytania n+1-szego elementu może prowadzić
   do zapętlenia. Proszę pamiętać, że sfilter ma być
   leniwy - tzn. samo wywołanie sfilter nie może skanować
   wejściowego strumienia w poszukiwaniu głowy spełniającej
   predykat. To ma nastąpić dopiero w trakcie czytania zwróconego
   strumienia. *)
(* val sfilter = fn : ('a -> bool) -> 'a stream -> 'a stream *)
fun sfilter p x_stream = smemo (fn _ => 
let
  fun sfilter' y_stream = let
    val (y, ys) = seval y_stream;
    val result = p y
  in
    if result then (y, sfilter p ys) else sfilter' ys
  end
in
  sfilter' x_stream
end)

(* 'stakewhile p s' to lista początkowych elementów strumienia,
   dla których p jest prawdziwy. Jeśli p jest prawdziwy
   dla nieskończenie wielu elementów, to funkcja może nie skończyć
   działania *)
(* val stakewhile = fn : ('a -> bool) -> 'a stream -> 'a list *)
fun stakewhile p x_stream = 
let
  val (x, xs) = seval x_stream;
  val result = p x;
in
  if result then x::(stakewhile p xs) else []
end

(* srepeat tworzy strumień elementów powtarzających się cyklicznie.
   Zakładamy, że podana lista jest niepusta. Odczytanie pierwszych
   k elementów ze strumienia zajmuje czas O(k). Pamięć ograniczona
   jest przez O(n) gdzie n to liczba elementów w danej liście. Programy
   wykorzystujące więcej pamięci mogą dostać RTE lub TLE. *)
(* val srepeat = fn : 'a list -> 'a stream *)
fun 
srepeat [] = raise Domain |
srepeat (x::xs) = smemo (fn builder => 
  let
    fun 
    srepeat' [] = builder |
    srepeat' (y::ys) = smemo (fn _ => (y, srepeat' ys))
  in
    (x, srepeat' xs)
  end)
  
(* spairs s to strumień:
   [(s_0, s_1), (s_2, s_3), (s_4, s_5), ...] *)
(* val spairs = fn : 'a stream -> ('a * 'a) stream *)
fun spairs x_stream = smemo (fn _ => 
let
  fun spairs' a y_stream = let
    val (y, ys) = seval y_stream;
  in
    ((a, y), spairs ys)
  end
  val (x, xs) = seval x_stream;
in
  spairs' x xs
end)


fun 
sskip 0 x_stream = x_stream |
sskip n x_stream = sskip (n-1) (stl x_stream);

fun 
severynth 0 x_stream = x_stream | 
severynth n x_stream = smemo (fn _ => 
let
  val (x, xs) = seval x_stream;
  fun severynth' y_stream = smemo (fn _ => 
  let
    val (y, ys) = seval (sskip (n - 1) y_stream);
  in
    (y, severynth' ys)
  end)
in
  (x, severynth' xs)
end)




(* dzieli strumień s na n strumieni:
   [[s_0, s_n, s_{2n}, s_{3n} ...]
    [s_1, s_{n+1}, s_{2n+1}, ...]
    ...
    [s_(n-1), s_{2n-1}, s_{3n-1}, ...]]
   UWAGA:
    Jeżeli z otrzymanych strumieni odczytujemy łącznie k elementów,
    a największy numer odczytanego elementu w oryginalnym strumieniu
    to l, to czas działania jest ograniczony przez O(n+k+l). Tzn.,
    zwrócone strumienie dzielą pracę i nie przeskakują niezależnie elementów
    w czasie O(nk). *)
(* val ssplitn = fn : int -> 'a stream -> 'a stream list *)
fun 
list_nat 0 f x = [] |
list_nat n f x = x::(list_nat (n - 1) f (f x))

fun ssplitn n x_stream = let
  val indexes = List.tabulate(n, fn x => x);
  val matrix = snat stl x_stream;
  val first_stream = severynth n matrix;
  val streams = list_nat n (smap stl) first_stream
in
  map (smap shd) streams
end

(* Operacja odwrotna do ssplitn. Przeplata dane strumienie tworząc
   wspólny strumień *)
(* val sinterleave = fn : 'a stream list -> 'a stream *)
fun sinterleave stream_list =
let
  fun 
  sinterleave' [] stack = sinterleave' (rev stack) [] |
  sinterleave' (stream::streams) stack = smemo (fn _ => 
    let
      val (x, xs) = seval stream;
    in
      (x, sinterleave' streams (xs::stack))
    end)
in
  sinterleave' stream_list []
end