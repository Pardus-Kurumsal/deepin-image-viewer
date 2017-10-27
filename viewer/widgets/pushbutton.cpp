#include "pushbutton.h"
#include "application.h"
#include <QDebug>
#include <QDesktopWidget>
#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QPainter>
#include <QPaintEvent>
#include <QTimer>

PushButton::PushButton(QWidget *parent)
    : QWidget(parent)
    , m_checked(false)
    , m_isPressed(false)
    , m_spacing(2)
{
    onThemeChanged(dApp->viewerTheme->getCurrentTheme());
    connect(dApp->viewerTheme, &ViewerThemeManager::viewerThemeChanged,
            this, &PushButton::onThemeChanged);
}

QString PushButton::normalPic() const
{
    return m_normalPic;
}

QString PushButton::hoverPic() const
{
    return m_hoverPic;
}

QString PushButton::pressPic() const
{
    return m_pressPic;
}

QString PushButton::disablePic() const
{
    return m_disablePic;
}

QString PushButton::text() const
{
    return m_text;
}

QColor PushButton::normalColor() const
{
    return m_normalColor;
}

QColor PushButton::hoverColor() const
{
    return m_hoverColor;
}

QColor PushButton::pressColor() const
{
    return m_pressColor;
}

QColor PushButton::disableColor() const
{
    return m_disableColor;
}

void PushButton::setNormalPic(QString normalPic)
{
    if (m_normalPic == normalPic)
        return;

    m_normalPic = normalPic;
    m_currentPic = normalPic;
    emit normalPicChanged(normalPic);
}

void PushButton::setHoverPic(QString hoverPic)
{
    if (m_hoverPic == hoverPic)
        return;

    m_hoverPic = hoverPic;
    emit hoverPicChanged(hoverPic);
}

void PushButton::setPressPic(QString pressPic)
{
    if (m_pressPic == pressPic)
        return;

    m_pressPic = pressPic;
    emit pressPicChanged(pressPic);
}

void PushButton::setDisablePic(QString disablePic)
{
    if (m_disablePic == disablePic)
        return;

    m_disablePic = disablePic;
    emit disablePicChanged(disablePic);
}

void PushButton::setText(QString text)
{
    if (m_text == text)
        return;

    m_text = text;
    emit textChanged(text);
}

void PushButton::setNormalColor(QColor normalColor)
{
    if (m_normalColor == normalColor)
        return;

    m_normalColor = normalColor;
    m_currentColor = normalColor;
    emit normalColorChanged(normalColor);
}

void PushButton::setHoverColor(QColor hoverColor)
{
    if (m_hoverColor == hoverColor)
        return;

    m_hoverColor = hoverColor;
    emit hoverColorChanged(hoverColor);
}

void PushButton::setPressColor(QColor pressColor)
{
    if (m_pressColor == pressColor)
        return;

    m_pressColor = pressColor;
    emit pressColorChanged(pressColor);
}

void PushButton::setDisableColor(QColor disableColor)
{
    if (m_disableColor == disableColor)
        return;

    m_disableColor = disableColor;
    emit disableColorChanged(disableColor);
}

void PushButton::setCheckedPic(QString checkedPic)
{
    if (m_checkedPic == checkedPic)
        return;

    m_checkedPic = checkedPic;
    emit checkedPicChanged(checkedPic);
}

bool PushButton::event(QEvent *e)
{
    if (e->type() == QEvent::ToolTip) {
        if (QHelpEvent *he = static_cast<QHelpEvent *>(e)) {
            showTooltip(he->globalPos());

            return false;
        }
    }

    return QWidget::event(e);
}

void PushButton::paintEvent(QPaintEvent *e)
{
    QWidget::paintEvent(e);

    QPainter painter(this);

    QMargins m = contentsMargins();
    int ph = 0;
    int spacing = 0;
    QPixmap pixmap(getPixmap());
    if (! pixmap.isNull()) {
        if (pixmap.width() > width() || pixmap.height() > height()) {
            ph = height() - m.top() - m.bottom();
            const QRect pr(m.left(), (height() - ph) / 2, ph, ph);
            painter.drawPixmap(pr, pixmap.scaled(pr.size(), Qt::KeepAspectRatioByExpanding));
        }
        else {
            ph = pixmap.height();
            const QRect pr(m.left(), (height() - ph) / 2, pixmap.width(), ph);
            painter.drawPixmap(pr, pixmap);
        }
        spacing = m_spacing;
    }

    QFontMetrics fm(font());
    const int tw = width() - m.left() - spacing - ph - m.right();
    const int th = fm.height();
    const QRect textRect(m.left() + ph + spacing, (height() - th) / 2,
                   tw, th);
    const QString mt = fm.elidedText(m_text, Qt::ElideMiddle, tw);
    painter.setPen(QPen(getTextColor()));
    painter.drawText(textRect, Qt::AlignCenter, mt);
}

