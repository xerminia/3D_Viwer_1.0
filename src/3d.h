#ifndef SRC_3D_H_
#define SRC_3D_H_

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SUCCESS 1
#define FAILURE 0

typedef struct matrix_struct {
  double **matrix;
  int rows;
  int columns;
} matrix_t;

typedef struct facets {
  int *vertexes;
  int numbers_of_vertexes_in_facets;
} polygon_t;

typedef struct my_data {
  int count_of_vertexes;
  int count_of_facets;
  matrix_t matrix_of_vertexes;
  polygon_t *polygons;
} my_data;

matrix_t s21_create_matrix(int rows, int columns);
void s21_remove_matrix(matrix_t *A);
matrix_t parc_vertexes(char *arr, int rows);
bool chislo(char x);
my_data *open_file(char *arr);
void parc_matrix(char *line, matrix_t *matrix, int row);
void parsing_in_polygon(polygon_t *polygon, char *line);
void number_in_facets(polygon_t *polygon, char *line);
void povorot_matr(matrix_t *matrix, double angle, int axis);

#endif  // SRC_3D_H_
