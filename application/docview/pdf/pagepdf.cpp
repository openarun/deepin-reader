#include "pagepdf.h"
#include <QPainter>
#include <QDebug>

PagePdf::PagePdf(QWidget *parent)
    : PageBase(parent),
      m_imagewidth(0.01),
      m_imageheight(0.01),
      m_page(nullptr),
      m_paintercolor(QColor(72, 118, 255, 100)),
      m_pencolor (QColor(72, 118, 255, 0)),
      m_penwidth(0)
{
}

void PagePdf::paintEvent(QPaintEvent *event)
{
    QLabel::paintEvent(event);
    QPainter qpainter(this);
    qpainter.setBrush(m_paintercolor);
    QPen qpen(m_pencolor, m_penwidth);
    qpainter.setPen(qpen);
    for (int i = 0; i < paintrects.size(); i++) {
        qpainter.drawRect(paintrects[i]);
    }
}

void PagePdf::clearPageTextSelections()
{
    if (paintrects.size() > 0) {
        paintrects.clear();
        update();
    }
}

void PagePdf::setPage(Poppler::Page *page)
{
    m_page = page;
    m_imagewidth = m_page->pageSizeF().width();
    m_imageheight = m_page->pageSizeF().height();
}
bool PagePdf::showImage(double scale, RotateType_EM rotate)
{
    if (!m_page)
        return false;
    int xres = 72.0, yres = 72.0;
    m_scale = scale;
    m_rotate = rotate;
    QImage image = m_page->renderToImage(xres * scale, yres * scale, m_imagewidth * scale, m_imageheight * scale);
    QPixmap map = QPixmap::fromImage(image);
    QMatrix leftmatrix;
    switch (rotate) {
    case RotateType_90:
        leftmatrix.rotate(90);
        break;
    case RotateType_180:
        leftmatrix.rotate(180);
        break;
    case RotateType_270:
        leftmatrix.rotate(270);
        break;
    default:
        leftmatrix.rotate(0);
        break;
    }
    setPixmap(map.transformed(leftmatrix, Qt::SmoothTransformation));
    return true;
}

bool PagePdf::getImage(QImage &image, double width, double height, RotateType_EM rotate)
{
    if (!m_page)
        return false;
    int xres = 72.0, yres = 72.0;
    double scalex = width / m_imagewidth;
    double scaley = height / m_imageheight;
    image = m_page->renderToImage(xres * scalex, yres * scaley, width, height, (Poppler::Page::Rotation)rotate);
    return true;
}

bool PagePdf::setSelectTextStyle(QColor paintercolor, QColor pencolor, int penwidth)
{
    m_paintercolor = paintercolor;
    m_pencolor = pencolor;
    m_penwidth = penwidth;
    update();
    return true;
}

