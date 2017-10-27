#ifndef SLIDER_H
#define SLIDER_H

#include <QSlider>

class Slider : public QSlider
{
    Q_OBJECT
public:
    explicit Slider(QWidget *parent = 0);
    explicit Slider(Qt::Orientation orientation, QWidget *parent = 0);
protected:
  void mousePressEvent (QMouseEvent *e) Q_DECL_OVERRIDE;

};

#endif // SLIDER_H
