#pragma once

#include <QWidget>
#include <QPixmap>
#include <QImage>

class CutWidget : public QWidget
{
public:
    CutWidget(QWidget* parent = 0) : QWidget(parent) {}
    void setAspectRatio(qreal value); //<=0: not limited
    void setImage(const QImage& img);
    QRect cutRect() const;

protected:
    void paintEvent(QPaintEvent *);
    void resizeEvent(QResizeEvent *);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *);
private:
    void updateTransform();

    bool m_rotating = false;
    qreal m_ar;
    QPoint m_pos, m_posG;
    QPoint m_pos0, m_pos1;
    QRect m_rect;
    QRegion m_clip;
    int m_rot = 0, m_rot0 = 0;
    QTransform m_mat;
    QPixmap m_pixmap; //FIXME: why declare in another place will crash
};
