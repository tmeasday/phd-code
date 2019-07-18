function [costs, time, lbls] = do_dasguptas (Advices, repeats)
    
n_ps = length (Advices);

tic;
for i=1:n_ps
    [y, n] = RepeatPartition (Advices{i}, repeats);
    n_edges = length (find (Advices{i}));
    
    costs(i) = n_edges - n;
    lbls{i} = y;
end
time = toc;