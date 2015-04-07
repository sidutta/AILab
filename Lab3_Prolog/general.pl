

%signal(gn1o,0).
%signal(ga1o,0).

signal(X,Y):-connected(Z,X),signal(Z,Y),!.

signal(X,Y):-out(X1,X),type(X1,and1),in(X1,[H1|[H2|_]]),signal(H1,1),signal(H2,1),!,Y=1.
signal(X,Y):-out(X1,X),type(X1,and1),in(X1,[H1|[H2|_]]),signal(H1,0),signal(H2,1),!,Y=0.
signal(X,Y):-out(X1,X),type(X1,and1),in(X1,[H1|[H2|_]]),signal(H1,1),signal(H2,0),!,Y=0.
signal(X,Y):-out(X1,X),type(X1,and1),in(X1,[H1|[H2|_]]),signal(H1,0),signal(H2,0),!,Y=0.
%signal(X,Y):-out(X1,X),type(X1,and1),!,Y=0.

signal(X,Y):-out(X1,X),type(X1,or1),in(X1,[H1|[H2|_]]),signal(H1,0),signal(H2,0),!,Y=0.
signal(X,Y):-out(X1,X),type(X1,or1),in(X1,[H1|[H2|_]]),signal(H1,1),signal(H2,0),!,Y=1.
signal(X,Y):-out(X1,X),type(X1,or1),in(X1,[H1|[H2|_]]),signal(H1,0),signal(H2,1),!,Y=1.
signal(X,Y):-out(X1,X),type(X1,or1),in(X1,[H1|[H2|_]]),signal(H1,1),signal(H2,1),!,Y=1.
%signal(X,Y):-out(X1,X),type(X1,or1),!,Y=1.

signal(X,Y):-out(X1,X),type(X1,not1),in(X1,[H1|_]),signal(H1,1),!,Y=0.
signal(X,Y):-out(X1,X),type(X1,not1),in(X1,[H1|_]),signal(H1,0),!,Y=1.
%signal(X,Y):-out(X1,X),type(X1,not1),!,Y=1.

disconnected(A):-isinp(A),!,false.
disconnected(A):-isout(A),!,false.
disconnected(A):-connected(A,B),!,false.
disconnected(A):-connected(B,A),!,false.
disconnected(A).


%A is gate,B is broken terminal
isbroken(A,B):-type(A,not1),in(A,[B]),disconnected(B).
isbroken(A,B):-type(A,and1),in(A,[B,C]),disconnected(B).
isbroken(A,C):-type(A,and1),in(A,[B,C]),disconnected(C).
isbroken(A,B):-type(A,or1),in(A,[B,C]),disconnected(B).
isbroken(A,C):-type(A,or1),in(A,[B,C]),disconnected(C).
isbroken(A,B):-out(A,[B]),disconnected(B).


isgrounded(A,B):-out(A,B),signal(B,0),!.


signal(xin1,0).
signal(xin2,0).
isinp(xin1).
isinp(xin2).
isout(go1o).

signal(adin1,0).
signal(adin2,0).
signal(adin3,0).
isinp(adin1).
isinp(adin2).
isinp(adin3).
isout(go11o).
isout(go3o).
