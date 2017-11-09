/*
 * Maze -- A maze / flipper game implementation for RPi with Sense Hat
 * Copyright (C) 2016, 2017  Jürgen Reuter
 *
 * This program is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see
 * <https://www.gnu.org/licenses/>.
 *
 * For updates and more info or contacting the author, visit:
 * <https://github.com/soundpaint/maze>
 *
 * Author's web site: www.juergen-reuter.de
 */

#include <license-dialog.hh>
#include <QtCore/QSizeF>
#include <log.hh>
#include <COPYING.h>

License_dialog::License_dialog(QWidget *parent) : QDialog(parent)
{
  setWindowTitle(QString(tr("License")));
  _layout = new QVBoxLayout();
  if (!_layout) {
    Log::fatal("License_dialog::License_dialog(): not enough memory");
  }
  setLayout(_layout);

  _license_text = new QTextEdit(this);
  if (!_license_text) {
    Log::fatal("License_dialog::License_dialog(): not enough memory");
  }
  _license_text->setReadOnly(true);
  _license_text->setDocumentTitle(QString(tr("License")));
  _license_text->setMinimumSize(730, 400);

  _license_document = new QTextDocument(_license_text);
  if (!_license_document) {
    Log::fatal("License_dialog::License_dialog(): not enough memory");
  }
  _license_document->setHtml(QString((const char *)COPYING_html));

  _license_text->setDocument(_license_document);
  _layout->addWidget(_license_text);

  _button_box = new QDialogButtonBox(QDialogButtonBox::Ok);
  if (!_button_box) {
    Log::fatal("License_dialog::License_dialog(): not enough memory");
  }
  _layout->addWidget(_button_box);

  create_actions();
}

License_dialog::~License_dialog()
{
  // Q objects will be deleted by Qt, just set them to 0
  _action_done = 0;
  _license_document = 0;
  _license_text = 0;
  _button_box = 0;
}

void
License_dialog::create_actions()
{
  connect(_button_box,
	  SIGNAL(accepted()),
	  this,
	  SLOT(accept()));
  connect(_button_box,
	  SIGNAL(rejected()),
	  this,
	  SLOT(reject()));
}

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
