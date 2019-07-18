function y = PASTA_flip_single_new (Advice, H, Tree, inT)


H_inT = H(:,inT);

% get a list of the edges in the graph in canonical order 
[cols rows] = find(Advice');
edges = [rows cols];
edges_inT = edges(inT,:);

% get the advice from the graph (in the right order)
state = upper_triangle (Advice);
state = state(state ~= 0) == -1;

n_cycles = sum (H_inT, 1);

mistakes = mod (H * state, 2);

while true
    n_mistakes = mistakes' * H_inT;

    [worst, idx] = max (n_mistakes - n_cycles / 2);

    if (worst <= 0.25) % less than a half, no stress about round errors
        break;
    end

    mistakes = mod (mistakes + H_inT(:,idx), 2);
      
    Advice (edges_inT(idx,1), edges_inT(idx,2)) = Advice (edges_inT(idx,1), edges_inT(idx,2)) * -1;
end

y = PAST_single (Advice, Tree);

