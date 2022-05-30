#include "widget.h"

#include "ui_widget.h"
#define GL_SILENCE_DEPRECATION

Widget::Widget(QWidget *parent) : QOpenGLWidget(parent), ui(new Ui::Widget) {
  setGeometry(300, 75, 800, 600);
  ui->setupUi(this);
}

Widget::~Widget() {
  delete ui;
  if (giga_struct) {
    s21_remove_matrix(&giga_struct->matrix_of_vertexes);
    for (int i = 1; i <= giga_struct->count_of_facets + 1; i++) {
      free(giga_struct->polygons[i].vertexes);
    }
    free(giga_struct->polygons);
    free(giga_struct);
  }
}

void Widget::initializeGL() {
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  QString path = HOME_DIR "/Desktop/3DViewer_v1.0/setting.conf";
  QFile file(path);
  if (file.open(QIODevice::ReadOnly)) {
    QTextStream input(&file);
    QString string_file = input.readLine();
    QStringList list = string_file.split("=");
    proection_type = list[1].toDouble();
    string_file = input.readLine();
    list = string_file.split("=");
    line_type = list[1].toDouble();
    string_file = input.readLine();
    list = string_file.split("=");
    line_color = list[1].toDouble();
    string_file = input.readLine();
    list = string_file.split("=");
    line_thickness = list[1].toDouble();
    string_file = input.readLine();
    list = string_file.split("=");
    dot_type = list[1].toDouble();
    string_file = input.readLine();
    list = string_file.split("=");
    dot_color = list[1].toDouble();
    string_file = input.readLine();
    list = string_file.split("=");
    dot_thickness = list[1].toDouble();
    string_file = input.readLine();
    list = string_file.split("=");
    background_color = list[1].toDouble();
    file.close();
  }
  if (proection_type == 1)
    flag_proeks = 0;
  else
    flag_proeks = 1;

  line_types(line_type);
  line_colors(line_color);
  size_line = line_thickness;
  flag_vershina = dot_type;
  dot_colors(dot_color);
  size_point = dot_thickness;
  fon_color(background_color);
  ui->label_name_file->setText("Нет файла");
}

void Widget::resizeGL(int w, int h) {
  width = w, height = h;

  char open_str[256] = "\0";
  std::string tmp_input_str = ui->label->text().toUtf8().data();
  strcpy(open_str, tmp_input_str.c_str());
  setlocale(LC_NUMERIC, "C");
  if (new_file) {
    giga_struct = open_file(open_str);
    QString vertex = QString::number(giga_struct->count_of_vertexes, 'd', 0);
    ui->label_vertex->setText(vertex);
    new_file = 0;
  }
  if (gradus_x) povorot_matr(&giga_struct->matrix_of_vertexes, gradus_x, 1);
  if (gradus_y) povorot_matr(&giga_struct->matrix_of_vertexes, gradus_y, 2);
  if (gradus_z) povorot_matr(&giga_struct->matrix_of_vertexes, gradus_z, 3);
  gradus_x = 0, gradus_y = 0, gradus_z = 0;
  if (mashtab_minus) {
    for (int i = 1; i <= giga_struct->count_of_vertexes; i++) {
      for (int j = 0; j < 3; j++) {
        giga_struct->matrix_of_vertexes.matrix[i][j] *= mashtab_on_size;
      }
    }
    mashtab_minus = 0;
  }
  if (sdvig_x) {
    for (int i = 1; i <= giga_struct->count_of_vertexes; i++)
      giga_struct->matrix_of_vertexes.matrix[i][0] += sdvig_x;
    sdvig_x = 0;
  }
  if (sdvig_y) {
    for (int i = 1; i <= giga_struct->count_of_vertexes; i++)
      giga_struct->matrix_of_vertexes.matrix[i][1] += sdvig_y;
    sdvig_y = 0;
  }
  if (sdvig_z) {
    for (int i = 1; i <= giga_struct->count_of_vertexes; i++)
      giga_struct->matrix_of_vertexes.matrix[i][2] += sdvig_z;
    sdvig_z = 0;
  }

  float aspect = w / (float)h;
  m_projectionMatrix.setToIdentity();
  m_projectionMatrix.perspective(45, aspect, 0.1f, 10.0f);

  repaint();
}

