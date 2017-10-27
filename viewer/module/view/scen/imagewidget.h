#pragma once

#include <QWidget>
#include <QImage>

class ImageWidget : public QWidget
{
    Q_OBJECT
public:
    ImageWidget(QWidget *parent = 0);

    void setImage(const QString &path);
    void setImage(const QImage& image);
    QImage image() const {return m_image;}
    QString imageName() const;
    QString imagePath() const;
    void fitImage();
    void fitWindow();
    qreal scaleValue() const;
    qreal windowRelativeScale() const;
    void setTransformOrigin(const QPointF &imageP, const QPointF &deviceP);
    QPoint mapToImage(const QPoint& p) const;
    QRect mapToImage(const QRect& r) const;
    QRect visibleImageRect() const;
    bool isWholeImageVisible() const;
    bool isMoving() const;

    bool inSlideShow() const;
    void setInSlideShow(bool inSlideShow);

Q_SIGNALS:
    void scaleValueChanged(qreal);
    void transformChanged(const QTransform&);
    void doubleClicked();
    void fliped(bool x, bool y);
    void mouseMoved();

public Q_SLOTS:
    void setImageMove(int x, int y);
    void setScaleValue(qreal value);
    void rotateClockWise();
    void rotateCounterclockwise();
    void rotate(int deg);
    void flipX();
    void flipY();
protected:
    void resizeEvent(QResizeEvent *e) override;
    void timerEvent(QTimerEvent* e) override;
    void paintEvent(QPaintEvent *) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *) override {
        Q_EMIT doubleClicked();
    }

private:
    const QPointF rectCenter() const;
    const QRectF imageRect() const;
    void resetTransform();
    void updateTransform();

    int m_flipX = 1;
    int m_flipY = 1;
    int m_rot = 0;
    int m_tid = 0;
    bool m_scaling = false;
    bool m_moving = false;
    bool m_inSlideShow = false;
    qreal m_scale = 0; // when an image is loaded to fit widget, m_scale is not 1.0
    QPointF m_o_img, m_o_dev;
    QString m_path;
    QImage m_image;
    QSize m_imageOriginSize = QSize();
    QPixmap m_pixmap;
    QTransform m_mat;

    QPoint m_pos, m_posG;
};
