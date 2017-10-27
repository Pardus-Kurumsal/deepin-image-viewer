#pragma once
#include "utils/imageutils.h"
#include "slideeffect.h"
#include <QThread>
#include <QMap>

class CacheThread : public QThread
{
    Q_OBJECT
public:
    CacheThread(const QString &path)
        : QThread(NULL)
        , m_path(path) {}

signals:
    void cached(QString, QImage);

protected:
    void run() Q_DECL_OVERRIDE
    {
        QImage img = utils::image::getRotatedImage(m_path);
        emit cached(m_path, img);
    }

private:
    QString m_path;
};

class SlideEffectPlayer : public QObject
{
    Q_OBJECT
public:
    SlideEffectPlayer(QObject* parent = 0);
    void setFrameSize(int width, int height);
    QSize frameSize() const { return QSize(m_w, m_h);}
    // call setCurrentImage later
    void setImagePaths(const QStringList& paths);
    // invalid path: black image+1st image
    void setCurrentImage(const QString& path = QString());
    QString currentImagePath() const;
    bool isRunning() const;

Q_SIGNALS:
    void frameReady(const QImage& image);
    void finished();
    void currentImageChanged(const QString& path);
    void stepChanged(int steps);

public Q_SLOTS:
    void start();
    void stop();
    void pause();

protected:
    void timerEvent(QTimerEvent *e);

private:
    int duration() const;
    bool startNext();
    void cacheNext();

private:
    bool m_running = false;
    bool m_pausing = false;
    bool m_random = true;
    int m_tid;
    int m_w, m_h;
    QMap<QString, QImage> m_cacheImages;
    QStringList m_paths;
    QStringList::ConstIterator m_current;
    QThread m_thread;
    SlideEffect *m_effect = NULL;
};
