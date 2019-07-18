function Tree = breadth_first_st (Advice, vertex)

%% read in a graph and a vertex and returns the bfs tree rooted at vertex


n = size (Advice,1);

Advice = Advice + Advice';

to_search = [vertex];
searched  = repmat(false, 1, n);
searched(vertex) = true;

Tree = sparse (n,n);
while ~isempty (to_search)
   i = to_search(end);
   % much faster to operate column-wise on sparse matrices
   new_i = (Advice(:,i) ~= 0)';
   new_i = new_i & ~searched;
   
   Tree (i, new_i & 1:n > i) = 1;
   Tree (new_i & 1:n < i, i) = 1;
   
   searched(new_i) = true;
   
   to_search = [find(new_i), to_search(1:end-1)];
end



