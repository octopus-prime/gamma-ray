/**
 * Solve for real or complex roots of the quartic equation
 * x^4 + a x^3 + b x^2 + c x + d = 0,
 * returning the number of such roots.
 *
 * Roots are returned ordered.
 */
int
gsl_poly_solve_quartic (double a, double b, double c, double d,
			double * x0, double * x1,
			double * x2, double * x3);
