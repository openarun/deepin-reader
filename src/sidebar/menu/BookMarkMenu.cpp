/*
 * Copyright (C) 2019 ~ 2020 Uniontech Software Technology Co.,Ltd.
 *
 * Author:     wangzhixuan<wangzhixuan@uniontech.com>
 *
 * Maintainer: wangzhixuan<wangzhixuan@uniontech.com>
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
#include "BookMarkMenu.h"

#include <QDebug>

BookMarkMenu::BookMarkMenu(DWidget *parent)
    : CustomMenu(parent)
{
    initActions();
}

void BookMarkMenu::initActions()
{
    QAction *dltBookMarkAction = this->addAction(tr("Remove bookmark"));
    connect(dltBookMarkAction, SIGNAL(triggered()), SLOT(slotDelete()));

    QAction *dltAllBookMarkAction = this->addAction(tr("Remove all"));
    connect(dltAllBookMarkAction, SIGNAL(triggered()), SLOT(slotDeleteAll()));
}

void BookMarkMenu::slotDelete()
{
    emit sigClickAction(E_BOOKMARK_DELETE);
}

void BookMarkMenu::slotDeleteAll()
{
    emit sigClickAction(E_BOOKMARK_DELETE_ALL);
}
