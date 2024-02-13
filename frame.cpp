/**
 * @file frame.cpp
 * @author teamname: The QT's
 * Reviewer name: Justin Zhu
 *
 * @brief
 * CS 3505
 * Assignment Name: A8: Sprite Editor Implementation
 *
 * The frame class is reponsible for displaying and interacting with sprite frames, handling rendering, mouse events,
 * and pixel updates. It provides functionalities like bucket filling and updating individual pixels, essential
 * for editing and visualizing sprite animations or images.
 *
 * @date 2023-11-15
 */
#include "frame.h"
#include "qpainter.h"
#include <QPixmap>
#include <QSizePolicy>
#include <QQueue>

Frame::Frame(int spriteSize, QWidget *parent) : QWidget{parent} {
    this->setFixedSize(QSize(120, 120));
    this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    this->setFocusPolicy(Qt::ClickFocus);

    // Set the size of the image
    width = spriteSize;
    height = spriteSize;
    QPixmap pixmap(width, height);

    // An image should start fully transparent
    pixmap.fill(QColor::fromRgb(0,0,0,0));
    image = pixmap.toImage();
}


void Frame::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    int x = 3;
    int y = 3;
    int w = 110;
    int h = 110;
    QRect target(x, y, w, h);

    // draw border if has focus
    if (this->hasFocus()) {
        QPen pen(Qt::cyan);
        int penWidth = 6;
        pen.setWidth(penWidth);
        painter.setPen(pen);
        painter.drawRect(x - 3, y - 3, w + 3, h + 3);
    }

    // want to draw checkers to signify transparency, but do it on a copy so we don't change
    // the actual image data
    QImage copy = image.copy(0, 0, width, height);

    // Set the color of every pixel in copy
    for(int i = 0; i < width; i++) {
        for(int j = 0; j < height; j++) {
            if(i % 2 != 0 && j % 2 != 0 && image.pixelColor(i, j).alpha() == 0) {
                copy.setPixelColor(i, j, QColor::fromRgb(0,0,0,60));
            }
            else if(i % 2 == 0 && j % 2 == 0 && image.pixelColor(i, j).alpha() == 0) {
                copy.setPixelColor(i, j, QColor::fromRgb(0,0,0,60));
            }
            else if(image.pixelColor(i, j).alpha() == 0) {
                copy.setPixelColor(i, j, QColor::fromRgb(0,0,0,50));
            }
        }
    }
    painter.drawImage(target, copy);
}

// if a widget gains focus, draw border around it
void Frame::focusInEvent(QFocusEvent *) {
    repaint();
}

// if a widget loses focus, get rid of border
void Frame::focusOutEvent(QFocusEvent *) {
    repaint();

    // clear any selected pixels
    emit focusLost();
}

void Frame::updatePixel(int pixelX, int pixelY, QColor color) {
    image.setPixelColor(pixelX, pixelY, color);
    repaint();
}

void Frame::mousePressEvent(QMouseEvent *event) {
    emit clicked(this);
}

// https://www.geeksforgeeks.org/flood-fill-algorithm/
QList<QPair<int, int>> Frame::bucketFill(int pixelX, int pixelY, QColor newColor) {
    QList<QPair<int, int>> modifiedPixels;

    QColor oldColor = image.pixelColor(pixelX, pixelY);

    QQueue< QPair<int, int> > pixelQueue;

    QPair<int, int> pixel(pixelX, pixelY);
    pixelQueue.enqueue(pixel);

    image.setPixelColor(pixelX,pixelY,newColor);
    modifiedPixels.append(qMakePair(pixelX, pixelY));

    while (pixelQueue.size() > 0) {
        QPair<int, int> currPixel = pixelQueue.front();
        pixelQueue.dequeue();

        int posX = currPixel.first;
        int posY = currPixel.second;

        // Check every pixel around the current pixel
        if (isValidPixel(posX + 1, posY, oldColor, newColor)) {
            image.setPixelColor(posX + 1,posY,newColor);
            modifiedPixels.append(qMakePair(posX + 1, posY));
            pixel.first = posX + 1;
            pixel.second = posY;
            pixelQueue.enqueue(pixel);
        }

        if (isValidPixel(posX - 1, posY, oldColor, newColor)) {
            image.setPixelColor(posX - 1,posY,newColor);
            modifiedPixels.append(qMakePair(posX - 1, posY));
            pixel.first = posX - 1;
            pixel.second = posY;
            pixelQueue.enqueue(pixel);
        }

        if (isValidPixel(posX, posY + 1, oldColor, newColor)) {
            image.setPixelColor(posX,posY + 1,newColor);
            modifiedPixels.append(qMakePair(posX, posY + 1));
            pixel.first = posX;
            pixel.second = posY + 1;
            pixelQueue.enqueue(pixel);
        }

        if (isValidPixel(posX, posY - 1, oldColor, newColor)) {
            image.setPixelColor(posX,posY - 1,newColor);
            modifiedPixels.append(qMakePair(posX, posY - 1));
            pixel.first = posX;
            pixel.second = posY - 1;
            pixelQueue.enqueue(pixel);
        }
    }
    return modifiedPixels;
}

bool Frame::isValidPixel(int pixelX, int pixelY, QColor OldColor, QColor newColor) {
    if (pixelX < 0 || pixelX >= width || pixelY < 0 || pixelY >= height || image.pixelColor(pixelX,pixelY) != OldColor || image.pixelColor(pixelX,pixelY) == newColor) {
        return false;
    }
    return true;
}
