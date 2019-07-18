function y = PAST_single (Advice, Tree)

%% PAST - Pick a Spanning Tree. 
%
% This function does a single run of PAST. So we've already 'picked' the
% spanning tree---now we need to grab the clustering. Well this is quite
% easy really--just give a label to 1 and then walk the tree, assigning
% labels as we go.

% makes my life easier
Advice = Advice + Advice';
Tree = Tree + Tree';

n = size(Advice,1);
y = zeros(1,n);
y(1) = 1;
to_search = [1];
searched  = repmat(false, 1, size(Advice, 1));

while ~isempty (to_search)
   i = to_search(end);
   % much faster to operate column-wise on sparse matrices
   new_i = (Tree(:,i) == 1)';
   new_i = new_i & ~searched;
   
   y(new_i) = Advice(i,new_i) * y(i); 
   
   searched(i) = true;
   
   to_search = [find(new_i), to_search(1:end-1)];
end

% make y into a column vector
y = y';
