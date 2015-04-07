type(gn1,not1).
in(gn1,[adin1]).
out(gn1,gn1o).

type(gn2,not1).
in(gn2,[adin2]).
out(gn2,gn2o).

type(ga1,and1).
connected(gn2o,gn2oi).
in(ga1,[adin1,gn2oi]).
out(ga1,ga1o).

type(ga2,and1).
connected(gn1o,gn1oi).
in(ga2,[adin2,gn1oi]).
out(ga2,ga2o).

type(go1,or1).
connected(ga2o,ga2oi).
connected(ga1o,ga1oi).
%replace ga2oib with ga2oi
in(go1,[ga2oi,ga1oi]).
out(go1,go1o).





type(gn11,not1).
in(gn11,[adin3]).
out(gn11,gn11o).

%gn110 is not C

type(gn21,not1).
%replace go1oif with go1oi
connected(go1o,go1oi).
in(gn21,[go1oi]).
out(gn21,gn21o).

%gn210 is not of A xor B

type(ga11,and1).
connected(gn21o,gn21oi).
in(ga11,[adin3,gn21oi]).
out(ga11,ga11o).



type(ga21,and1).
connected(gn11o,gn11oi).
connected(go1o,go1oii).
in(ga21,[gn11oi,go1oii]).
out(ga21,ga21o).

type(go11,or1).
connected(ga21o,ga21oi).
connected(ga11o,ga11oi).
in(go11,[ga21oi,ga11oi]).
out(go11,go11o).



%go11o is sum

type(ga3,and1).
connected(go1o,go1oiii).
in(ga3,[adin3,go1oiii]).
out(ga3,ga3o).

type(ga4,and1).
in(ga3,[adin1,adin2]).
out(ga4,ga4o).

type(go3,or1).
connected(ga3o,ga3oi).
connected(ga4o,ga4oi).
in(go3,[ga3oi,ga4oi]).
out(go3,go3o).


circuitbroken(fadder,A):-isbroken(gn1,A).
circuitbroken(fadder,A):-isbroken(gn2,A).
circuitbroken(fadder,A):-isbroken(ga1,A).
circuitbroken(fadder,A):-isbroken(ga2,A).
circuitbroken(fadder,A):-isbroken(go1,A).

circuitbroken(fadder,A):-isbroken(gn11,A).
circuitbroken(fadder,A):-isbroken(gn21,A).
circuitbroken(fadder,A):-isbroken(ga11,A).
circuitbroken(fadder,A):-isbroken(ga21,A).
circuitbroken(fadder,A):-isbroken(go11,A).

circuitbroken(fadder,A):-isbroken(ga3,A).
circuitbroken(fadder,A):-isbroken(ga4,A).
circuitbroken(fadder,A):-isbroken(go3,A).

verify(fadder,X,Y):-signal(go11o,X),signal(go3o,Y).
