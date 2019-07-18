#include <assert.h>

#ifdef GLPK
#include <glpk.h>
#else
#include <ilcplex/cplex.h>
#endif
/*
   lp-solve.c
   Tournaments
   
   Created by Tom Coleman on 2007-07-22.
   Copyright 2007 The University of Melbourne. All rights reserved.
*/

static int 
kill_output (void *info, const char *s)
{
	return 1;
}

double**
solve_digraph (digraph dg)
{
	int i, j, k;
	int n = dg.size;
	
	// xij for all i != j
	int n_cols = n * (n - 1);
	
	// we need n * n-1 * n-2 pijk constraints
	// and n * n - 1 / 2 qij constraints
	int n_rows = 	n_cols * (n - 2) + n_cols / 2;

#ifdef GLPK
	glp_prob *lp;
	lp = glp_create_prob ();
	glp_set_obj_dir (lp, GLP_MIN);
#else
  CPXENVptr env;
  int status = 0;
  env = CPXopenCPLEX (&status);
  assert (status == 0);
  CPXLPptr lp;
  lp = CPXcreateprob (env, &status, "test");
  assert (status == 0);
  CPXchgobjsen (env, lp, CPX_MIN);
#endif
	
#ifdef GLPK
	glp_add_cols (lp, n_cols);
	int col_no = 1;
#else
  double obj[n_cols], ub[n_cols], lb[n_cols];
	int col_no = 0;
#endif

	int col_nos[n][n];
	for (i = 0; i < n; i += 1) {
		for (j = 0; j < n; j += 1) {
			if (i == j) continue;
			
			// save this for later
			col_nos[i][j] = col_no;
			
#ifdef GLPK
			glp_set_col_bnds (lp, col_no, GLP_LO, 0.0, 0.0);
			glp_set_obj_coef (lp, col_no, dg_weight (dg, i, j));
#else
      obj[col_no] = dg_weight (dg, i, j);
      lb[col_no] = 0.0;
      ub[col_no] = CPX_INFBOUND;
#endif
			col_no += 1;
		}
	}

#ifndef GLPK
  status = CPXnewcols (env, lp, n_cols, obj, lb, ub, NULL, NULL);
  assert (status == 0);
#endif
	
#ifdef GLPK
	glp_add_rows (lp, n_rows);
	int row_no = 1;
#else
  int nzcnt = 3 * n_cols * (n - 2) + 2 * n_cols / 2;
  int cntno = 0;
  double rhs[n_rows], rmatval[nzcnt];
  char sense[n_rows];
  int matbeg[n_rows], matind[nzcnt];
	int row_no = 0;
#endif

	for (i = 0; i < n; i += 1) {
		for (j = i+1; j < n; j += 1) {

#ifdef GLPK
			glp_set_row_bnds (lp, row_no, GLP_FX, 1.0, 1.0);
			
			int    is[] = {0, col_nos[i][j], col_nos[j][i]};
			double as[] = {0, 1, 1};
			glp_set_mat_row  (lp, row_no, 2, is, as);
#else
      rhs[row_no]         = 1.0;
      sense[row_no]       = 'E';
      matbeg[row_no]      = cntno;
      matind[cntno]       = col_nos[i][j]; 
      rmatval[cntno++]    = 1.0;
      matind[cntno]       = col_nos[j][i];
      rmatval[cntno++]    = 1.0;
#endif
			
			row_no += 1;
		}
	}
	
	for (i = 0; i < n; i += 1) {
		for (j = 0; j < n; j += 1) {
			if (i == j) continue;
			for (k = 0; k < n; k += 1) {
				if (i == k || j == k) continue;

#ifdef GLPK	
				glp_set_row_bnds (lp, row_no, GLP_LO, 0.0, 0.0);
			
				int    is[] = {0, col_nos[i][j], col_nos[j][k], col_nos[i][k]};
				double as[] = {0, 1, 1, -1};
				glp_set_mat_row (lp, row_no++, 3, is, as);
#else
      rhs[row_no]         = 0.0;
      sense[row_no]       = 'G';
      matbeg[row_no]      = cntno;
      matind[cntno]       = col_nos[i][j]; 
      rmatval[cntno++]    = 1.0;
      matind[cntno]       = col_nos[j][k]; 
      rmatval[cntno++]    = 1.0;
      matind[cntno]       = col_nos[i][k]; 
      rmatval[cntno++]    = -1.0;
#endif
				row_no += 1;
			}
		}
	}
#ifndef GLPK
  status = CPXaddrows (env, lp, 0, n_rows, cntno, rhs, sense,
                       matbeg, matind, rmatval, NULL, NULL);
  assert (status == 0);
#endif

#ifdef GLPK 
	glp_term_hook (kill_output, NULL);
	lpx_simplex (lp);
#else
  status = CPXlpopt (env, lp);
  assert (status == 0);

  double x[n_cols];
  status = CPXsolution (env, lp, NULL, NULL, x, NULL, NULL, NULL);
  assert (status == 0);
#endif
	
	double** solution = malloc (n * sizeof (double*));
	assert (solution != NULL);
	for (i = 0; i < n; i += 1) {
		solution[i] = malloc (n * sizeof (double));
		assert (solution[i] != NULL);
		
		for (j = 0; j < n; j += 1) {
			if (i == j) continue;

#ifdef GLPK
			solution[i][j] = glp_get_col_prim (lp, col_nos[i][j]);
#else
      solution[i][j] = x[col_nos[i][j]];
#endif
		}
	}

#ifdef GLPK
	glp_delete_prob (lp);
#else
  CPXfreeprob (env, &lp);
  CPXcloseCPLEX (&env);
#endif

	return solution;
}
