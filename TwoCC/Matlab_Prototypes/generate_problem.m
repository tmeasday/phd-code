function [Advice, y_orig] = generate_problem(n, k, p_edge, p_flip)

%% function: [Advice, y_orig] = generate_problem(n, k, p_edge, p_flip)
%
% arguments: 
% -- n, the number of vertices
% -- k, the size of the first cluster
% -- p_edge, probability of keeping an edge in the graph.
% -- p_flip, probability of flipping the label on an edge.
%
% returns:   
% -- Advice, a sparse upper triangular matrix where 
%    Advice(i,j) = 0            for i >= j
%    Advice(i,j) \in {-1, 0, 1} for i <  j
% -- y_orig, the "true" clustering.

%% generate an underlying graph
G = sparse (triu (rand(n) < p_edge, 1));

%% generate a problem instance on this graph
[Advice, y_orig] = generate_advice(G, k, p_flip);