function [y, cost] = local_search (Advice, y)

%% local_search (Advice, y)
% 
% Do gradient descent on the solution given by y by swapping nodes between
% clusters.

if nargin == 1
    n = size (Advice, 1);
    y = randint (n, 1, 2) * 2 - 1;
end

EPS = 0.00001;
Advice = Advice + Advice';

while (true)
    changes = (Advice * y) .* y;
    
    [min_change, min_idx] = min (changes);
    
    if (min_change) < -EPS
        y(min_idx) = y(min_idx) * -1;
    else
        break;
    end
end

cost = CCcost(y, triu(Advice,1));