void Widget::paintGL() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glClearColor(fon_r, fon_g, fon_b, 0);

  if (ui->label_name_file->text() == "cow.obj") {
    if (flag_proeks == 0) {
      glLoadIdentity();  // Сброс матрицы проекции
      glOrtho(-10, 10, -10, 10, -20, 20);
      glTranslatef(2, 0, 0);
    } else if (flag_proeks == 1) {
      glLoadIdentity();  // Сброс матрицы проекции
      glFrustum(-1, 1, -1, 1, 1, 100);
      glTranslatef(2, 0, -10);
    }
  } else if (ui->label_name_file->text() == "cube.obj") {
    if (flag_proeks == 0) {
      glLoadIdentity();  // Сброс матрицы проекции
      glOrtho(-1.5, 1.5, -1.5, 1.5, -10, 10);
      glTranslatef(0.3, 0, 0);
    } else if (flag_proeks == 1) {
      glLoadIdentity();  // Сброс матрицы проекции
      glFrustum(-1, 1, -1, 1, 1, 20);
      glTranslatef(0.3, 0.0, -2);
    }
  } else if (ui->label_name_file->text() == "lamp.obj") {
    if (flag_proeks == 0) {
      glLoadIdentity();  // Сброс матрицы проекции
      glOrtho(-10, 10, -10, 10, -10, 10);
      glTranslatef(2, -2, 0);
    } else if (flag_proeks == 1) {
      glLoadIdentity();  // Сброс матрицы проекции
      glFrustum(-1, 1, -1, 1, 1, 50);
      glTranslatef(2, -2, -10.5);
    }
  } else if (ui->label_name_file->text() == "magnolia.obj") {
    if (flag_proeks == 0) {
      glLoadIdentity();  // Сброс матрицы проекции
      glOrtho(-130, 130, -130, 130, -200, 200);
      glTranslatef(30, 10, 0);
    } else if (flag_proeks == 1) {
      glLoadIdentity();  // Сброс матрицы проекции
      glFrustum(-1, 1, -1, 1, 1, 200);
      glTranslatef(30, 10, -125);
    }
  } else if (ui->label_name_file->text() == "one_million.obj") {
    if (flag_proeks == 0) {
      glLoadIdentity();  // Сброс матрицы проекции
      glOrtho(-15, 15, -15, 15, -50, 50);
      glTranslatef(3, -10, 0);
    } else if (flag_proeks == 1) {
      glLoadIdentity();  // Сброс матрицы проекции
      glFrustum(-1, 1, -1, 1, 1, 100);
      glTranslatef(3, -10, -16);
    }
  } else if (ui->label_name_file->text() == "pumpkin.obj") {
    if (flag_proeks == 0) {
      glLoadIdentity();  // Сброс матрицы проекции
      glOrtho(-100, 100, -100, 100, -200, 200);
      glTranslatef(30, 0, 0);
    } else if (flag_proeks == 1) {
      glLoadIdentity();  // Сброс матрицы проекции
      glFrustum(-1, 1, -1, 1, 1, 200);
      glTranslatef(30, 0, 0);
    }
  } else if (ui->label_name_file->text() == "slot-machine.obj") {
    if (flag_proeks == 0) {
      glLoadIdentity();  // Сброс матрицы проекции
      glOrtho(-20, 20, -20, 20, -20, 20);
      glTranslatef(5, 0, 0);
    } else if (flag_proeks == 1) {
      glLoadIdentity();  // Сброс матрицы проекции
      glFrustum(-1, 1, -1, 1, 1, 80);
      glTranslatef(5, 0, -23.9);
    }
  } else if (ui->label_name_file->text() == "teapot.obj") {
    if (flag_proeks == 0) {
      glLoadIdentity();  // Сброс матрицы проекции
      glOrtho(-100, 100, -100, 100, -200, 200);
      glTranslatef(25, 0, 0);
    } else if (flag_proeks == 1) {
      glLoadIdentity();  // Сброс матрицы проекции
      glFrustum(-1, 1, -1, 1, 1, 300);
      glTranslatef(25, 0, -110);
    }
  } else if (ui->label_name_file->text() == "teddy.obj") {
    if (flag_proeks == 0) {
      glLoadIdentity();  // Сброс матрицы проекции
      glOrtho(-40, 40, -40, 40, -100, 100);
      glTranslatef(7, 0, 0);
    } else if (flag_proeks == 1) {
      glLoadIdentity();  // Сброс матрицы проекции
      glFrustum(-1, 1, -1, 1, 1, 100);
      glTranslatef(7, 0, -45);
    }
  } else if (ui->label_name_file->text() == "al.obj") {
    if (flag_proeks == 0) {
      glLoadIdentity();  // Сброс матрицы проекции
      glOrtho(-5, 5, -5, 5, -40, 40);
      glTranslatef(1, 0, 0);
    } else if (flag_proeks == 1) {
      glLoadIdentity();  // Сброс матрицы проекции
      glFrustum(-1, 1, -1, 1, 1, 20);
      glTranslatef(1, 0, -5.5);
    }
  } else if (ui->label_name_file->text() == "1bmw.obj") {
    if (flag_proeks == 0) {
      glLoadIdentity();  // Сброс матрицы проекции
      glOrtho(-150, 150, -150, 150, -300, 300);
      glTranslatef(50, 0, 0);
    } else if (flag_proeks == 1) {
      glLoadIdentity();  // Сброс матрицы проекции
      glFrustum(-1, 1, -1, 1, 1, 500);
      glTranslatef(50, 0, -300);
    }
  } else {
    if (flag_proeks == 0) {
      glLoadIdentity();  // Сброс матрицы проекции
      glOrtho(-150, 150, -150, 150, -300, 300);
      glTranslatef(50, 0, 0);
    } else if (flag_proeks == 1) {
      glLoadIdentity();  // Сброс матрицы проекции
      glFrustum(-1, 1, -1, 1, 1, 500);
      glTranslatef(10, 10, -300);
    }
  }
  glScalef(0.7, 1, 1);

  if (flag_shtrih == 1) {
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(2, shtrih);
  } else {
    glDisable(GL_LINE_STIPPLE);
  }
  glLineWidth(size_line);
  glColor3f(0, 1, 0);

  int rebro = 0;
  for (int i = 1; i <= giga_struct->count_of_facets; i++) {
    glBegin(GL_LINE_LOOP);
    glLineWidth(size_line);
    glColor3d(line_r, line_g, line_b);
    for (int j = 0; j < giga_struct->polygons[i].numbers_of_vertexes_in_facets;
         j++) {
      my_line(i, j);
      glVertex3d(x, y, z);
      rebro++;
    }
    rebro++;
    glEnd();
  }
  QString faces = QString::number(rebro, 'd', 0);
  ui->label_faces->setText(faces);

  glPointSize(size_point);
  if (flag_vershina == 2) {
    glEnable(GL_POINT_SMOOTH);
  } else {
    glDisable(GL_POINT_SMOOTH);
  }

  if (flag_vershina > 1) {
    glBegin(GL_POINTS);
    glColor3d(point_r, point_g, point_b);
    glPointSize(size_point);

    for (int i = 1; i < giga_struct->count_of_vertexes; i++) {
      x = giga_struct->matrix_of_vertexes.matrix[i][0];
      y = giga_struct->matrix_of_vertexes.matrix[i][1];
      z = giga_struct->matrix_of_vertexes.matrix[i][2];
      glVertex3d(x, y, z);
    }
    glEnd();
  }
}

