function [y, cost] = PASTA_flip_random(Advice, m)

%% function [y,cost] = PASTA_flip_random(Advice, m)
%
% arguments:    
% -- Advice, a sparse upper triangular matrix where 
%    Advice(i,j) = 0            for i >= j
%    Advice(i,j) \in {-1, 0, 1} for i <  j
%    NOTE: we assume that advice forms a 2-connected graph.
% -- m, the number of spanning trees to use
% 
% returns:
% -- y, a clustering
% -- cost, the 2CC cost of y
%
% algorithm: 
% -- choose n random spannings trees
% -- run PASTA_flip_single_new on each tree.
% -- of these, return the cheapest clustering

%% 
if nargin == 1
    m = 1;
end

n = size(Advice,1);
cost = Inf;

% now we can assume that every edge of the input is involved in a cycle.
for i=1:m
    i
   [H, Tree, inT] = random_cycle_basis (Advice);
   y_curr    = PASTA_flip_single_new (Advice, H, Tree, inT);
   cost_curr = CCcost(y_curr, Advice);
   if cost_curr < cost
       cost = cost_curr;
       y    =    y_curr;
   end
end
    
    