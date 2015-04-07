type(gn1,not1).
in(gn1,[adin1]).
out(gn1,gn1o).

type(gn2,not1).
in(gn2,[adin2]).
out(gn2,gn2o).

type(ga1,and1).
in(ga1,[adin1,gn2o]).
out(ga1,ga1o).

type(ga2,and1).
in(ga2,[adin2,gn1o]).
out(ga2,ga2o).

type(go1,or1).
in(go1,[ga2o,ga1o]).
out(go1,go1o).





type(gn11,not1).
in(gn11,[adin3]).
out(gn11,gn11o).

%gn110 is not C

type(gn21,not1).
in(gn21,[go1o]).
out(gn21,gn21o).

%gn210 is not of A xor B

type(ga11,and1).
in(ga11,[adin3,gn21o]).
out(ga11,ga11o).



type(ga21,and1).
in(ga21,[gn11o,go1o]).
out(ga21,ga21o).

type(go11,or1).
in(go11,[ga21o,ga11o]).
out(go11,go11o).



%go11o is sum

type(ga3,and1).
in(ga3,[adin3,go1o]).
out(ga3,ga3o).

type(ga4,and1).
in(ga3,[adin1,adin2]).
out(ga4,ga4o).

type(go3,or1).
in(go3,[ga3o,ga4o]).
out(go3,go3o).


verify(fadder,X,Y):-signal(go11o,X),signal(go3o,Y).
