function [indices] = to_blocks (Advice)

%% function: [indices] = to_blocks (Advice)
%
% arguments: A sparse upper triangular matrix Advice with 
% -- Advice(i,j) = 0            for i >= j
% -- Advice(i,j) \in {-1, 0, 1} for i <  j
%
% returns:   
% % -- blocks, a cell array with blocks{i} being a square submatrix of Advice
% %    consisting of the ith "block" (i.e. maximal 2-connected subgraph) of 
% %    the Advice graph.
% -- indices, a cell array with indices{i} being the vertices of the Advice
%    graph involved in the ith "block".
%
% algorithm: From K. Paton, "An algorithm for the blocks and cutnodes of a
% graph" Communications of the ACM, July 1971, Vol. 14, No. 7.
%
% 1. Run BFS on the graph
% 2. Keep track of the shortest paths between nodes on the tree
% 3. When find a back edge {i,j}, know that all edges in the 
% shortest path from i to j are in the same block. If none of these edges
% are currently labelled, label them with the "next" block number. If some
% of these edges are already labelled:
%   a. find the smallest block label already on these edges
%   b. label all these edges with that label *and* for every edge that
%   just got *re*-labelled from b1 to b2, relabel all the edges that 
%   were labelled b1 to be labelled b2.
% 4. If BFS terminates by not all vertices have been seen, choose a vx that
% has not been seen an repeat the process.


% number of vertices and edges respectively
n = size(Advice,1);
e = nnz(Advice);

% each column of block_edges will be an indicator vector of edges in the
% block. Each edge starts out in its own block. As we find cycles, we merge
% blocks.
block_edges = speye(e);

% get a list of the edges in the graph in canonical order 
[cols rows] = find(Advice');
edges = [rows cols];

Advice = Advice + Advice';

finished  = repmat(false, n, 1);
searched  = repmat(false, n, 1);
inT = repmat(false, e, 1);


% e x n matrix whose ith column is the indicator vector of the shortest 
% path in the spanning tree from vertex i to the root. The column 
% corresponding to the root is all zeros for simplicity. 
SPs = sparse(e,n);

% run this loop for each connected component
while(any (~searched))
    candidates_to_search = find(~searched);
    to_search = candidates_to_search(1);
    searched(to_search) = true;

    while ~isempty (to_search)
        i = to_search(end);

        new_i    = Advice(:,i) ~= 0;
        new_i_ns = new_i & ~searched;
        new_i_s  = new_i & ~finished & searched;
                
        if any(new_i_ns)
            % indicator for the edges from i to the new unsearched vertices
            i2new_i_ns_edges = ( (edges(:,1) == i) & (new_i_ns(edges(:,2))) ) | ...
                               ( (edges(:,2) == i) & (new_i_ns(edges(:,1))) );
            SPs_last_term = sparse(e, sum(new_i_ns));
            SPs_last_term(i2new_i_ns_edges,:) = speye(full(sum(new_i_ns)));
            % find the shortest path from the unsearched neighbours of i to the root
            SPs(:,new_i_ns) = mod(repmat(SPs(:,i), 1, sum(new_i_ns)) + ...
                    SPs_last_term, 2);
                
            inT = inT | i2new_i_ns_edges;
            searched(new_i_ns) = true;
        end
   
        if any(new_i_s)
         
            % indicator vectors of the blocks formed by the newly found
            % fundamental cycles. 
            new_block = mod(repmat(SPs(:,i), 1, sum(new_i_s)) + ...
                  SPs(:,new_i_s), 2);
            % note that all back-edges from a particular vertex must be 
            % part of the same block becasue they all share the edge from the 
            % vertex to its parent in the spanning tree.
            % Thus take the logical OR along each row.
            new_block = sum(new_block,2) > 0;
              
            % find where this new block intersects with the
            % old blocks. This is always non-empty as each edge is
            % initially in its own block.
            match = any (repmat(new_block, 1, size(block_edges, 2)) & block_edges);
            
            % take the logical OR of the blocks that inersected with
            % the new one and throw away all the constituent blocks.
            super_block = sum(block_edges(:,match), 2) + new_block > 0;
            block_edges = [block_edges(:,~match), super_block];
        end    
        to_search = [find(new_i_ns); to_search(1:end-1)];
        finished(i) = true; 
    end
end

%% convert from block_edges to indices for each block.
% first banish the "blocks" consisting only of edges that are not in the
% tree.
useful_blocks = any (block_edges(inT,:));
block_edges = block_edges(:,useful_blocks);
singletons = find(all(Advice == 0));

indices = cell(1,size(block_edges,2) + length(singletons));
for i=1:size(block_edges,2)
    indices{i} = union(edges(block_edges(:,i)>0,1)', edges(block_edges(:,i)>0,2)');
end

% this whole process should have "missed" singleton vertices. Add these on
% the end.

for i=1:length(singletons)
    indices{i + size(block_edges,2)} = singletons(i);
end

