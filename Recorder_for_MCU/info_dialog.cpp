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

#include "info_dialog.h"
#include "ui_info_dialog.h"
#include <QtSerialPort>
#include <QSerialPortInfo>
#include <QList>

Info_Dialog::Info_Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Info_Dialog)
{
    this->setWindowTitle("settings");
    ui->setupUi(this);
    QObject::connect(this->ui->pushButton_2, &QPushButton::clicked, this, &Info_Dialog::is_port);
    this->is_port();
    ui->comboBox->setFocus();
    bool t = true;
    ui->start_pushButton->setEnabled(t);
    ui->stop_pushButton->setEnabled(!t);
    ui->number_spinBox->setMinimum(1);
    ui->number_spinBox->setMaximum(10);

}

Info_Dialog::~Info_Dialog()
{
    delete ui;
}


void Info_Dialog::button(bool t)
{
    ui->start_pushButton->setEnabled(t);
    ui->stop_pushButton->setEnabled(!t);
    ui->save_pushButton->setEnabled(t);
    ui->comboBox->setEnabled(t);
    ui->number_spinBox->setEnabled(t);
    ui->time_interval_lineEdit->setEnabled(t);
    ui->to_values_radioButton->setEnabled(t);
    ui->to_time_radioButton_2->setEnabled(t);
    ui->pushButton_2->setEnabled(t);
    ui->frame->setEnabled(t);
    if(!ui->start_pushButton->isEnabled())
    {
        ui->auto_checkBox->setEnabled(t);
        ui->range_checkBox->setEnabled(t);
        ui->range_spinBox->setEnabled(t);
    }
    else
    {
        ui->auto_checkBox->setEnabled(ui->to_values_radioButton->isChecked());
        ui->range_checkBox->setEnabled(ui->to_time_radioButton_2->isChecked());
        ui->range_spinBox->setEnabled(ui->to_time_radioButton_2->isChecked());
    }
}

int Info_Dialog::is_number_graph()
{
    return ui->number_spinBox->value();
}

int Info_Dialog::is_time_interval()
{
    return ui->time_interval_lineEdit->text().toInt();
}

void Info_Dialog::boot()
{
    if(!this->isVisible())
    {
        this->show();
    }
    else
    {
        this->hide();
    }
}

int Info_Dialog::is_take()
{
    return ui->all_radioButton->isChecked() + 2 * ui->last_radioButton->isChecked() + 3 * ui->mean_radioButton->isChecked();
}



void Info_Dialog::is_port()
{
    const auto infos = QSerialPortInfo::availablePorts();
    this->ui->comboBox->clear();
    for (const QSerialPortInfo &info : infos)
        this->ui->comboBox->addItem(info.portName());
    if(!(this->ui->comboBox->size().isNull()))
        this->ui->comboBox->setCurrentIndex(0);
}