void Widget::my_line(int i, int j) {
  x = giga_struct->matrix_of_vertexes
          .matrix[giga_struct->polygons[i].vertexes[j]][0];
  y = giga_struct->matrix_of_vertexes
          .matrix[giga_struct->polygons[i].vertexes[j]][1];
  z = giga_struct->matrix_of_vertexes
          .matrix[giga_struct->polygons[i].vertexes[j]][2];
}

void Widget::on_pushButton_open_clicked() {
  QString file = QFileDialog::getOpenFileName(this, "Выбрать файл", "",
                                              "Object files (*.obj)");

  if (!file.isEmpty()) {
    if (giga_struct) {
      s21_remove_matrix(&giga_struct->matrix_of_vertexes);
      for (int i = 1; i <= giga_struct->count_of_facets + 1; i++) {
        free(giga_struct->polygons[i].vertexes);
      }
      free(giga_struct->polygons);
      free(giga_struct);
    }

    ui->label->setText(file);
    QStringList list = ui->label->text().split("/");
    QString namefile;

    for (int i = 0; i < 15; i++) {
      if (list[i].endsWith(".obj")) {
        namefile = list[i];
        break;
      } else {
        continue;
      }
    }

    ui->label_name_file->setText(namefile);
    new_file = 1;
    resizeGL(width, height);
  }
}

void Widget::on_pushButton_x_clicked() {
  bool ok;
  double tmp_is_ok = ui->lineEdit_grad->text().toDouble(&ok);
  if (ok) {
    if (ui->radioButton_povorot->isChecked()) {
      gradus_x = tmp_is_ok;
    } else if (ui->radioButton_sdvig->isChecked()) {
      sdvig_x = tmp_is_ok;
    }
    resizeGL(width, height);
  }
}

void Widget::on_pushButton_y_clicked() {
  bool ok;
  double tmp_is_ok = ui->lineEdit_grad->text().toDouble(&ok);
  if (ok) {
    if (ui->radioButton_povorot->isChecked()) {
      gradus_y = tmp_is_ok;
    } else if (ui->radioButton_sdvig->isChecked()) {
      sdvig_y = tmp_is_ok;
    }
    resizeGL(width, height);
  }
}

