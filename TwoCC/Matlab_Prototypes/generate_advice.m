function [Advice, y_orig] = generate_advice(G, k, p)

%% function: [Advice, y_orig] = generate_advice(G, k, p)
%
% arguments: 
% -- A sparse adjacency matrix for a graph G where
%    G(i,j) = 0        for i >= j
%    G(i,j) \in {0, 1} for i <  j
% -- p, probability of flipping the label on an edge.
% -- k, the size of the first cluster
%
% returns:   
% -- Advice, a sparse upper triangular matrix where 
%    Advice(i,j) = 0            for i >= j
%    Advice(i,j) \in {-1, 0, 1} for i <  j
% -- y_orig, the "true" clustering.

%% cluster the vertices of G with cluster "1" of size k.
n      = size (G,2);
y_orig = [ones(k,1); -ones(n-k,1)];
y_orig = y_orig(1) * y_orig;

%% turn it into a labelling on the edges of G
orig_labelling  = G .* (y_orig * y_orig');

%% add "noise" to the labelling
noise  = G .* (1 - 2*(rand (n) < p)); 
Advice = orig_labelling .* noise; 
