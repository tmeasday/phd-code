function [y, cost] = PAST(Advice)

%% function [y,cost] = PAST(Advice)
%
% arguments:    
% -- Advice, a sparse upper triangular matrix where 
%    Advice(i,j) = 0            for i >= j
%    Advice(i,j) \in {-1, 0, 1} for i <  j
% 
% returns:
% -- y, a clustering
% -- cost, the 2CC cost of y
%
% algorithm: 
% -- find the BFS spanning tree from each vertex. 
% -- run PAST_single on that tree.
% -- of these, return the cheapest clustering

%% 
n = size(Advice,1);
cost = Inf;

for i=1:n
   i
   Tree      = breadth_first_st(Advice, i);
   y_curr    = PAST_single (Advice, Tree);
   cost_curr = CCcost(y_curr, Advice);
   if cost_curr < cost
       cost = cost_curr;
       y    =    y_curr;
   end
end