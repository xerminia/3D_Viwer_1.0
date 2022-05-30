#include <check.h>

#include "3d.h"

START_TEST(test_cube) {
  char arr[20] = "modeles/cube.obj";
  my_data *giga_struct = open_file(arr);
  double check[24] = {-0.5, -0.5, -0.5, -0.5, -0.5, 0.5,  -0.5, 0.5,
                      -0.5, -0.5, 0.5,  0.5,  0.5,  -0.5, -0.5, 0.5,
                      -0.5, 0.5,  0.5,  0.5,  -0.5, 0.5,  0.5,  0.5};
  int k = 0;
  for (int i = 1; i <= giga_struct->count_of_vertexes; i++) {
    for (int j = 0; j < 3; j++) {
      ck_assert_uint_eq(giga_struct->matrix_of_vertexes.matrix[i][j], check[k]);
      k++;
    }
  }
  k = 0;
  int check2[25] = {1, 2, 4, 3, 1, 3, 7, 5, 7, 5, 6, 8, 6, 2, 4, 8};
  for (int i = 1; i <= giga_struct->count_of_facets; i++) {
    for (int j = 0; j < giga_struct->polygons[i].numbers_of_vertexes_in_facets;
         j++) {
      ck_assert_int_eq(giga_struct->polygons[i].vertexes[j], check2[k]);
      k++;
    }
  }
  povorot_matr(&giga_struct->matrix_of_vertexes, 90, 1);
  double check_rotate_x[24] = {-0.5, -0.5, 0.5, -0.5, 0.5,  0.5,  -0.5, -0.5,
                               -0.5, -0.5, 0.5, -0.5, 0.5,  -0.5, 0.5,  0.5,
                               0.5,  0.5,  0.5, -0.5, -0.5, 0.5,  0.5,  -0.5};
  k = 0;
  for (int i = 1; i <= giga_struct->count_of_vertexes; i++) {
    for (int j = 0; j < 3; j++) {
      ck_assert_double_eq_tol(giga_struct->matrix_of_vertexes.matrix[i][j],
                              check_rotate_x[k], 0.0000001);
      k++;
    }
  }
  povorot_matr(&giga_struct->matrix_of_vertexes, 90, 2);
  double check_rotate_y[24] = {-0.5, -0.5, -0.5, -0.5, 0.5,  -0.5, 0.5, -0.5,
                               -0.5, 0.5,  0.5,  -0.5, -0.5, -0.5, 0.5, -0.5,
                               0.5,  0.5,  0.5,  -0.5, 0.5,  0.5,  0.5, 0.5};
  k = 0;
  for (int i = 1; i <= giga_struct->count_of_vertexes; i++) {
    for (int j = 0; j < 3; j++) {
      ck_assert_double_eq_tol(giga_struct->matrix_of_vertexes.matrix[i][j],
                              check_rotate_y[k], 0.0000001);
      k++;
    }
  }
  povorot_matr(&giga_struct->matrix_of_vertexes, 90, 3);
  double check_rotate_z[24] = {-0.5, 0.5, -0.5, 0.5,  0.5,  -0.5, -0.5, -0.5,
                               -0.5, 0.5, -0.5, -0.5, -0.5, 0.5,  0.5,  0.5,
                               0.5,  0.5, -0.5, -0.5, 0.5,  0.5,  -0.5, 0.5};
  k = 0;
  for (int i = 1; i <= giga_struct->count_of_vertexes; i++) {
    for (int j = 0; j < 3; j++) {
      ck_assert_double_eq_tol(giga_struct->matrix_of_vertexes.matrix[i][j],
                              check_rotate_z[k], 0.0000001);
      k++;
    }
  }
  s21_remove_matrix(&giga_struct->matrix_of_vertexes);
  for (int i = 0; i <= giga_struct->count_of_facets + 1; i++) {
    free(giga_struct->polygons[i].vertexes);
  }
  free(giga_struct->polygons);
  free(giga_struct);
}
END_TEST

int main(void) {
  Suite *s1 = suite_create("It's OK");
  SRunner *sr = srunner_create(s1);
  int nf;

  TCase *tc1_1 = tcase_create("FAIL");
  suite_add_tcase(s1, tc1_1);
  tcase_add_test(tc1_1, test_cube);

  srunner_run_all(sr, CK_ENV);
  nf = srunner_ntests_failed(sr);
  srunner_free(sr);

  return nf == 0 ? 0 : 1;
}
