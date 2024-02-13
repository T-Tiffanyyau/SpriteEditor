#include <QImage>
#include "spritepreview.h"
#include "qpainter.h"

SpritePreview::SpritePreview(QWidget *parent) : QWidget{parent} {
    currFrame = nullptr;
}

void SpritePreview::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    QRect target(0, 0, 130, 130);

    if (currFrame) {
        painter.drawImage(target, currFrame->image);
    }
}

void SpritePreview::addFrame(Frame* frame) {
    if (frame) {
        currFrame = frame;
        repaint();
    }
}

void SpritePreview::updatePreview(Frame* frame) {
    if (frame) {
        currFrame = frame;
    } else {
        currFrame = nullptr;
    }
    repaint();
}

void SpritePreview::loadNewFrames(QList<Frame*> newFrames) {
    frames = newFrames;
    currFrame = frames.isEmpty() ? nullptr : frames.first();
}
