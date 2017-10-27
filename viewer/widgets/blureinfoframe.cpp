#include "blureinfoframe.h"
#include "separator.h"
#include "utils/baseutils.h"
#include "widgets/imagebutton.h"
#include <dwindowclosebutton.h>
#include <QLabel>
#include <QEvent>

namespace {

const int MAX_FIELD_WIDTH = 140;
const int TITLE_MAXWIDTH = 80;
}

class SimpleFormLabel : public QLabel {
    Q_OBJECT
public:
    explicit SimpleFormLabel(const QString &t, QWidget *parent = 0)
        : QLabel(t, parent) {}
};

class SimpleFormField : public QLabel {
    Q_OBJECT
public:
    explicit SimpleFormField(QWidget *parent = 0)
        : QLabel(parent) {
        setWordWrap(true);
    }
protected:
    void resizeEvent(QResizeEvent* event) {
        if (wordWrap()&&sizePolicy().verticalPolicy() == QSizePolicy::Minimum ) {
            // heightForWidth rely on minimumSize to evaulate, so reset it before
            setMinimumHeight(0);
            // define minimum height
            setMinimumHeight(heightForWidth(width()));
        }
        QLabel::resizeEvent(event);
    }
};

BlureInfoFrame::BlureInfoFrame(QWidget *parent)
    : BlurFrame(parent)
{
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlags(Qt::Sheet | Qt::FramelessWindowHint);
//    setWindowModality(Qt::WindowModal);

    setBorderRadius(4);
    setBorderWidth(1);
    setBorderColor(QColor(255, 255, 255, 51));

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(5, 5, 5, 5);

    QFrame *tf = new QFrame;
    tf->setObjectName("InfoTopContent");
    tf->setContentsMargins(0, 0, 0, 0);
    m_topLayout = new QVBoxLayout(tf);
    m_topLayout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(tf);

    Separator *s = new Separator;
    s->setFixedHeight(1);
    layout->addSpacing(5);
    layout->addWidget(s);
    layout->addSpacing(5);

    m_infoFrame = new QFrame;
    m_infoLayout = new QFormLayout(m_infoFrame);
    m_infoLayout->setSpacing(3);
    m_infoLayout->setContentsMargins(10, 0, 10, 0);

    m_infoLayout->setLabelAlignment(Qt::AlignRight);
    layout->addWidget(m_infoFrame, 1, Qt::AlignCenter);
    layout->addSpacing(8);
    layout->addStretch(1);

    setStyleSheet(utils::base::getFileContent(
                      ":/resources/common/qss/BlureInfoFrame.qss"));
}

void BlureInfoFrame::setTopContent(QWidget *w)
{
    QLayoutItem *item;
    while (( item = m_topLayout->takeAt(0))) {
        if (item->widget())
            delete item->widget();
        delete item;
    }
    m_topLayout->addWidget(w);

    ImageButton* cb = new ImageButton(this);
    cb->setTooltipVisible(true);
    cb->setNormalPic(":/resources/common/images/closewindow_normal.png");
    cb->setHoverPic(":/resources/common/images/closewindow_hover.png");
    cb->setPressPic(":/resources/common/images/closewindow_press.png");
    cb->setFixedSize(24, 24);
    cb->move(this->sizeHint().width() - cb->width() - 5, 0);
    connect(cb, &ImageButton::clicked, this, &BlureInfoFrame::close);

    connect(cb, &Dtk::Widget::DWindowCloseButton::clicked,
            this, &BlureInfoFrame::close);
    this->setFixedWidth(w->width() + 6*2);
}

void BlureInfoFrame::addInfoPair(const QString &title, const QString &value)
{
    SimpleFormField *vl = new SimpleFormField;
    vl->setAlignment(Qt::AlignLeft|Qt::AlignTop);
    vl->setText(utils::base::wrapStr(value, vl->font(), MAX_FIELD_WIDTH));

    SimpleFormLabel *tl = new SimpleFormLabel(title);
    tl->setMinimumHeight(vl->minimumHeight());
    tl->setFixedWidth(qMin(tl->width(), TITLE_MAXWIDTH));
    tl->setAlignment(Qt::AlignRight|Qt::AlignTop);

    m_infoLayout->addRow(tl, vl);
    m_infoFrame->setFixedWidth(qMin(vl->width() + tl->width(), width() - 16));
}

void BlureInfoFrame::close()
{
    emit closed();
    BlurFrame::close();
}

void BlureInfoFrame::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Escape) {
        this->close();
        return;
    }
    BlurFrame::keyPressEvent(e);
}
