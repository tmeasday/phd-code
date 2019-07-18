function Advices = generate_problems (n_ps, n, k, p_edge, p_flip)

    for i=1:n_ps
        [adv,y] = generate_problem (n, k, p_edge, p_flip);
        Advices{i} = adv;
    end