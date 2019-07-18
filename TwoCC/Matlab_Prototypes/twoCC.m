function [y_blocks, cost] = twoCC(Advice, alg)

%% find the "blocks" of the advice graph
indices = to_blocks(Advice);
% blockT  = block_tree(indices);

%% determine which clustering algorithm to use
if strcmp(alg, 'spectral')
    cluster_func = @spectral_twoCC;
elseif strcmp(alg, 'dbc')
    cluster_func = @(x) SDP_twoCC(x, 'dbc');
elseif strcmp(alg, 'tri')
    cluster_func = @(x) SDP_twoCC(x, 'tri');
elseif strcmp(alg, 'PASTA')
    cluster_func = @PASTA_flip;
elseif strcmp(alg, 'PAST')
    cluster_func = @PAST;
elseif strcmp(alg, 'PASTA_rand')
    cluster_func = @(x) PASTA_flip_random (x, size (x, 1));
end



%% loop over the blocks and run "alg" on each block
y = zeros(size(Advice,1),1);
y_blocks  = zeros(size(Advice,1),length(indices));
cost = zeros(length(indices),1);

for i=1:length(indices)
    if length(indices{i}) == 1
        y_blocks(indices{i}, i) = 1;
        cost(i) = 0;
    elseif length(indices{i}) == 2
        y_blocks(indices{i}, i) = [1;Advice(indices{i}(1), indices{i}(2))];
    else
        [y_blocks(indices{i}, i), cost(i)] = cluster_func(Advice(indices{i},indices{i}));
    end
end

cost = sum(cost);