void PushButton::enterEvent(QEvent *e)
{
    Q_UNUSED(e)
    m_currentPic = hoverPic();
    m_currentColor = hoverColor();
    setCursor(Qt::PointingHandCursor);
    this->update();
}

void PushButton::leaveEvent(QEvent *e)
{
    Q_UNUSED(e)
    m_currentColor = normalColor();
    m_currentPic = normalPic();
    setCursor(Qt::ArrowCursor);
    this->update();

    emit mouseLeave();
}

void PushButton::mousePressEvent(QMouseEvent *e)
{
    Q_UNUSED(e)
    m_isPressed = true;
    m_currentColor = pressColor();
    m_currentPic = pressPic();
    this->update();
}

void PushButton::mouseReleaseEvent(QMouseEvent *e)
{
    Q_UNUSED(e)
    m_currentColor = normalColor();
    m_currentPic = normalPic();
    this->update();
    if (m_isPressed) {
        m_isPressed = false;
        emit clicked();
    }
}

QSize PushButton::sizeHint() const
{
    QPixmap p(getPixmap());
    QMargins m = contentsMargins();
    QFontMetrics fm(font());
    int spacing = p.isNull() ? 0 : m_spacing;
    int h = p.height() + m.top() + m.bottom();
    int w = p.width() + fm.width(m_text) + m.left() + m.right() + spacing + 3;

    return QSize(w, qMax(h, fm.height()));
}

QString PushButton::getPixmap() const
{
    if (m_checked) {
        return checkedPic();
    }
    else if (isEnabled()) {
        return m_currentPic;
    }
    else {
        return disablePic();
    }
}

QColor PushButton::getTextColor() const
{
    if (isEnabled()) {
        return m_currentColor;
    }
    else {
        return disableColor();
    }
}

void PushButton::showTooltip(const QPoint &pos)
{
    QFrame *tf = new QFrame();
    tf->setStyleSheet(this->styleSheet());
    tf->setWindowFlags(Qt::ToolTip);
    tf->setAttribute(Qt::WA_TranslucentBackground);
    QLabel *tl = new QLabel(tf);
    tl->setObjectName("ButtonTooltip");
    tl->setText(toolTip());
    QHBoxLayout *layout = new QHBoxLayout(tf);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(tl);

    tf->show();
    QRect dr = qApp->desktop()->geometry();
    int y = pos.y() + tf->height();
    if (y > dr.y() + dr.height()) {
        y = pos.y() - tf->height() - 10;
    }
    tf->move(pos.x() - tf->width()/3, y - tf->height()/3);

    QTimer::singleShot(5000, tf, SLOT(deleteLater()));

    connect(this, &PushButton::mouseLeave, tf, &QFrame::deleteLater);
    connect(this, &PushButton::clicked, tf, &QFrame::deleteLater);
}

void PushButton::onThemeChanged(ViewerThemeManager::AppTheme theme)
{
    QFile darkF(":/resources/dark/qss/PushButton.qss"),
          lightF(":/resources/light/qss/PushButton.qss");
    if (theme == ViewerThemeManager::Dark) {
        if (darkF.open(QIODevice::ReadOnly)) {
            setStyleSheet(darkF.readAll());
            darkF.close();
        } else {
            qDebug() << "Set dark style sheet for PushButton failed";
        }
    } else {
        if (lightF.open(QIODevice::ReadOnly)) {
            setStyleSheet(lightF.readAll());
            lightF.close();
        } else {
            qDebug() << "Set light style sheet for PushButton failed";
        }
    }
}

bool PushButton::getChecked() const
{
    return m_checked;
}

void PushButton::setChecked(bool checked)
{
    m_checked = checked;
    this->update();
}

QString PushButton::checkedPic() const
{
    return m_checkedPic;
}

int PushButton::getSpacing() const
{
    return m_spacing;
}

void PushButton::setSpacing(int spacing)
{
    m_spacing = spacing;
}

