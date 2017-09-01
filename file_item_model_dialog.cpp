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

#include "file_item_model_dialog.h"
#include "ui_file_item_model_dialog.h"
#include <QDebug>

File_Item_Model_Dialog::File_Item_Model_Dialog(QWidget *parent, QString a) :
    QDialog(parent),
    ui(new Ui::File_Item_Model_Dialog)
{
    ui->setupUi(this);
    model = new QFileSystemModel(this);
    model->setFilter(QDir::AllEntries);
    model->setRootPath("");
    ui->listView->setModel(model);
    this->setModal(true);
    if (!a.isEmpty())
    {
        QDir dir(a);
        FileInfo = model->fileInfo(model->index(dir.path()));
        ui->listView->setRootIndex(model->index(FileInfo.path()));
    }
    else
    {
        ui->listView->setRootIndex(model->index(QDir::homePath()));
    }
}

File_Item_Model_Dialog::~File_Item_Model_Dialog()
{
    this->setModal(false);
    delete ui;
}

void File_Item_Model_Dialog::on_listView_doubleClicked(const QModelIndex &index)
{
    QListView *ListView = (QListView *)sender();
    FileInfo = model->fileInfo(index);
    if (FileInfo.fileName() == "..")
    {
        QDir dir = FileInfo.dir();
        dir.cdUp();
        ListView->setRootIndex(model->index(dir.path()));
        ui->lineEdit_2->setText(dir.path());
    }
    else if (FileInfo.fileName() == ".")
    {
        QDir dir = QDir::homePath();
        ListView->setRootIndex(model->index(dir.path()));
        ui->lineEdit_2->setText(dir.path());
    }
    else if (FileInfo.isDir())
    {
        QDir dir(FileInfo.path());
        dir.cd(FileInfo.fileName());
        ListView->setRootIndex(index);
        ui->lineEdit_2->setText(dir.path());
    }
}

void File_Item_Model_Dialog::on_listView_clicked(const QModelIndex &index)
{
    FileInfo = model->fileInfo(index);
    if (FileInfo.isFile())
    {
        ui->lineEdit_2->setText(FileInfo.path());
        ui->lineEdit->setText(FileInfo.fileName());
    }
}



void File_Item_Model_Dialog::on_buttonBox_accepted()
{
    QString path = ui->lineEdit_2->text() + "/" + ui->lineEdit->text();
    emit FilePath(path);
}
