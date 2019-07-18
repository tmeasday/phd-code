function [H, Tree, inT] = bfs_cycle_basis (Advice, root)

%% function H = bfs_cycle_basis (Advice, root)
% always assume that the input is 2-connected, and is not just a single
% edge (we rule this case out earlier).

% number of vertices and edges respectively
n = size(Advice,1);
e = nnz(Advice);
Tree   = sparse(n,n);

% get a list of the edges in the graph in canonical order 
[cols rows] = find(Advice');
edges = [rows cols];
edge_nos = sparse (n,n);
edge_nos (sub2ind ([n,n], edges(:,1), edges(:,2))) = 1:e;

Advice = Advice + Advice';
edge_nos = edge_nos + edge_nos';

to_search = root;
finished  = repmat(false, n, 1);
searched  = repmat(false, n, 1);
searched(root) = true;
inT = repmat(false, e, 1);

% e x n matrix whose ith column is the indicator vector of the shortest 
% path in the spanning tree from vertex i to the root. The column 
% corresponding to the root is all zeros for simplicity. 
SPs = sparse(e,n);

% e x (e - n + 1) matrix whose ith column is the indicator vector of the
% ith fundamental cycle found in the process of the BFS. This is what the
% function ultimately returns.
H = sparse(e,e-n+1);
counter = 1;

while ~isempty (to_search)
   i = to_search(end);

   new_i    = Advice(:,i) ~= 0;
   new_i_ns = new_i & ~searched;
   num_ns = sum(new_i_ns);
   new_i_s  = new_i & ~finished & searched;
   num_s = sum(new_i_s);
   
   if any(new_i_ns)
        % indicator for the edges from i to the new unsearched vertices
        SPs_last_term = sparse(e, num_ns);
        SPs_last_term(edge_nos(new_i_ns, i),:) = speye(full(num_ns));
        % find the shortest path from the unsearched neighbours of i to the root
        SPs(:,new_i_ns) = mod(repmat(SPs(:,i), 1, num_ns) + ...
                SPs_last_term, 2);       
        Tree (i, new_i_ns) = 1;
        inT (edge_nos(new_i_ns,i)) = true;

        searched(new_i_ns) = true;
   end
   
   if any(new_i_s)
        H_last_term = sparse(e, num_s);
        H_last_term(edge_nos(new_i_s, i),:) = speye(full(num_s));
         
        % find the new cycles formed by the edges from i to the searched
        % neighbours of i.
        H(:, counter:(counter + num_s - 1) ) = ...
            mod(repmat(SPs(:,i), 1, num_s) + ...
                  SPs(:,new_i_s) + H_last_term, 2);
              
        counter = counter + num_s;
   end
   to_search = [find(new_i_ns); to_search(1:end-1)];
   finished(i) = true; 
end

H = H';

% symmetrize and make Tree upper triangular
Tree = Tree + Tree';
Tree = triu(Tree,1);

