function [H, Tree, inT] = random_cycle_basis (Advice)

n = size (Advice, 1);
e = nnz (Advice);

[cols rows] = find (Advice');
edges = [rows cols];
edge_nos = sparse (n,n);
edge_nos (sub2ind ([n,n], edges(:,1), edges(:,2))) = 1:e;

Advice = Advice + Advice';
edge_nos = edge_nos + edge_nos';

Tree = sparse (n,n);
SPs = sparse (e, n);
inT = repmat (false, e, 1);
H = sparse (e, e-n+1);

visited = repmat (false, n, 1);

% starting node
curr = randint (1,1,[1,n]);
visited(curr) = true;

c = 1;
while (any (~visited))
   % take a step
   news = find (Advice (:, curr) ~= 0);
   new = news (randint (1,1,[1,length(news)]));
   
   % check if new node has been visited
   if (~visited (new)) 
       visited (new) = true;
       
       % add the edge to the tree
       Tree (curr, new) = 1;
       inT (edge_nos(curr,new)) = true;
       
       % update the shortest path information
       SPs (:, new) = SPs (:, curr);
       SPs (edge_nos(curr,new), new) = 1;
       
       % add all created cycles to H
       neighbours = Advice (:, new) ~= 0 & visited;
       % the parent is the only one that doesn't form a cycle
       neighbours(curr) = false; 
       
       numn = sum (neighbours);
       
       H_last_term   = sparse(e, numn);
       H_last_term(edge_nos(neighbours, new), :) = speye (full (numn));
            
       H(:, c:c+numn-1) = mod(repmat(SPs(:,new), 1, numn) + ...
           SPs(:,neighbours) + H_last_term, 2);
   
       c = c + numn;
   end
   
   curr = new;
end

% make sure the tree is upper triangular
Tree = Tree + Tree';
Tree = triu (Tree, 1);

H = H';