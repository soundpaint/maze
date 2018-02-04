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

#ifndef ABOUT_DIALOG_HH
#define ABOUT_DIALOG_HH

#include <QtGui/QTextDocument>
#include <QtWidgets/QAction>
#include <QtWidgets/QDialog>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QVBoxLayout>

class About_dialog : public QDialog
{
  Q_OBJECT
public:
  explicit About_dialog(QWidget *parent);
  virtual ~About_dialog();
private:
  QVBoxLayout *_layout;
  QTextEdit *_about_text;
  QTextDocument *_about_document;
  QAction *_action_done;
  QDialogButtonBox *_button_box;
  void create_actions();
};

#endif /* ABOUT_DIALOG_HH */

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