bool PagePdf::pageTextSelections(const QPoint start, const QPoint end)
{
    qDebug() << "pageTextSelections start:" << start << " end:" << end;
    qDebug() << "pageTextSelections x():" << x() << " y()" << y();
    QPoint startC = QPoint(start.x() - x() - (width() - m_scale * m_imagewidth) / 2, start.y() - y() - (height() - m_scale * m_imageheight) / 2);
    QPoint endC = QPoint(end.x() - x() - (width() - m_scale * m_imagewidth) / 2, end.y() - y() - (height() - m_scale * m_imageheight) / 2);
    qDebug() << "startC1:" << startC << " endC1:" << endC;
    switch (m_rotate) {
    case RotateType_90:
        startC = QPoint((start.x() - x() - (width() - m_scale * m_imageheight) / 2), (start.y() - y() - (height() - m_scale * m_imagewidth) / 2));
        startC = QPoint(startC.y(), m_scale * m_imageheight - startC.x());
        endC = QPoint((end.x() - x() - (width() - m_scale * m_imageheight) / 2), (end.y() - y() - (height() - m_scale * m_imagewidth) / 2));
        endC = QPoint(endC.y(), m_scale * m_imageheight - endC.x());
        break;
    case RotateType_180:
        startC = QPoint(m_scale * m_imagewidth - startC.x(), m_scale * m_imageheight - startC.y());
        endC = QPoint(m_scale * m_imagewidth - endC.x(), m_scale * m_imageheight - endC.y());
        break;
    case RotateType_270:
        startC = QPoint((start.x() - x() - (width() - m_scale * m_imageheight) / 2), (start.y() - y() - (height() - m_scale * m_imagewidth) / 2));
        startC = QPoint(m_scale * m_imagewidth - startC.y(), startC.x());
        endC = QPoint((end.x() - x() - (width() - m_scale * m_imageheight) / 2), (end.y() - y() - (height() - m_scale * m_imagewidth) / 2));
        endC = QPoint(m_scale * m_imagewidth - endC.y(), endC.x());
        break;
    default:
        break;
    }
    qDebug() << "startC:" << startC << " endC:" << endC;
    QPoint temp;
    if (startC.x() > endC.x()) {
        temp = startC;
        startC = endC;
        endC = temp;
    }

    const QRect start_end = (startC.y() < endC.y())
                            ? QRect(startC.x(), startC.y(), endC.x(), endC.y())
                            : QRect(startC.x(), endC.y(), endC.x(), startC.y());

    QRectF tmp;
    int startword = 0, stopword = -1;
    qDebug() << "page width:" << width() << " height:" << height() << " m_imagewidth:" << m_imagewidth << " m_imageheight:" << m_imageheight;
//    const double scaleX = width() / m_imagewidth;
//    const double scaleY = height() / m_imageheight;
    const double scaleX = m_scale;
    const double scaleY = m_scale;
    qDebug() << "m_words size:" << m_words.size();
    for (int i = 0; i < m_words.size(); i++) {
//        qDebug() << "m_words i:" << i << " rect:" << m_words.at(i).rect;
        tmp = m_words.at(i).rect;
        if (startC.x() > (tmp.x() * m_scale) &&
                startC.x() < (tmp.x() * scaleX + tmp.width() * scaleX) &&
                startC.y() > (tmp.y() * scaleY) &&
                startC.y() < (tmp.y() * scaleY + tmp.height() * scaleY)) {
            startword = i;
        }
        if (endC.x() > (tmp.x() * scaleX ) &&
                endC.x() < (tmp.x() * scaleX + tmp.width() * scaleX ) &&
                endC.y() > (tmp.y() * scaleY ) &&
                endC.y() < (tmp.y() * scaleY + tmp.height() * scaleY)) {
            stopword = i;
        }
    }
    qDebug() << " startword:" << startword << " stopword:" << stopword;
    if (-1 == startword && stopword == -1) {
        int i;
        for (i = 0; i < m_words.size(); i++) {
            tmp = m_words.at(i).rect;
            if (start_end.intersects(QRect(tmp.x() * scaleX,
                                           tmp.y() * scaleY, tmp.width() * scaleX,
                                           tmp.height() * scaleY))) {
//                qDebug() << "break i:" << i;
                break;
            }
        }

        if (i == m_words.size()) {
            return false;
        }
    }
    bool selection_two_start = false;
    if (startword == 0) {
        QRectF rect;
        if (startC.y() <= endC.y()) {
            for (int i = 0; i < m_words.size(); i++) {
                tmp = m_words.at(i).rect;
                rect = QRect(tmp.x() * scaleX, tmp.y() * scaleY,
                             tmp.width() * scaleX, tmp.height() * scaleY);
                if (rect.y() > startC.y() && rect.x() > startC.x()) {
                    startword = i;
                    break;
                }
            }
        } else {
            selection_two_start = true;
            int distance = scaleX + scaleY + 100;
            int count = 0;

            for (int i = 0; i < m_words.size(); i++) {
                tmp = m_words.at(i).rect;
                rect = QRect(tmp.x() * scaleX, tmp.y() * scaleY,
                             tmp.width() * scaleX, tmp.height() * scaleY);

                if ((rect.y() + rect.height()) < startC.y() &&
                        (rect.x() + rect.height()) < startC.x()) {
                    count++;
                    int xdist, ydist;
                    xdist = rect.center().x() - startC.x();
                    ydist = rect.center().y() - startC.y();

                    if (xdist < 0)
                        xdist = -xdist;
                    if (ydist < 0)
                        ydist = -ydist;

                    if ((xdist + ydist) < distance) {
                        distance = xdist + ydist;
                        startword = i;
                    }
                }
            }
        }
    }
    if (stopword == -1) {
        QRectF rect;

        if (startC.y() <= endC.y()) {
            for (int i = m_words.size() - 1; i >= 0; i--) {
                tmp = m_words.at(i).rect;
                rect = QRect(tmp.x() * scaleX, tmp.y() * scaleY,
                             tmp.width() * scaleX, tmp.height() * scaleY);

                if ((rect.y() + rect.height()) < endC.y() && (rect.x() + rect.width()) < endC.x()) {
                    stopword = i;
                    break;
                }
            }
        }

        else {
            int distance = scaleX + scaleY + 100;
            for (int i = m_words.size() - 1; i >= 0; i--) {
                tmp = m_words.at(i).rect;
                rect = QRect(tmp.x() * scaleX, tmp.y() * scaleY,
                             tmp.width() * scaleX, tmp.height() * scaleY);
                if (rect.y() > endC.y() && rect.x() > endC.x()) {
                    int xdist, ydist;
                    xdist = rect.center().x() - endC.x();
                    ydist = rect.center().y() - endC.y();

                    if (xdist < 0)
                        xdist = -xdist;
                    if (ydist < 0)
                        ydist = -ydist;

                    if ((xdist + ydist) < distance) {
                        distance = xdist + ydist;
                        stopword = i;
                    }
                }
            }
        }
    }
    if (-1 == stopword)
        return false;
    if (selection_two_start) {
        if (startword > stopword) {
            startword = startword - 1;
        }
    }
    if (startword > stopword) {
        int im = startword;
        startword = stopword;
        stopword = im;
    }
    paintrects.clear();
    tmp = m_words.at(startword).rect;
    for (int i = startword + 1; i <= stopword; i++) {
        QRectF tmpafter;
        tmpafter = m_words.at(i).rect;
        if ((abs(tmp.y() - tmpafter.y()) < tmp.height() / 5 ||
                abs(tmp.y() + tmp.height() / 2 - tmpafter.y() + tmpafter.height() / 2) <
                tmp.height() / 5) &&
                abs(tmp.x() + tmp.width() - tmpafter.x()) < tmp.width() / 5) {
            if (tmpafter.y() < tmp.y()) {
                tmp.setY(tmpafter.y());
            }
            if (tmpafter.height() > tmp.height()) {
                tmp.setHeight(tmpafter.height());
            }
            tmp.setWidth(tmpafter.x() + tmpafter.width() - tmp.x());
        } else {
            QRect paintrect = QRect(tmp.x() * scaleX + (width() - m_scale * m_imagewidth) / 2, tmp.y() * scaleY + (height() - m_scale * m_imageheight) / 2, tmp.width() * scaleX,
                                    tmp.height() * scaleY);
            switch (m_rotate) {
            case RotateType_90:
                paintrect = QRect(tmp.x() * scaleX + (height() - m_scale * m_imagewidth) / 2, tmp.y() * scaleY + (width() - m_scale * m_imageheight) / 2, tmp.width() * scaleX,
                                  tmp.height() * scaleY);
                paintrect = QRect(width() - paintrect.y() - paintrect.height(), paintrect.x(), paintrect.height(), paintrect.width());
                break;
            case RotateType_180:
                paintrect = QRect(width() - paintrect.x() - paintrect.width(), height() - paintrect.y() - paintrect.height(), paintrect.width(), paintrect.height());
                break;
            case RotateType_270:
                paintrect = QRect(tmp.x() * scaleX + (height() - m_scale * m_imagewidth) / 2, tmp.y() * scaleY + (width() - m_scale * m_imageheight) / 2, tmp.width() * scaleX,
                                  tmp.height() * scaleY);
                paintrect = QRect(paintrect.y(), height() - paintrect.x() - paintrect.width(), paintrect.height(), paintrect.width());
                break;
            default:
                break;
            }
            paintrects.append(paintrect);
            tmp = tmpafter;
        }
    }
    QRect paintrect = QRect(tmp.x() * scaleX + (width() - m_scale * m_imagewidth) / 2, tmp.y() * scaleY + (height() - m_scale * m_imageheight) / 2, tmp.width() * scaleX, tmp.height() * scaleY);
    switch (m_rotate) {
    case RotateType_90:
        paintrect = QRect(tmp.x() * scaleX + (height() - m_scale * m_imagewidth) / 2, tmp.y() * scaleY + (width() - m_scale * m_imageheight) / 2, tmp.width() * scaleX,
                          tmp.height() * scaleY);
        paintrect = QRect(width() - paintrect.y() - paintrect.height(), paintrect.x(), paintrect.height(), paintrect.width());
        break;
    case RotateType_180:
        paintrect = QRect(width() - paintrect.x() - paintrect.width(), height() - paintrect.y() - paintrect.height(), paintrect.width(), paintrect.height());
        break;
    case RotateType_270:
        paintrect = QRect(tmp.x() * scaleX + (height() - m_scale * m_imagewidth) / 2, tmp.y() * scaleY + (width() - m_scale * m_imageheight) / 2, tmp.width() * scaleX,
                          tmp.height() * scaleY);
        paintrect = QRect(paintrect.y(), height() - paintrect.x() - paintrect.width(), paintrect.height(), paintrect.width());
        break;
    default:
        break;
    }
    paintrects.append(paintrect);
    update();
    return true;
}

