#include "BookMarkItemWidget.h"
#include "utils/utils.h"
#include <DFontSizeManager>

BookMarkItemWidget::BookMarkItemWidget(CustomItemWidget *parent) :
    CustomItemWidget("BookMarkItemWidget", parent)
{
    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(slotShowContextMenu(const QPoint &)));

    initWidget();
}

/**
 * @brief BookMarkItemWidget::dealWithData
 * 处理全局消息接口
 * @return
 */
int BookMarkItemWidget::dealWithData(const int &msgType, const QString &msgContent)
{
    if (MSG_NOTIFY_KEY_MSG == msgType) {
        if (msgContent == KeyStr::g_del) {
            bool bFocus = this->hasFocus();
            if (bFocus) {
                emit sigDeleleteItem(m_nPageIndex);
            }
        }
    }
    return 0;
}

void BookMarkItemWidget::setBSelect(const bool &paint)
{
    if (m_pPicture) {
        m_pPicture->setSelect(paint);
    }
    m_bPaint = paint;
    update();
}

/**
 * @brief BookMarkItemWidget::slotDltBookMark
 * 删除当前书签，并发送删除书签全局消息
 */
void BookMarkItemWidget::slotDltBookMark()
{
    sendMsg(MSG_BOOKMARK_DLTITEM, QString::number(m_nPageIndex));
}

/**
 * @brief BookMarkItemWidget::slotShowContextMenu
 * 显示书签右键菜单
 */
void BookMarkItemWidget::slotShowContextMenu(const QPoint &)
{
    QMenu *t_menu = new QMenu(this);

    QAction *dltBookMarkAction = t_menu->addAction(tr("delete bookmark"));
    connect(dltBookMarkAction, SIGNAL(triggered()), this, SLOT(slotDltBookMark()));
    t_menu->exec(QCursor::pos());
}

/**
 * @brief BookMarkItemWidget::initWidget
 * 初始化界面
 */
void BookMarkItemWidget::initWidget()
{
//    QFont font;
//    font = Utils::getPixFont(QString("SourceHanSansSC-Medium"), 12);
    m_pPageNumber = new PageNumberLabel(this);
    m_pPageNumber->setMinimumWidth(31);
    m_pPageNumber->setFixedHeight(18);
//    m_pPageNumber->setFont(font);
    DFontSizeManager::instance()->bind(m_pPageNumber, DFontSizeManager::T8);

    auto hLine = new DHorizontalLine;
    auto m_pRightVLayout = new QVBoxLayout;

    auto m_pPageVLayout = new QHBoxLayout;
    m_pPageVLayout->setContentsMargins(0, 18, 0, 44);
    m_pPageVLayout->addWidget(m_pPageNumber);

    m_pRightVLayout->addItem(m_pPageVLayout);
    m_pRightVLayout->addWidget(hLine);
    m_pRightVLayout->setContentsMargins(20, 0, 10, 0);

    m_pPicture = new ImageLabel(this);
    m_pPicture->setFixedSize(QSize(48, 68));
    m_pPicture->setAlignment(Qt::AlignCenter);

    auto m_pHLayout = new QHBoxLayout;
    m_pHLayout->setContentsMargins(1, 0, 1, 0);
    m_pHLayout->setSpacing(1);
    m_pHLayout->addWidget(m_pPicture);
    m_pHLayout->addItem(m_pRightVLayout);

    this->setLayout(m_pHLayout);
}

void BookMarkItemWidget::paintEvent(QPaintEvent *event)
{
    QPalette p(m_pPicture->palette());
    QColor color;

    //  涉及到 主题颜色
    if (m_bPaint) {
        color = QColor(QString("#0081FF"));
        p.setColor(QPalette::Text, p.highlight().color());
    } else {
        color = QColor::fromRgbF(0, 0, 0, 0.08);
        p.setColor(QPalette::Text, color);
    }

    m_pPicture->setPalette(p);

//    int width = this->width() - 10;
//    int height = this->height() - 1;

//    QPainter painter(this);
//    color = QColor::fromRgbF(216,216,216,0.0);
//    painter.setPen(QPen(QColor(QString("#D8D8D8")), 1));

//    QPoint startP(67, height);
//    QPoint endP(width, height);

//    painter.drawLine(startP, endP);

    CustomWidget::paintEvent(event);
}

