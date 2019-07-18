function [y, cost] = spectral_twoCC(Advice)

%% function: [y, cost] = spectral_twoCC(Advice)
%
% arguments:  
% -- Advice, a sparse upper triangular matrix where 
%    Advice(i,j) = 0            for i >= j
%    Advice(i,j) \in {-1, 0, 1} for i <  j
%
% returns:
% -- y, the output clustering. Note that WLOG y(1) = 1.
% -- cost, the 2CC cost of y.
%
% notes: 
% -- if there are multiple eigenvectors of minimum relaxed cost, we pick 
%    one arbitrarily.
% -- currently computing all the eigenvalues/vectors.
% -- currently rounding by cutting at zero. May do "line search" in the
%    future.

%% Form the advice-block matrix of Advice
Advice = Advice + Advice';
d = sum (abs (Advice), 2);
M = diag (d) - Advice;

%% Find the eigenvector corresponding to the smallest eigenvalue of M
%[Vecs, Vals] = eigs(M,1,'sm');
[Vecs, Vals] = eig(full(M));
[lambda, idx] = min (diag (Vals));
%idx = 1;
v = Vecs(:,idx);

%% Round to give a clustering with y(1) = 1.
y = 2 * (v > 0) - 1;
y = y(1) * y;

%% compute final cost
cost = 0.25 * y' * M * y;