bool PagePdf::ifMouseMoveOverText(const QPoint point)
{
//    QPoint qpoint = QPoint(point.x() - (width() - m_scale * m_imagewidth) / 2, point.y() - (height() - m_scale * m_imageheight) / 2);
//    const double scaleX = width() / m_imagewidth;
//    const double scaleY = height() / m_imageheight;
    const double scaleX = m_scale;
    const double scaleY = m_scale;
    QPoint qp = QPoint((point.x() - x() - (width() - m_scale * m_imagewidth) / 2) / scaleX, (point.y() - y() - (height() - m_scale * m_imageheight) / 2) / scaleY);
//    qDebug() << "point:" << point;
//    qDebug() << "qp1:" << qp;
    switch (m_rotate) {
    case RotateType_90:
        qp = QPoint((point.x() - x() - (width() - m_scale * m_imageheight) / 2) / scaleX, (point.y() - y() - (height() - m_scale * m_imagewidth) / 2) / scaleY);
        qp = QPoint(qp.y(), m_imageheight - qp.x());
        break;
    case RotateType_180:
        qp = QPoint(m_imagewidth - qp.x(),  m_imageheight - qp.y());
        break;
    case RotateType_270:
        qp = QPoint((point.x() - x() - (width() - m_imageheight) / 2) / scaleX, (point.y() - y() - (height() - m_scale * m_imagewidth) / 2) / scaleY);
        qp = QPoint(m_imagewidth - qp.y(), qp.x());
        break;
    default:
        break;
    }
//    qDebug() << "------this rect x:" << x() << " y:" << y() << " width:" << width() << " height:" << height();
//    qDebug() << "qp:" << qp;
    for (int i = 0; i < m_words.size(); i++) {
        if (qp.x() > m_words.at(i).rect.x() &&
                qp.x() < m_words.at(i).rect.x() + m_words.at(i).rect.width() &&
                qp.y() > m_words.at(i).rect.y() &&
                qp.y() < m_words.at(i).rect.y() + m_words.at(i).rect.height()) {
            qDebug() << "rect:" << m_words.at(i).rect;
            return true;
        }
    }
    return false;
}

