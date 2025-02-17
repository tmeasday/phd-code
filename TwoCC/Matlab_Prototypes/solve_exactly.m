function [y, mincost] = solve_exactly (Advice)

    n = size (Advice,1);
    
    ys = [de2bi(0:2^(n-1) -1) * 2 - 1, ones(2^(n-1), 1)]';
    
    [mincost, idx] = min(CCcost(ys, Advice));
    y = ys(:,idx);