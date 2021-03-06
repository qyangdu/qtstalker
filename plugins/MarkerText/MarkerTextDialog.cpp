/*
 *  Qtstalker stock charter
 *
 *  Copyright (C) 2001-2010 Stefan S. Stratigakos
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307,
 *  USA.
 */

#include "MarkerTextDialog.h"
#include "Util.h"

#include <QDebug>

MarkerTextDialog::MarkerTextDialog (QHash<QString, void *> l, QString name) : Dialog (0, name)
{
  QStringList tl;
  tl << QString("OTA") << QString("-") << tr("Editing Marker Text");
  setWindowTitle(tl.join(" "));

  tl.clear();
  QDir dir(QDir::homePath());
  tl << dir.absolutePath() << QString("OTA") << QString("MarkerText") << QString("settings") << QString("dialog");
  _settingsPath = tl.join("/");

  Util util;
  _color = util.object(QString("ColorButton"), QString(), QString("color"));
  _font = util.object(QString("FontButton"), QString(), QString("font"));
  
  createTab(l);
  loadSettings();
}

MarkerTextDialog::~MarkerTextDialog ()
{
  saveSettings();
  
  if (_color)
    delete _color;
  
  if (_font)
    delete _font;
}

void
MarkerTextDialog::createTab (QHash<QString, void *> l)
{
  QWidget *w = new QWidget;
  
  QFormLayout *form = new QFormLayout;
  form->setSpacing(2);
  form->setMargin(10);
  w->setLayout(form);

  // plot
  _plot = new QComboBox;
  connect(_plot, SIGNAL(currentIndexChanged(int)), this, SLOT(modified()));

  QStringList tl;
  QHashIterator<QString, void *> it(l);
  while (it.hasNext())
  {
    it.next();
    Object *o = (Object *) it.value();
    if (o->plugin() == QString("Plot"))
      tl << it.key();
  }
  if (tl.size())
  {
    _plot->addItems(tl);
    form->addRow(tr("Plot"), _plot);
  }
  
  // color
  if (_color)
  {
    connect(_color, SIGNAL(signalMessage(ObjectCommand)), this, SLOT(modified()));
    form->addRow(tr("Color"), _color->widget());
  }

  // font
  if (_font)
  {
    connect(_font, SIGNAL(signalMessage(ObjectCommand)), this, SLOT(modified()));
    form->addRow(tr("Font"), _font->widget());
  }
  
  // date
  _date = new QDateTimeEdit;
  _date->setCalendarPopup(TRUE);
  connect(_date, SIGNAL(dateTimeChanged(const QDateTime &)), this, SLOT(modified()));
  form->addRow(tr("Date"), _date);
  
  // price
  _price = new QDoubleSpinBox;
  _price->setRange(-99999999.0, 99999999.0);
  _price->setDecimals(4);
  connect(_price, SIGNAL(valueChanged(double)), this, SLOT(modified()));
  form->addRow(tr("Price"), _price);

  // text
  _text = new QLineEdit;
  connect(_text, SIGNAL(textEdited(const QString &)), this, SLOT(modified()));
  form->addRow(tr("Text"), _text);

  _tabs->addTab(w, tr("Settings"));
}

void
MarkerTextDialog::done ()
{
  emit signalDone((void *) this);
  Dialog::done();
}

void
MarkerTextDialog::loadSettings ()
{
  QSettings settings(_settingsPath, QSettings::NativeFormat);
  Dialog::loadSettings(settings);
}

void
MarkerTextDialog::saveSettings ()
{
  QSettings settings(_settingsPath, QSettings::NativeFormat);
  Dialog::saveSettings(settings);
}
void
MarkerTextDialog::setSettings (QColor c, QDateTime d, double p, QString t, QFont f, QString po)
{
  if (_color)
  {
    ObjectCommand toc(QString("set_color"));
    toc.setValue(QString("color"), c);
    _color->message(&toc);
  }
  
  if (_font)
  {
    ObjectCommand toc(QString("set_font"));
    toc.setValue(QString("font"), f);
    _font->message(&toc);
  }
  
  _date->setDateTime(d);
  _price->setValue(p);
  _text->setText(t);

  int pos = _plot->findText(po);
  if (pos == -1)
    pos = 0;
  _plot->setCurrentIndex(pos);
}

void
MarkerTextDialog::settings (QColor &c, QDateTime &d, double &p, QString &t, QFont &f, QString &po)
{
  if (_color)
  {
    QString key("color");
    ObjectCommand toc(key);
    if (_color->message(&toc))
      c = toc.getColor(key);
  }
  
  if (_font)
  {
    QString key("font");
    ObjectCommand toc(key);
    if (_font->message(&toc))
      f = toc.getFont(key);
  }
  
  d = _date->dateTime();
  p = _price->value();
  t = _text->text();
  po = _plot->currentText();
}
