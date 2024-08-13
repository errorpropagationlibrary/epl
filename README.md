
The Error Propagation library EPL is a C++ library designed to extend
calculations in a painless way to obtain a Gaussian error on your
results if you know the error of your input. 

There are two ways to look on EPL: First as an extension of a floating
point type like double or float to include an error. Second as
class for stochastic variables limited to Gaussian distributions. You
will be able to read the manual and use EPL without touching stochastic,
except if you read chapter Math foundation.

EPL maintains something like a correlation matrix which allows to get
the correlation between your variables. The library is designed for easy
use instead of for fancy features or performance. EPL is a small piece of
code, consider that a design feature.
