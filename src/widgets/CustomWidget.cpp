/*
 * Copyright (C) 2019 ~ 2020 Uniontech Software Technology Co.,Ltd.
 *
 * Author:     duanxiaohui<duanxiaohui@uniontech.com>
 *
 * Maintainer: duanxiaohui<duanxiaohui@uniontech.com>
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
#include "CustomWidget.h"

#include <DWidgetUtil>

CustomWidget::CustomWidget(DWidget *parent)
    : DWidget(parent)
{
    setWindowFlags(Qt::FramelessWindowHint);
    setFocusPolicy(Qt::StrongFocus);
    setAutoFillBackground(true);
    setContextMenuPolicy(Qt::CustomContextMenu);//让widget支持右键菜单事件
}

CustomWidget::~CustomWidget()
{

}

void CustomWidget::initWidget()
{

}

void CustomWidget::updateWidgetTheme()
{
    auto plt = Dtk::Gui::DGuiApplicationHelper::instance()->applicationPalette();
    plt.setColor(Dtk::Gui::DPalette::Background, plt.color(Dtk::Gui::DPalette::Base));
    setPalette(plt);
}

void CustomWidget::adaptWindowSize(const double &)
{

}

void CustomWidget::updateThumbnail(const int &)
{

}

void CustomWidget::showMenu()
{

}
