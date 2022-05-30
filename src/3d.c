#include "3d.h"

matrix_t s21_create_matrix(int rows, int columns) {
  matrix_t res;
  res.matrix = (double **)calloc(rows, sizeof(double *));
  res.rows = rows;
  res.columns = columns;
  for (int i = 0; i < rows; i++) {
    res.matrix[i] = (double *)calloc(columns, sizeof(double));
  }
  return res;
}

void s21_remove_matrix(matrix_t *A) {
  for (int i = 0; i < A->rows; i++) free(A->matrix[i]);
  free(A->matrix);
}

my_data *open_file(char *arr) {
  my_data *giga_struct = malloc(sizeof *giga_struct);
  FILE *fp;
  giga_struct->count_of_vertexes = 0;
  giga_struct->count_of_facets = 0;
  char line[126] = "\0";
  if ((fp = fopen(arr, "r")) == NULL) {
    // no file
  } else {
    while (!feof(fp)) {
      fgets(line, 100, fp);
      if (line[0] == 'v') {
        if (line[1] != ' ') continue;
        giga_struct->count_of_vertexes++;
      } else if (line[0] == 'f') {
        giga_struct->count_of_facets++;
      }
    }
    fclose(fp);
  }
  giga_struct->matrix_of_vertexes =
      s21_create_matrix(giga_struct->count_of_vertexes + 1, 3);
  giga_struct->polygons = (struct facets *)calloc(
      giga_struct->count_of_facets + 1, sizeof(struct facets));
  if ((fp = fopen(arr, "r")) == NULL) {
    // no file
  } else {
    int i_polygon = 1, j_matr = 1;
    while (!feof(fp)) {
      fgets(line, 100, fp);
      if (line[0] == 'v') {
        if (line[1] != ' ') continue;
        parc_matrix(line, &giga_struct->matrix_of_vertexes, j_matr);
        j_matr++;
      } else if (line[0] == 'f') {
        if (line[1] != ' ') continue;
        number_in_facets(&giga_struct->polygons[i_polygon], line);
        if (giga_struct->polygons[i_polygon].numbers_of_vertexes_in_facets) {
          giga_struct->polygons[i_polygon].vertexes = (int *)malloc(
              giga_struct->polygons[i_polygon].numbers_of_vertexes_in_facets *
              sizeof(int));
          parsing_in_polygon(&giga_struct->polygons[i_polygon], line);
        }
        i_polygon++;
      }
    }
    fclose(fp);
  }
  giga_struct->count_of_facets--;
  return giga_struct;
}

void parsing_in_polygon(polygon_t *polygon, char *line) {
  int tmp_polygon_in_vertex = 0;
  char mass[256] = "\0", *buf;
  for (int i = 0; line[i] != '\0'; i++) {
    if (!chislo(line[i])) {
      continue;
    } else {
      while (chislo(line[i])) {
        strncat(mass, line + i, 1);
        i++;
      }
      int facets = strtod(mass, &buf);
      polygon->vertexes[tmp_polygon_in_vertex] = facets;
      memset(mass, 0, 256);
      if (line[i] == ' ' || line[i] == '\0' || line[i] == '\n') {
        i--;
      } else {
        while (line[i] != '\0' && line[i] != ' ' && line[i] != '\n') {
          i++;
        }
        i--;
      }
    }
    tmp_polygon_in_vertex++;
  }
}

void number_in_facets(polygon_t *polygon, char *line) {
  int i = 0;
  polygon->numbers_of_vertexes_in_facets = 0;
  while (line[i] != '\0') {
    if (!chislo(line[i])) {
      i++;
      continue;
    } else {
      polygon->numbers_of_vertexes_in_facets += 1;
      while (line[i] != '\0' && line[i] != ' ' && line[i] != '\n') {
        i++;
      }
    }
  }
}

void parc_matrix(char *line, matrix_t *matrix, int row) {
  char mass[256] = "\0", *buf;
  int j = 0, sign = 1;
  for (int i = 0; line[i] != '\0'; i++) {
    if (!chislo(line[i])) {
      if (line[i] == '-') {
        sign = -1;
      }
      continue;
    } else {
      while (chislo(line[i]) || line[i] == '.') {
        strncat(mass, line + i, 1);
        i++;
      }
      double vershina = strtod(mass, &buf) * sign;
      matrix->matrix[row][j] = vershina;
      j++;
      i--;
      memset(mass, 0, 256);
      sign = 1;
    }
  }
}

bool chislo(char x) { return (x >= '0' && x <= '9'); }

void povorot_matr(matrix_t *matrix, double angle, int axis) {
  angle = (angle * M_PI) / 180;
  double temp_x, temp_y, temp_z;
  if (axis == 1) {
    for (int i = 1; i < matrix->rows; i++) {
      temp_y = matrix->matrix[i][1];
      temp_z = matrix->matrix[i][2];
      matrix->matrix[i][1] = cos(angle) * temp_y + sin(angle) * temp_z;
      matrix->matrix[i][2] = -sin(angle) * temp_y + cos(angle) * temp_z;
    }
  } else if (axis == 2) {
    for (int i = 1; i < matrix->rows; i++) {
      temp_x = matrix->matrix[i][0];
      temp_z = matrix->matrix[i][2];
      matrix->matrix[i][0] = cos(angle) * temp_x - sin(angle) * temp_z;
      matrix->matrix[i][2] = sin(angle) * temp_x + cos(angle) * temp_z;
    }
  } else if (axis == 3) {
    for (int i = 1; i < matrix->rows; i++) {
      temp_x = matrix->matrix[i][0];
      temp_y = matrix->matrix[i][1];
      matrix->matrix[i][0] = cos(angle) * temp_x + sin(angle) * temp_y;
      matrix->matrix[i][1] = -sin(angle) * temp_x + cos(angle) * temp_y;
    }
  }
}
