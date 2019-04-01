#ifndef CQToolBarMenu_H
#define CQToolBarMenu_H

#include <QFrame>
#include <QMenu>

class CQToolBarMenuBar;
class CQToolBarMenuMenu;
class CQAutoHide;

class QToolButton;
class QAction;
class QHBoxLayout;

class CQToolBarMenu : public QObject {
  Q_OBJECT

 public:
  CQToolBarMenu();

  void addToolButton(QToolButton *button);

  void addAction(QAction *action);

  void exec(const QPoint &gpos);

 private:
  bool eventFilter(QObject *obj, QEvent *event);

  bool processEvent(QObject *obj, QEvent *event) const;

  bool isInsideWidgets(const QPoint &gpos) const;

 private slots:
  void hideToolBar();

 private:
  CQToolBarMenuBar  *bar_          { nullptr };
  CQToolBarMenuMenu *menu_         { nullptr };
  mutable bool       popupMenu_    { false };
  mutable QPoint     popupMenuPos_;
};

class CQToolBarMenuBar : public QFrame {
 Q_OBJECT

 public:
  CQToolBarMenuBar(CQToolBarMenu *menu);
 ~CQToolBarMenuBar();

  void addButton(QToolButton *button);

  void show(const QRect &rect);

 private:
  bool event(QEvent *e);

 private:
  CQToolBarMenu *menu_   { nullptr };
  QHBoxLayout   *layout_ { nullptr };
  CQAutoHide    *hider_  { nullptr };
};

class CQToolBarMenuMenu : public QMenu {
 Q_OBJECT

 public:
  CQToolBarMenuMenu(CQToolBarMenu *menu);

  void addAction(QAction *action);

 private:
  CQToolBarMenu *menu_ { nullptr };
};

#endif
