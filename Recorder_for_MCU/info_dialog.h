/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt Charts module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef INFO_DIALOG_H
#define INFO_DIALOG_H

#include <QDialog>
#include <QString>
#include "ui_info_dialog.h"
#include "file_item_model_dialog.h"

namespace Ui {
class Info_Dialog;
}

class Info_Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Info_Dialog(QWidget *parent = 0);
    ~Info_Dialog();
    Ui::Info_Dialog *ui;
    File_Item_Model_Dialog *item;

public slots:
    void button(bool t);
    int is_number_graph();
    int is_time_interval();
    void boot();
    int is_take();

private:

signals:
    void start();
    void stop();
private slots:
    void on_save_pushButton_clicked();
    void is_port();
};

#endif // INFO_DIALOG_H
