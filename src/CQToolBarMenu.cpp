#include <CQToolBarMenu.h>
#include <CQAutoHide.h>

#include <QToolButton>
#include <QAction>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QApplication>

#include <iostream>

CQToolBarMenu::
CQToolBarMenu()
{
  bar_  = new CQToolBarMenuBar (this);
  menu_ = new CQToolBarMenuMenu(this);
}

void
CQToolBarMenu::
addAction(QAction *action)
{
  menu_->addAction(action);

  connect(action, SIGNAL(triggered()), this, SLOT(hideToolBar()));
}

void
CQToolBarMenu::
addToolButton(QToolButton *button)
{
  bar_->addButton(button);
}

void
CQToolBarMenu::
exec(const QPoint &gpos)
{
  popupMenu_    = false;
  popupMenuPos_ = QPoint();

  //---

  qApp->installEventFilter(this);

  QSize s = bar_->sizeHint();

  QRect rect(gpos.x(), gpos.y() - s.height() - 4, s.width(), s.height());

  bar_->show(rect);

  menu_->exec(gpos);

  qApp->removeEventFilter(this);

  //---

  if (popupMenu_) {
    QPoint gpos = popupMenuPos_;

    exec(gpos);
  }
}

void
CQToolBarMenu::
hideToolBar()
{
  bar_->hide();
}

bool
CQToolBarMenu::
eventFilter(QObject *obj, QEvent *event)
{
  bool ok = processEvent(obj, event);

  if (! ok) {
    menu_->hide();
    bar_ ->hide();
  }

  return QObject::eventFilter(obj, event);
}

bool
CQToolBarMenu::
processEvent(QObject *obj, QEvent *event) const
{
  QEvent::Type type = event->type();

  // ignore if not a widget
  QWidget *w = qobject_cast<QWidget *>(obj);
  if (! w) return true;

  // handle button press
  if      (type == QEvent::MouseButtonPress) {
    QMouseEvent *me = static_cast<QMouseEvent *>(event);

    if      (me->button() == Qt::LeftButton) {
      if (! isInsideWidgets(me->globalPos()))
        return false;
    }
    else if (me->button() == Qt::RightButton) {
      popupMenu_    = true;
      popupMenuPos_ = me->globalPos();

      return false;
    }
  }
  else if (type == QEvent::MouseButtonDblClick) {
    QMouseEvent *me = static_cast<QMouseEvent *>(event);

    if (! isInsideWidgets(me->globalPos()))
      return false;
  }

  return true;
}

bool
CQToolBarMenu::
isInsideWidgets(const QPoint &gpos) const
{
  // check if inside menu or bar wdgets
  return (menu_->rect().contains(menu_->mapFromGlobal(gpos)) ||
          bar_ ->rect().contains(bar_ ->mapFromGlobal(gpos)));
}

//---

CQToolBarMenuBar::
CQToolBarMenuBar(CQToolBarMenu *menu) :
 menu_(menu)
{
  layout_ = new QHBoxLayout(this);

  hider_ = new CQAutoHide(this);

  hide();
}

CQToolBarMenuBar::
~CQToolBarMenuBar()
{
  delete hider_;
}

void
CQToolBarMenuBar::
addButton(QToolButton *button)
{
  layout_->addWidget(button);
}

void
CQToolBarMenuBar::
show(const QRect &rect)
{
  setParent(0, Qt::Tool | Qt::FramelessWindowHint | Qt::X11BypassWindowManagerHint);

  setGeometry(rect);

  QFrame::show();

  raise();

  hider_->setActive(true);
}

bool
CQToolBarMenuBar::
event(QEvent *e)
{
  if      (e->type() == QEvent::KeyPress) {
    QKeyEvent *ke = dynamic_cast<QKeyEvent *>(e);

    if (ke->key() == Qt::Key_Escape) {
      hide();
    }
  }
  else if (e->type() == QEvent::Hide) {
    hider_->setActive(false);
  }

  return QFrame::event(e);
}

//---

CQToolBarMenuMenu::
CQToolBarMenuMenu(CQToolBarMenu *menu) :
 menu_(menu)
{
}

void
CQToolBarMenuMenu::
addAction(QAction *action)
{
  QMenu::addAction(action);
}
