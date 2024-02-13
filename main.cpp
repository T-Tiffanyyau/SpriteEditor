#include "spriteeditor.h"
#include "sprite.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    File file;
    SpriteEditor w(file);
    w.show();
    return a.exec();
}