void Widget::on_pushButton_z_clicked() {
  bool ok;
  double tmp_is_ok = ui->lineEdit_grad->text().toDouble(&ok);
  if (ok) {
    if (ui->radioButton_povorot->isChecked()) {
      gradus_z = tmp_is_ok;
    } else if (ui->radioButton_sdvig->isChecked()) {
      sdvig_z = tmp_is_ok;
    }
    resizeGL(width, height);
  }
}

void Widget::on_pushButton_clicked() {
  if (giga_struct) {
    s21_remove_matrix(&giga_struct->matrix_of_vertexes);
    for (int i = 1; i <= giga_struct->count_of_facets; i++) {
      free(giga_struct->polygons[i].vertexes);
    }
    free(giga_struct->polygons);
    free(giga_struct);
  }
  new_file = 1;
  resizeGL(width, height);
}

void Widget::on_pushButton_mashtab_minus_clicked() {
  bool ok;
  mashtab_on_size = ui->lineEdit_grad->text().toDouble(&ok);
  if (ok && mashtab_on_size > 0) {
    mashtab_minus = 1;
    resizeGL(width, height);
  }
}

void Widget::on_comboBox_fon_color_activated(int index) {
  if (index) {
    background_color = index;
    fon_color(index);
  }
}

void Widget::fon_color(int index) {
  if (index) {
    if (index == 1) {
      fon_r = 1, fon_g = 1, fon_b = 1;
      this->setStyleSheet("background-color: rgb(255, 255, 255);");
    } else if (index == 2) {
      fon_r = 1.0, fon_g = 0, fon_b = 0;
      this->setStyleSheet("background-color: rgb(255, 2, 0);");
    } else if (index == 3) {
      fon_r = 1.0, fon_g = 0.3, fon_b = 0;
      this->setStyleSheet("background-color: rgb(255, 77, 0);");
    } else if (index == 4) {
      fon_r = 1.0, fon_g = 1.0, fon_b = 0;
      this->setStyleSheet("background-color: rgb(255, 255, 0);");
    } else if (index == 5) {
      fon_r = 0, fon_g = 1.0, fon_b = 0;
      this->setStyleSheet("background-color: rgb(0, 255, 0);");
    } else if (index == 6) {
      fon_r = 0, fon_g = 1, fon_b = 1;
      this->setStyleSheet("background-color: rgb(0, 255, 255);");
    } else if (index == 7) {
      fon_r = 0, fon_g = 0, fon_b = 1;
      this->setStyleSheet("background-color: rgb(3, 0, 255);");
    } else if (index == 8) {
      fon_r = 0.5, fon_g = 0, fon_b = 1;
      this->setStyleSheet("background-color: rgb(130, 0, 255);");
    } else if (index == 9) {
      fon_r = 0, fon_g = 0, fon_b = 0;
      this->setStyleSheet("background-color: rgb(0, 0, 0);");
    }
    glClearColor(fon_r, fon_g, fon_b, 0);
  }
}

void Widget::on_comboBox_line_color_activated(int index) {
  if (index) {
    line_color = index;
    line_colors(index);
  }
}

void Widget::line_colors(int index) {
  if (index) {
    if (index == 1) {
      line_r = 0.0, line_g = 0.0, line_b = 0.0;
    } else if (index == 2) {
      line_r = 1.0, line_g = 0, line_b = 0;
    } else if (index == 3) {
      line_r = 1.0, line_g = 0.3, line_b = 0;
    } else if (index == 4) {
      line_r = 1.0, line_g = 1.0, line_b = 0;
    } else if (index == 5) {
      line_r = 0, line_g = 1.0, line_b = 0;
    } else if (index == 6) {
      line_r = 0, line_g = 1, line_b = 1;
    } else if (index == 7) {
      line_r = 0, line_g = 0, line_b = 1;
    } else if (index == 8) {
      line_r = 0.5, line_g = 0, line_b = 1;
    } else if (index == 9) {
      line_r = 1, line_g = 1, line_b = 1;
    }
  }
  if (flag_point_default == 0) {
    point_r = line_r, point_g = line_g, point_b = line_b;
  }
}

void Widget::on_comboBox_dot_color_activated(int index) {
  if (index) {
    dot_color = index;
    dot_colors(index);
  }
}

