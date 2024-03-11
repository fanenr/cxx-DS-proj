#include "log.h"
#include "home.h"
#include "http.h"
#include "reg.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>

#include <QtNetwork/QNetworkReply>

Log::Log (Home *parent) : QMainWindow (parent), prnt (parent)
{
  ui->setupUi (this);
  setAttribute (Qt::WA_DeleteOnClose);

  btns.addButton (ui->rbtn1, 1);
  btns.addButton (ui->rbtn2, 2);
}

type
Log::category ()
{
  if (ui->rbtn1->isChecked ())
    return type::STUDENT;
  if (ui->rbtn2->isChecked ())
    return type::MERCHANT;
  abort ();
}

void
Log::on_pbtn1_clicked ()
{
  auto user = ui->ledit1->text ();
  auto pass = ui->ledit2->text ();

  if (user.isEmpty () || pass.isEmpty ())
    {
      QMessageBox::warning (this, tr ("提示"), tr ("请输入帐号密码"));
      return;
    }

  Reg *reg;
  static Reg *sreg;
  static Reg *mreg;
  auto typ = category ();

  switch (typ)
    {
    case type::STUDENT:
      if (!sreg)
        sreg = new Reg (this);
      reg = sreg;
      break;
    case type::MERCHANT:
      if (!mreg)
        mreg = new Reg (this);
      reg = mreg;
      break;
    default:
      break;
    }

  if (reg->isVisible ())
    return;

  reg->show ();
}

void
Log::on_pbtn2_clicked ()
{
  auto user = ui->ledit1->text ();
  auto pass = ui->ledit2->text ();

  if (user.isEmpty () || pass.isEmpty ())
    {
      QMessageBox::warning (this, tr ("提示"), tr ("请输入帐号密码"));
      return;
    }

  QString req_url;
  auto typ = category ();

  switch (typ)
    {
    case type::STUDENT:
      req_url = URL_STUDENT_LOG;
      break;
    case type::MERCHANT:
      req_url = URL_MERCHANT_LOG;
      break;
    default:
      break;
    }

  QJsonObject req_data;
  req_data["user"] = user;
  req_data["pass"] = pass;

  Http http;
  auto reply = http.post (req_url, req_data);
  if (reply->error ())
    {
      QMessageBox::warning (this, tr ("失败"), tr ("无法发送网络请求"));
      return;
    }

  auto res = QJsonDocument::fromJson (reply->readAll ()).object ();
  if (res["code"] != 0)
    {
      QMessageBox::warning (this, tr ("失败"),
                            res["data"].toString (tr ("信息丢失")));
      return;
    }

  QMap<QString, QString> info;
  auto map = res["data"].toObject ().toVariantMap ();

  for (auto it = map.cbegin (); it != map.cend (); it++)
    info.insert (it.key (), it.value ().toString ());

  prnt->info = std::move (info);
  prnt->typ = typ;

  prnt->load_info ();
  prnt->load_dish ();
  prnt->show ();
  close ();
}
