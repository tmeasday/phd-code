function Matrix = into_upper_triangle (vec)

%% This function makes the upper triangle of a Matrix and a vector

tri_n = length (vec);
n = (1 + sqrt (tri_n * 8 + 1)) / 2;
Matrix = zeros(n);

mask = ones(n,1) * [1:n] - [1:n]' * ones(1,n) < 0;

Matrix(mask) = vec;
Matrix = Matrix';