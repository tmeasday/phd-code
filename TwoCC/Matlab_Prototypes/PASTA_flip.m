function [y, cost] = PASTA_flip(Advice)

%% function [y,cost] = PASTA_flip(Advice)
%
% arguments:    
% -- Advice, a sparse upper triangular matrix where 
%    Advice(i,j) = 0            for i >= j
%    Advice(i,j) \in {-1, 0, 1} for i <  j
%    NOTE: we assume that advice forms a 2-connected graph.
% 
% returns:
% -- y, a clustering
% -- cost, the 2CC cost of y
%
% algorithm: 
% -- find the BFS spanning tree from each vertex. 
% -- run PASTA_flip_single_new on that tree.
% -- of these, return the cheapest clustering

%% 
n = size(Advice,1);
cost = Inf;

if n==2
    % if we are dealing with just a single edge
    y = [1;Advice(1,2)];
    cost = 0;
    return;
end

% now we can assume that every edge of the input is involved in a cycle.

for i=1:n
    i
   [H, Tree, inT] = bfs_cycle_basis (Advice, i);
   y_curr    = PASTA_flip_single_new (Advice, H, Tree, inT);
   cost_curr = CCcost(y_curr, Advice);
   if cost_curr < cost
       cost = cost_curr;
       y    =    y_curr;
   end
end
    
    