void Widget::dot_colors(int index) {
  if (index == 1) {
    point_r = line_r, point_g = line_g, point_b = line_b;
    flag_point_default = 0;
  } else if (index == 2) {
    point_r = 1.0, point_g = 0, point_b = 0;
    flag_point_default = 1;
  } else if (index == 3) {
    point_r = 1.0, point_g = 0.3, point_b = 0;
    flag_point_default = 1;
  } else if (index == 4) {
    point_r = 1.0, point_g = 1.0, point_b = 0;
    flag_point_default = 1;
  } else if (index == 5) {
    point_r = 0, point_g = 1.0, point_b = 0;
    flag_point_default = 1;
  } else if (index == 6) {
    point_r = 0, point_g = 1, point_b = 1;
    flag_point_default = 1;
  } else if (index == 7) {
    point_r = 0, point_g = 0, point_b = 1;
    flag_point_default = 1;
  } else if (index == 8) {
    point_r = 0.5, point_g = 0, point_b = 1;
    flag_point_default = 1;
  } else if (index == 9) {
    point_r = 1, point_g = 1, point_b = 1;
    flag_point_default = 1;
  } else if (index == 10) {
    point_r = 0, point_g = 0, point_b = 0;
    flag_point_default = 1;
  }
}

void Widget::on_comboBox_line_type_activated(int index) {
  if (index) {
    line_type = index;
    line_types(index);
  }
}

void Widget::line_types(int index) {
  if (index == 1) {
    flag_shtrih = 0;
  } else if (index == 2) {
    shtrih = 0X000F;
    flag_shtrih = 1;
  }
}

void Widget::on_pushButton_settings_clicked() {
  QString path = HOME_DIR "/Desktop/3DViewer_v1.0/setting.conf";
  QFile file(path);
  file.open(QIODevice::ReadWrite);
  file.flush();
  QTextStream out(&file);
  out << "proection_type=" << (qint32)proection_type << "\n";
  out << "line_type=" << (qint32)line_type << "\n";
  out << "line_color=" << (qint32)line_color << "\n";
  out << "line_thickness=" << (qint32)line_thickness << "\n";
  out << "dot_type=" << (qint32)dot_type << "\n";
  out << "dot_color=" << (qint32)dot_color << "\n";
  out << "dot_thickness=" << (qint32)dot_thickness << "\n";
  out << "background_color=" << (qint32)background_color << "\n";
  file.close();
  resizeGL(width, height);
}

void Widget::on_comboBox_line_size_activated(int index) {
  if (index) {
    line_thickness = index;
    size_line = index;
  }
}

void Widget::on_comboBox_dot_size_activated(int index) {
  if (index) {
    dot_thickness = index;
    size_point = index;
  }
}

void Widget::on_comboBox_dot_type_activated(int index) {
  if (index) {
    dot_type = index;
    flag_vershina = index;
  }
}

void Widget::on_comboBox_proection_type_activated(int index) {
  if (index) {
    proection_type = index;
    if (index == 1) {
      flag_proeks = 0;
    } else if (index == 2) {
      flag_proeks = 1;
    }
  }
}

void Widget::on_pushButton_2_clicked() {
  proection_type = 1, line_type = 1, line_color = 1, line_thickness = 1;
  dot_type = 1, dot_color = 1, dot_thickness = 1, background_color = 1;
  if (proection_type == 1)
    flag_proeks = 0;
  else
    flag_proeks = 1;

  line_types(line_type);
  line_colors(line_color);
  size_line = line_thickness;
  flag_vershina = dot_type;
  dot_colors(dot_color);
  size_point = dot_thickness;
  fon_color(background_color);
  on_pushButton_settings_clicked();
}

void Widget::on_pushButton_3_clicked() {
  QImage image = this->grabFramebuffer();
  QString FileName = QFileDialog::getSaveFileName(
      this, "Open a file", QDir::homePath(), "BMP (*.bmp);; JPEG (*.jpeg)");
  if (!FileName.isNull() && !FileName.isEmpty()) {
    image.save(FileName);
  }
}

void Widget::on_pushButton_4_clicked() {
  gif = new QGifImage(QSize(640, 480));
  gif->setDefaultDelay(100);
  glRotated(10, 0, 1, 0);
  timer = new QTimer(this);
  timer->start(100);
  connect(timer, SIGNAL(timeout()), this, SLOT(gif_slot()));
}

void Widget::gif_slot() {
  if (gif->frameCount() < 50) {
    QImage image;
    image = this->grabFramebuffer();
    ;
    image =
        image.scaled(640, 480, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    gif->addFrame(image);
    rot = 0;
  } else {
    QString FileName = QFileDialog::getSaveFileName(
        this, "Open a file", QDir::homePath(), "GIF (*.gif)");
    gif->save(FileName);
    timer->stop();
    delete gif;
  }
}
