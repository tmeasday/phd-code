function [y_blocks, cost] = local_search_blocks(Advice, y_blocks)

%% local_search_blocks(Advice, y_blocks)
% given a clustering for each of the blocks of the graph, run local search
% on each one and return the result.

cost = zeros(size(y_blocks,2),1);

for i=1:size(y_blocks,2)
    block = y_blocks(:,i) ~= 0;
    if sum(block)==1
        y_block(block,i) = 1;
        cost(i) = 0;
    else
        y_blocks(block,i) = local_search(Advice(block,block), y_blocks(block,i));
        cost(i) = CCcost(y_blocks(block,i), Advice(block,block));
    end
end

cost = sum(cost);

