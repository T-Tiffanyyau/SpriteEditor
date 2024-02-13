/**
 * @file sprite.h
 * @author teamname: The QT's
 *
 * @brief
 * CS 3505
 * Assignment Name: A8: Sprite Editor Implementation
 *
 * The sprite class manages sprite animations by handling a collection of Frame objects and
 * controlling their display and updates.
 *
 * Reviewed by Bracken Asay.
 *
 * @date 2023-11-14
 */

#include "sprite.h"
#include "frame.h"
#include <QColor>
#include <QMouseEvent>

Sprite::Sprite(QWidget *parent)
    : QWidget{parent},
    framesIndex(0)
{
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Sprite::sendFrames);
}

Sprite::~Sprite() {
    for(Frame* frame : frames){
        delete frame;
    }

    frames.clear();
}

void Sprite::mousePressEvent(QMouseEvent *) {
    if(frames.size() != 0){
        emit sendFramesToPreview(frames[0]);
    }
}


void Sprite::sendFrames() {
    if (frames.isEmpty()) {
        emit sendFramesToPreview(nullptr);
        return;
    }

    if (framesIndex >= frames.size()) framesIndex = 0;

    emit sendFramesToPreview(frames[framesIndex]);

    framesIndex++;
}


void Sprite::setPreviewSpeed(int speed) {
    if (speed > 0) {
        timer->setInterval(1000 / speed);
    }else{
        timer->setInterval(1000);
    }
}

void Sprite::setSpriteSize(int size) {
    spriteSize = size;

    Frame* frame1 = new Frame(spriteSize);
    frames.append(frame1);

    emit passFrameSignal(frame1);

    timer->setInterval(1000);
    timer->start();

    framesIndex = 0;
}

void Sprite::saveSpriteToFile() {
    emit saveSprite(this);
}

void Sprite::openSpriteFromFile() {
    timer->stop();
    emit openSprite(this);
}

void Sprite::createNewFile( ) {
    timer->stop();

    this->frames.clear();

    Frame* frame = new Frame(spriteSize);
    frames.append(frame);

    emit passFrameSignal(frame);
    emit sendSpriteToView(this);
    emit sendAllFramesToPreview(frames);

    timer->start();
}

void Sprite::updateSprite() {
    timer->start();

    if(!frames.isEmpty()) {
        emit passFrameSignal(frames[0]);
        emit sendSpriteToView(this);
        emit sendAllFramesToPreview(frames);
        emit setCanvasSize(spriteSize);
    }
}
