function [y, cost] = sample_and_insert (Advice, sample_size)

%% sample_and_insert (Advice, sample_size)
%
% sample sample_size vertices from the problem, solve exactly
% on that sample, and then insert remaining nodes in a random order in the
% sensible way, using the clustering defined by the sampled solution

n = size (Advice, 1);

if nargin == 1
    sample_size = round (log (n));
end

perm = randperm (n);
sample = perm (1:sample_size);

y = zeros (n, 1);
y(sample) = solve_exactly (Advice(sample, sample));

Advice = Advice + Advice';
for i = perm(sample_size+1:end)
   % insert node i into the problem, iteratively
   y(i) = ((Advice(i,:) * y) > 0) * 2 - 1;
end

% if we wanted to do them all at once, using only the sample to help us, we
% could do something like
% y(perm(n+1:end)) = Advice(perm(n+1:end), :) * y) * 2 - 1;

cost = CCcost(y, triu(Advice, 1));