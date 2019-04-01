#include <CQToolBarMenuTest.h>
#include <CQToolBarMenu.h>
#include <CQPixmapCache.h>

#include <QApplication>
#include <QToolButton>
#include <QContextMenuEvent>

#include <svg/bold_svg.h>
#include <svg/italic_svg.h>
#include <svg/underline_svg.h>

int
main(int argc, char **argv)
{
  QApplication app(argc, argv);

  CQToolBarMenuTest *test = new CQToolBarMenuTest;

  test->show();

  return app.exec();
}

CQToolBarMenuTest::
CQToolBarMenuTest()
{
  menu_ = new CQToolBarMenu;

  auto addAction = [&](const QString name) {
    QAction *action = new QAction(name);
    menu_->addAction(action);
    return action;
  };

  addAction("&Cut");
  addAction("&Copy");
  addAction("&Paste");

  auto createToolButton = [](const QString name, const QString &iconName) {
    QToolButton *button = new QToolButton;

    button->setObjectName(name);
    button->setIcon(CQPixmapCacheInst->getIcon(iconName));

    return button;
  };

  QToolButton *boldButton      = createToolButton("bold"     , "BOLD"     );
  QToolButton *italicButton    = createToolButton("italic"   , "ITALIC"   );
  QToolButton *underlineButton = createToolButton("underline", "UNDERLINE");

  menu_->addToolButton(boldButton);
  menu_->addToolButton(italicButton);
  menu_->addToolButton(underlineButton);
}

void
CQToolBarMenuTest::
contextMenuEvent(QContextMenuEvent *e)
{
  menu_->exec(e->globalPos());
}

QSize
CQToolBarMenuTest::
sizeHint() const
{
  return QSize(400, 400);
}
