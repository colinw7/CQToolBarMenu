#include <QFrame>

class CQToolBarMenu;

class CQToolBarMenuTest : public QFrame {
  Q_OBJECT

 public:
  CQToolBarMenuTest();

  void contextMenuEvent(QContextMenuEvent *e);

  QSize sizeHint() const;

 private:
  CQToolBarMenu *menu_ { nullptr };
};
