/**
 * @file spritecanvas.h
 * @author teamname: The QT's
 *
 * @brief
 * CS 3505
 * Assignment Name: A8: Sprite Editor Implementation
 *
 * The spritecanvas class is reponsible for displaying and editing individual frames of a sprite,
 * handling user inputs like mouse movements and clicks for drawing and manipulating pixels.
 *
 * @date 2023-11-14
 *
 * This file was reviewed by Hannah Larsen
 */
#include "spritecanvas.h"
#include "qpainter.h"
#include "frame.h"
#include <QPixmap>
#include <QMouseEvent>

SpriteCanvas::SpriteCanvas(QWidget *parent)
    : QWidget{parent}
{
    currFrame = nullptr;
    groupSelect = false;
    clickIsForAnchorSelection = false;
    clickIsForPasting = false;
    bucketFillOn = false;
    eraserOn = false;
    penToolOn = true;
    color = QColor::fromRgb(0, 0, 0);
    editScreenSize = 325;
}

void SpriteCanvas::setSpriteSize(int size) {
    spriteSize = size;
    source.setRect(0, 0, spriteSize, spriteSize);
}

void SpriteCanvas::setCurrentFrame(Frame* frame) {
    currFrame = frame;
}

void SpriteCanvas::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    QRect target(0, 0, editScreenSize, editScreenSize);

    if (currFrame) {
        // make a copy of currFrame's image so we can draw on it to show pixel selection without altering the actual
        // image data
        QImage copy = currFrame->image.copy(0, 0, currFrame->image.width(), currFrame->image.height());
        painter.drawImage(target, currFrame->image, source);
        update();

        // if a group pixel selection is in progress, draw semi-transparent layer over the selected pixels
        // to show they've been selected
        if (groupSelect) {
            QPainter painter2(&copy);
            QPen pen = QPen(QRgba64::fromRgba(0, 255, 255, 50));
            pen.setWidth(1);
            painter2.setPen(pen);

            for (QPair<int, int> coords : selectedPixels){
                // draw on the copied image
                painter2.drawPoint(coords.first, coords.second);
            }
        }

        // draw checkers to show transparent pixels
        for(int i = 0; i < currFrame->image.width(); i++) {
            for(int j = 0; j < currFrame->image.height(); j++) {
                if(i % 2 != 0 && j % 2 != 0 && currFrame->image.pixelColor(i, j).alpha() == 0) {
                    copy.setPixelColor(i, j, QColor::fromRgb(0,0,0,60));
                }
                else if(i % 2 == 0 && j % 2 == 0 && currFrame->image.pixelColor(i, j).alpha() == 0) {
                    copy.setPixelColor(i, j, QColor::fromRgb(0,0,0,60));
                }
                else if (currFrame->image.pixelColor(i, j).alpha() == 0) {
                    copy.setPixelColor(i, j, QColor::fromRgb(0,0,0,50));
                }
            }
        }
        painter.drawImage(target, copy, source);
    }
}

void SpriteCanvas::changeColor(QColor newColor) {
    color = newColor;

    if (groupSelect) {
        for (QPair<int, int> coords : selectedPixels) {
            currFrame->updatePixel(coords.first, coords.second, color);
        }

        currFrame->repaint();
        repaint();

        // "unselect" all pixels
        selectedPixels.clear();
        repaint();
    }
}

void SpriteCanvas::updateGroupSelectState(bool checked) {
    groupSelect = checked;

    // cover the case where they select pixels and turn off group select before
    // doing anything to them
    if (!groupSelect) {
        selectedPixels.clear();
    }
}

void SpriteCanvas::updateBucketFillState(bool checked) {
    bucketFillOn = checked;
}

void SpriteCanvas::updateEraserState(bool checked) {
    eraserOn = checked;
}

void SpriteCanvas::updatePenToolState(bool checked) {
    penToolOn = checked;
}

void SpriteCanvas::updateCopyPasteState() {
    // only copy if there are selected pixels
    if (selectedPixels.size() > 0) {
        clickIsForAnchorSelection = true;
        emit validCopy();
    }else {
        // give message saying no selected pixels to copy
        emit noSelectedPixelsToCopy();
    }
}

void SpriteCanvas::clearSelectedPixels() {
    selectedPixels.clear();

    // reset everything to their base states just in case
    emit pastingDone();
    clickIsForAnchorSelection = false;
}

void SpriteCanvas::mouseMoveEvent(QMouseEvent * e) {
    if (!currFrame) {
        return;
    }

    int xPos = e->pos().x();
    int yPos = e->pos().y();

    int pixelXCoord = source.x() + xPos/(editScreenSize/source.width());
    int pixelYCoord = source.y() + yPos/(editScreenSize/source.height());

    if (eraserOn) {
        currFrame->updatePixel(pixelXCoord, pixelYCoord, QColor::fromRgb(0,0,0,0));
        repaint();

        // make the pixel unselectable (QList doesn't have a nice remove method so needed to do it manually)
        QPair<int, int> pixelCoords = qMakePair(pixelXCoord, pixelYCoord);
        for (int i = 0; i < currFrame->selectablePixels.size(); i++) {
            if (currFrame->selectablePixels.at(i) == pixelCoords) {
                currFrame->selectablePixels.removeAt(i);
            }
        }
    }

    // if we're selecting pixels
    else if (groupSelect) {
        QPair<int, int> pixelCoords = qMakePair(pixelXCoord, pixelYCoord);

        // only "select" the pixel if it is "selectable" (has been drawn on before and is not already selected)
        if (currFrame->selectablePixels.contains(pixelCoords) && !selectedPixels.contains(pixelCoords)) {
            selectedPixels.append(pixelCoords);
        }

        // give a visual cue that it's selected
        repaint();
    }

    // if we're drawing
    else if (penToolOn){
        currFrame->updatePixel(pixelXCoord, pixelYCoord, color);
        currFrame->repaint();

        repaint();

        // this pixel is now selectable because it's been drawn on
        QPair<int, int> pixelCoords = qMakePair(pixelXCoord, pixelYCoord);
        if (!currFrame->selectablePixels.contains(pixelCoords)) {
            currFrame->selectablePixels.append(pixelCoords);
        }
    }
}

