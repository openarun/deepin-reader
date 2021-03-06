/*
* Copyright (C) 2019 ~ 2020 Uniontech Software Technology Co.,Ltd.
*
* Author:     zhangsong<zhangsong@uniontech.com>
*
* Maintainer: zhangsong<zhangsong@uniontech.com>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef BROWSERLINK_H
#define BROWSERLINK_H

#include "document/Model.h"

#include <QGraphicsItem>
#include <QRectF>

class BrowserLink : public QGraphicsItem
{
public:
    explicit BrowserLink(QGraphicsItem *parent, deepin_reader::Link link);

    ~BrowserLink() override;

private:
    deepin_reader::Link m_link;
};

#endif // BROWSERLINK_H
