function [costs, time, lbls] = twoCCs(Advices, alg)

    tic;
    for i=1:length(Advices)
        [ys, cost] = twoCC (Advices{i}, alg);
        costs(i) = cost;
        lbls{i} = ys;
    end
    time = toc;