void SpriteCanvas::mousePressEvent(QMouseEvent * e) {
    if (!currFrame) {
        return;
    }

    int xPos = e->pos().x();
    int yPos = e->pos().y();

    int pixelXCoord = source.x() + xPos/(editScreenSize/source.width());
    int pixelYCoord = source.y() + yPos/(editScreenSize/source.height());

    if (bucketFillOn) {
        QList<QPair<int, int>> modifiedPixels = currFrame->bucketFill(pixelXCoord, pixelYCoord, color);
        // these pixels are now selectable since they've been colored
        for (QPair<int, int> pixel : modifiedPixels) {
            if (!currFrame->selectablePixels.contains(pixel)) {
                currFrame->selectablePixels.append(pixel);
            }
        }
        currFrame->repaint();
        repaint();
    }

    else if (eraserOn) {
        currFrame->updatePixel(pixelXCoord, pixelYCoord, QColor::fromRgb(0,0,0,0));
        repaint();

        // make the pixel unselectable (QList doesn't have a nice remove method so needed to do it manually)
        QPair<int, int> pixelCoords = qMakePair(pixelXCoord, pixelYCoord);
        for (int i = 0; i < currFrame->selectablePixels.size(); i++) {
            if (currFrame->selectablePixels.at(i) == pixelCoords) {
                currFrame->selectablePixels.removeAt(i);
            }
        }
    }

    // if we're selecting pixels
    else if (groupSelect) {
        if (clickIsForAnchorSelection) {
            pastingAnchorPoint = qMakePair(pixelXCoord, pixelYCoord);
            clickIsForAnchorSelection = false;
            clickIsForPasting = true;
        }

        else if (clickIsForPasting) {
            QPair<int, int> pastePoint = qMakePair(pixelXCoord, pixelYCoord);
            clickIsForPasting = false;

            // now define the translation
            int xTranslation = pastePoint.first - pastingAnchorPoint.first;
            int yTranslation = pastePoint.second - pastingAnchorPoint.second;

            for (QPair<int, int> coords : selectedPixels) {
                QColor pixelColor = currFrame->image.pixelColor(coords.first, coords.second);

                int resultX = coords.first + xTranslation;
                int resultY = coords.second + yTranslation;

                // make sure it's a valid pixel position
                if (resultX >= 0 && resultX < source.width() && resultY >= 0 && resultY < source.height()) {
                    currFrame->updatePixel(resultX, resultY, pixelColor);

                    // this pixel is now selectable because it's been drawn on
                    QPair<int, int> pixelCoords = qMakePair(resultX, resultY);
                    if (!currFrame->selectablePixels.contains(pixelCoords)) {
                        currFrame->selectablePixels.append(pixelCoords);
                    }
                }
            }

            currFrame->repaint();
            selectedPixels.clear();
            repaint();
            emit pastingDone();
        }

        // otherwise just add to group
        else {
            QPair<int, int> pixelCoords = qMakePair(pixelXCoord, pixelYCoord);

            // only "select" the pixel if it is "selectable" (has been drawn on before and is not already selected)
            if (currFrame->selectablePixels.contains(pixelCoords) && !selectedPixels.contains(pixelCoords)) {
                selectedPixels.append(pixelCoords);
            }

            // give a visual cue that it's selected
            repaint();
        }
    }

    // if we're drawing
    else if (penToolOn) {
        currFrame->updatePixel(pixelXCoord, pixelYCoord, color);
        currFrame->repaint();

        repaint();

        // this pixel is now selectable because it's been drawn on
        QPair<int, int> pixelCoords = qMakePair(pixelXCoord, pixelYCoord);
        if (!currFrame->selectablePixels.contains(pixelCoords)) {
            currFrame->selectablePixels.append(pixelCoords);
        }
    }
}

void SpriteCanvas::wheelEvent(QWheelEvent * e) {
    int newSize = source.width();
    int newX = source.x();
    int newY = source.y();

    if(e->angleDelta().y() > 0) {
        if(source.width() > 1) {
            newSize = source.width() - 1;
        }
        if(e->position().x() - 125 > 0) {
            if(newX < spriteSize - 1) {
                newX = newX + 1;
            }
        }
        if(e->position().y() - 125 > 0) {
            if(newY < spriteSize - 1) {
                newY = newY + 1;
            }
        }
    }else {
        if(source.width() < spriteSize) {
            newSize = source.width() + 1;
        }
        if(newX > 0) {
            newX = newX - 1;
        }
        if(newY > 0) {
            newY = newY - 1;
        }
    }

    source.setRect(newX, newY, newSize, newSize);
    currFrame->repaint();
    repaint();
}
