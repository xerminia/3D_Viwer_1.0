#ifndef WIDGET_H
#define WIDGET_H
#ifdef __cplusplus
extern "C" {
#endif
#include "3d.h"
#ifdef __cplusplus
}
#endif
#define GL_SILENCE_DEPRECATION

#include <QFileDialog>
#include <QMatrix4x4>
#include <QMouseEvent>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLWidget>
#include <QTimer>
#include <QWidget>
#include <iostream>

#include "3rdParty/gifimage/qgifimage.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QOpenGLWidget {
  Q_OBJECT

 public:
  Widget(QWidget *parent = nullptr);
  ~Widget();

 protected:
  void initializeGL();
  void resizeGL(int w, int h);
  void paintGL();
  void my_line(int i, int j);
  void fon_color(int index);
  void line_colors(int index);
  void dot_colors(int index);
  void line_types(int index);

 private slots:
  void on_pushButton_open_clicked();
  void on_pushButton_x_clicked();
  void on_pushButton_y_clicked();
  void on_pushButton_z_clicked();
  void on_pushButton_clicked();
  void on_pushButton_mashtab_minus_clicked();
  void on_comboBox_fon_color_activated(int index);
  void on_pushButton_settings_clicked();
  void on_comboBox_line_type_activated(int index);
  void on_comboBox_line_size_activated(int index);
  void on_comboBox_line_color_activated(int index);
  void on_comboBox_dot_color_activated(int index);
  void on_comboBox_dot_size_activated(int index);
  void on_comboBox_dot_type_activated(int index);
  void on_comboBox_proection_type_activated(int index);
  void on_pushButton_2_clicked();
  void on_pushButton_3_clicked();
  void on_pushButton_4_clicked();
  void gif_slot();

 private:
  Ui::Widget *ui;
  QMatrix4x4 m_projectionMatrix;
  my_data *giga_struct = nullptr;
  QTimer *timer;
  int time;
  double x, y, z;
  double gradus_x = 0, gradus_y = 0, gradus_z = 0;
  int width, height,
      mashtab_minus = 0, new_file = 1;
  float sdvig_x = 0, sdvig_y = 0, sdvig_z = 0;
  float fon_r = 1.0f, fon_g = 1.0f, fon_b = 1.0f;
  float line_r = 0.0f, line_g = 0.0f, line_b = 0.0f;
  float point_r = line_r, point_g = line_g, point_b = line_b;
  bool flag_shtrih = 0;
  float shtrih;
  int xRot = 0;
  int size_line = 1;
  int size_point = 1;
  double mashtab_on_size;
  bool flag_point_default = 0;
  int flag_vershina = 0;
  int flag_proeks = 0;
  int proection_type = 1, line_type = 1, line_color = 1, line_thickness = 1,
      dot_type = 1, dot_color = 1, dot_thickness = 1, background_color = 1;
  QGifImage *gif;
  int rot = 0;
};
#endif  // WIDGET_H
