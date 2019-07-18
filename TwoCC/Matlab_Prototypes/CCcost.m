function cost = CCcost(Y, Advice)

%% function cost = CCcost(Y, Advice)
%
% arguments:
% -- Y, a matrix whose columns are clusterings 
% -- Advice, a sparse upper triangular matrix where 
%    Advice(i,j) = 0            for i >= j
%    Advice(i,j) \in {-1, 0, 1} for i <  j
% 
% returns:
% -- cost, a vector containing the correlation clustering costs of the 
%    columns of y

%% form M from Advice
Advice = Advice + Advice';
d = sum (abs (Advice), 2);
M = diag (d) - Advice;

%% find the cost
cost = diag (0.25 * Y' * M * Y);
