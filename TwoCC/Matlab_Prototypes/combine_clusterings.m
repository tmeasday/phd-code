function y = combine_clusterings(indices, ys)

%% function: y = combine_clusterings(indices, ys)
%
% arguments: 
% -- indices, a cell array with indices{i} being the vertices of the Advice
%    graph involved in the ith "block". (The second value returned by the
%    function to_blocks.)
% -- ys, a cell array with ys{i} being a clustering of the vertices in the 
%    ith "block" of the Advice graph. (Each element of ys{i} is in {-1,1}.)
%
% returns:   
% -- y, an array with elements in {-1,0,1} where
%    y(i,j) = 0 if vertex i is not in the jth connected component of the
%                  Advice graph.
%    y(i,j) = 1 if vertex i is the vertex of smallest label in the jth
%                  connected component of the Advice graph.
%    y(i,j) = 1 if vertex i is in the same connected component and cluster 
%                  as the vertex of smallest label in the jth connected 
%                  component of the Advice graph.
%    y(i,j) =-1 if vertex i is in the same connected component and not in 
%                  the same cluster as the vertex of smallest label in the 
%                  jth connected component of the Advice graph.    
%
% notes: THIS FUNCTION IS YET TO BE WRITTEN.