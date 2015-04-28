type(gn1,not1).
in(gn1,[xin1]).
out(gn1,gn1o).

type(gn2,not1).
in(gn2,[xin2]).
out(gn2,gn2o).

type(ga1,and1).
connected(gn2o,gn2oi).
in(ga1,[xin1,gn2oi]).
out(ga1,ga1o).

type(ga2,and1).
connected(gn1o,gn1oi).
in(ga2,[xin2,gn1o]).
out(ga2,ga2o).

type(go1,or1).
connected(ga2o,ga2oi).
connected(ga1o,ga1oi).
in(go1,[ga2oi,ga1oi]).
out(go1,go1o).


circuitbroken(xor,A):-isbroken(gn1,A).
circuitbroken(xor,A):-isbroken(gn2,A).
circuitbroken(xor,A):-isbroken(ga1,A).
circuitbroken(xor,A):-isbroken(ga2,A).
circuitbroken(xor,A):-isbroken(go1,A).



verify(xor,X):-signal(go1o,X),!.
