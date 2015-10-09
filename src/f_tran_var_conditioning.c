/*
 * Implementation of the BBOB Gamma(?) transformation for variables.
 */
#include <math.h>
#include <assert.h>

#include "coco.h"
#include "coco_problem.c"

typedef struct {
  double *x;
  double alpha;
} _tv_con_data_t;

static void private_evaluate_tran_var_conditioning(coco_problem_t *self, const double *x, double *y) {
  size_t i;
  _tv_con_data_t *data;
  coco_problem_t *inner_problem;

  data = coco_get_transform_data(self);
  inner_problem = coco_get_transform_inner_problem(self);

  for (i = 0; i < self->number_of_variables; ++i) {
    /* OME: We could precalculate the scaling coefficients if we
     * really wanted to.
     */
    data->x[i] = pow(data->alpha, 0.5 * (double) (long) i / ((double) (long) self->number_of_variables - 1.0))
        * x[i];
  }
  coco_evaluate_function(inner_problem, data->x, y);
}

static void private_free_tran_var_conditioning(void *thing) {
  _tv_con_data_t *data = thing;
  coco_free_memory(data->x);
}

/**
 * Perform monotone oscillation(?) transformation on input variables.
 */
static coco_problem_t *f_tran_var_conditioning(coco_problem_t *inner_problem, const double alpha) {
  _tv_con_data_t *data;
  coco_problem_t *self;
  data = coco_allocate_memory(sizeof(*data));
  data->x = coco_allocate_vector(inner_problem->number_of_variables);
  data->alpha = alpha;
  self = coco_allocate_transformed_problem(inner_problem, data, private_free_tran_var_conditioning);
  self->evaluate_function = private_evaluate_tran_var_conditioning;
  return self;
}
