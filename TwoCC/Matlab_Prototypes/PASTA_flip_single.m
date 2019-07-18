function y = PASTA_flip_single (Advice, Tree)

%% Pick a Spanning Tree and flip
%
% 

n = size (Advice, 1);
[rows, cols] = find (Tree);
% edges = [rows, cols];

Treenums = sparse (n,n);
Treenums((cols-1)*n+rows) = 1:n-1;
Treenums = Treenums + Treenums';

%% First work out the SPL matrix -- shortest path lengths on the tree
%  between all of the points

SPL = zeros (n);
Tree = Tree + Tree';
visited = eye (n) == 1;

length = 1;
T = Tree;
while any (any (~visited))
    new = (T == 1) & ~visited;
    
    SPL(new) = length;
    visited(new) = true;
    
    T = T*Tree;
    length = length + 1;
end
clear visited;

%% Next work out what the shortest paths actually are...

rownums = into_upper_triangle (1:n*(n-1)/2);
rownums = rownums + rownums';


paths = sparse (n*(n-1)/2, n-1);
SPL_p_1 = eye(n) == 1;
for p = 1:max(max(SPL))
    SPL_p = SPL == p;

    [is, js] = find (triu (SPL_p,1));
    i_ks = SPL_p_1 (is,:);
    % much faster to operate column-wise on sparse matrices
    j_ks = (Tree (:, js) == 1)';
    
    [id_, ks] = find (i_ks & j_ks);
    
    ijs = rownums (n*(js-1) + is);
    if (p > 1)
        iks = rownums (n*(ks-1) + is);
        % much faster to operate column-wise on sparse matrices
        paths = paths';
        paths (:, ijs) = paths (:, iks);
        paths = paths';
    end
    
    jks = Treenums (n*(ks-1) + js);
    paths (ijs + size (paths,1) * (jks-1)) = repmat (1, size (is,1), 1);
    
    SPL_p_1 = SPL_p;
end
clear rownums;
clear SPL;
clear SPL_p_1;
clear SPL_p;


%% Construct H

% go through each possible edge. if not in G, delete row from H
% if is in G, but also in T, also delete row from H
% else insert elementary vector as a col of H in the right spot

% first do the rows
inG = upper_triangle (Advice) ~= 0;
inT = upper_triangle (Tree) == 1;

H_inT = paths(inG & ~inT, :);


%% Now start ya flippin'
[rows, cols] = find (Advice ~= 0);
% sort them into the right order
[rows, p] = sort (rows);
edges = [rows, cols(p)];

state       = upper_triangle (Advice);
state_inT   = state(inT) == -1;
state_other = state(inG & ~inT) == -1;

n_cycles = sum (H_inT, 1);
edge_idxs = find(inT);

mistakes = mod (H_inT * state_inT + state_other, 2);

clear inG inT state_inT state_other;

while true
    n_mistakes = mistakes' * H_inT;

    [worst, idx] = max (n_mistakes - n_cycles / 2);

    if (worst <= 0.25) % less than a half, no stress about round errors
        break;
    end

    mistakes = mod (mistakes + H_inT(:,idx), 2);
    
    eidx = edge_idxs (idx);
    Advice (edges(eidx,1), edges(eidx,2)) = Advice (edges(eidx,1), edges(eidx,2)) * -1;
end
    
% PAST_single needs tree to be upper triangular
y = PAST_single (Advice, triu(Tree,1));