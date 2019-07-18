function [y, cost] = SDP_twoCC(Advice, mode)

%% function: [y, cost] = SDP_twoCC(Advice, mode)
%
% arguments:  
% -- Advice, a sparse upper triangular matrix where 
%    Advice(i,j) = 0            for i >= j
%    Advice(i,j) \in {-1, 0, 1} for i <  j
% -- mode, a string determining which algorithm to use, where
%    mode \in {'dbc', 'tri'}
%
% returns:
% -- y, the output clustering. Note that WLOG y(1) = 1.
% -- cost, the 2CC cost of y.
%
% notes: currently rounding with spherical 2-means 

%% Problem size
n = size (Advice,1);

%% Ensuring some input sanity
if nargin == 1
    mode = 'dbc';
end
if (~strcmp(mode, 'dbc')) && (~strcmp(mode, 'tri'))
    error ('Second argument must be ''dbc'' or ''tri''');
end
if strcmp(mode, 'tri') && n > 20
    error ('Problem too large for this algorithm');
end
if n <= 2
    error ('Should not call this function with problem of size <= 2');
end

%% Form the advice-block matrix of Advice
Advice = Advice + Advice';
d = sum (abs (Advice), 2);
M = diag (d) - Advice;

%% SDP parameters
num_var_dbc = n^2;
num_con_dbc = n;

%% constraints X_ii = 1
  b_dbc = sparse (ones (num_con_dbc,1));
con_dbc = sparse (num_con_dbc, num_var_dbc);
for i=1:n
    Ei           = sparse(n,n);
    Ei(i,i)      = 1;
    con_dbc(i,:) = Ei(:)';
end

%% squared triangle inequalities: X_ij + X_jk - X_ik - X_jj <= 0
if strcmp(mode, 'tri')
    num_var_tri = nchoosek (n,3);
    num_con_tri = nchoosek (n,3);
        con_tri = sparse(num_con_tri, num_var_dbc);
    ct = 1;
    for i=1:(n-2)
        for j=2:(n-1)
            for k=3:n
                Eijk          = sparse(n,n);
                Eijk(i,j)     = 1;
                Eijk(j,k)     = 1;
                Eijk(i,k)     = -1;
                Eijk(j,j)     = -1;
                con_tri(ct,:) = Eijk(:)';
                ct            = ct + 1;
            end
        end
    end
else
    num_var_tri = 0;
    num_con_tri = 0;
    con_tri     = [];
end

%% final SDP parameters
con = [sparse(num_con_dbc, num_var_tri), con_dbc; ...
       sparse(eye (num_con_tri))       , con_tri];
b   = [b_dbc; zeros(num_con_tri)];

K.f = 0;
K.l = num_var_tri;
K.q = 0;
K.r = 0;
K.s = n;

%% solve the SDP and transform back to a Gram matrix
[X,Y,info] = sedumi (con,b,M(:),K);
X          = reshape (X((num_var_tri+1):end),n,n);

%% Factorize to get V
[U,S] = eig(X);
V     = U * sqrt(S) * U';

%% Round to give a clustering with y(1) = 1.
y = (kmeans (V,2,'Distance','cosine')) * 2 - 3;
y = y(1) * y;

%% Compute final cost
cost = 0.25 * y' * M * y;
