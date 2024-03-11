#ifndef HOME_H
#define HOME_H

#include "ui_home.h"
#include "util.h"

class Home : public QMainWindow
{
  Q_OBJECT
  friend class Log;
  friend class Mod;
  friend class New;

private:
  type typ = type::NONE;
  Ui::Home *ui = new Ui::Home;
  QMap<QString, QString> info;

public:
  Home ();
  ~Home () { delete ui; }

protected:
  void showEvent (QShowEvent *event) override;

public:
  void load_info ();
  void load_dish ();

private slots:
  void on_pbtn1_clicked ();
  void on_pbtn2_clicked ();
  void on_pbtn3_clicked ();
  // void on_pbtn4_clicked ();
  void on_pbtn5_clicked ();
  void on_pbtn6_clicked ();
};

#endif