void PagePdf::appendWord(stWord word)
{
    m_words.append(word);
}

void PagePdf::setImageWidth(double width)
{
    m_imagewidth = width;
}
void PagePdf::setImageHeight(double height)
{
    m_imageheight = height;
}

void PagePdf::addHighlightAnnotation(const QList<QRectF> &listrect, const QColor &color)
{
    qDebug()<<"*************"<<listrect.size();
    if(listrect.size()<=0)return;
    Poppler::Annotation::Style style;
    style.setColor(color);

    Poppler::Annotation::Popup popup;
    popup.setFlags(Poppler::Annotation::Hidden | Poppler::Annotation::ToggleHidingOnMouse);

    Poppler::HighlightAnnotation* annotation = new Poppler::HighlightAnnotation();

    Poppler::HighlightAnnotation::Quad quad;
    QList<Poppler::HighlightAnnotation::Quad> qlistquad;
    QRectF rec,recboundary;
    foreach(rec,listrect)
    {
        recboundary.setTopLeft(QPointF(rec.left()/m_page->pageSizeF().width(),
                                   rec.top()/m_page->pageSizeF().height()));
        recboundary.setTopRight(QPointF(rec.right()/m_page->pageSizeF().width(),
                                    rec.top()/m_page->pageSizeF().height()));
        recboundary.setBottomLeft(QPointF(rec.left()/m_page->pageSizeF().width(),
                                      rec.bottom()/m_page->pageSizeF().height()));
        recboundary.setBottomRight(QPointF(rec.right()/m_page->pageSizeF().width(),
                                       rec.bottom()/m_page->pageSizeF().height()));


        qDebug()<<"**"<<rec<<"**";
        quad.points[0] = recboundary.topLeft();
        quad.points[1] = recboundary.topRight();
        quad.points[2] = recboundary.bottomRight();
        quad.points[3] = recboundary.bottomLeft();
        qlistquad.append(quad);
    }
    annotation->setHighlightQuads(qlistquad);
    annotation->setBoundary(recboundary);
    annotation->setStyle(style);
    annotation->setPopup(popup);
    m_page->addAnnotation(annotation);
    qDebug()<<"addHighlightAnnotation"<<annotation->boundary();
}

void PagePdf::removeAnnotation(Poppler::Annotation *annotation)
{
    m_page->removeAnnotation(annotation);
}
