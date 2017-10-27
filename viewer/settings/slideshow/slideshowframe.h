#ifndef SLIDESHOWFRAME_H
#define SLIDESHOWFRAME_H

#include <QFrame>

class QVBoxLayout;
class SlideshowFrame : public QFrame
{
    Q_OBJECT
public:
    explicit SlideshowFrame(QWidget *parent);

signals:
    void requestReset();

private:
    int defaultDuration() const;
    void initPreview();
    void initInterval();

private:
    QVBoxLayout *m_layout;
};

#endif // SLIDESHOWFRAME_H
