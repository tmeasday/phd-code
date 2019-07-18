function vec = upper_triangle (Matrix)

%% This function takes the upper triangle of a Matrix and returns a vector

n = size(Matrix,2);

mask = ones(n,1) * [1:n] - [1:n]' * ones(1,n) < 0;

Matrix = Matrix';
vec = Matrix (mask);