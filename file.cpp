/**
 * @file file.cpp
 * @author teamname: The QT's
 *
 * @brief
 * CS 3505
 * Assignment Name: A8: Sprite Editor Implementation
 *
 * The file class is responsible for saving and loading Sprite objects to and from files,
 * using JSON for serialization and deserialization.
 *
 * @date 2023-11-14
 *
 * This file was reviewed by Todd Oldham
 */
#include "file.h"

File::File(QWidget *parent)
    : QWidget{parent}
    , fileName("untitled.ssp") {
}

const QJsonObject File::serializeToJson(Sprite *sprite) {

    QJsonObject spriteObject;
    QJsonArray frameArray;

    spriteObject["spriteSize"] = sprite->spriteSize;

    for (Frame* frame : sprite->frames){

        QJsonObject frameObject;

        frameObject["width"] = frame->width;
        frameObject["height"] = frame->height;

        QJsonArray pixelArray;

        for(int i = 0; i < frame->width; i++) {

            QJsonArray arrayRow;

            for(int j = 0; j < frame->height; j++) {

                QColor currentPixel = frame->image.pixelColor(i,j);

                QJsonObject pixelData;
                int red, green, blue, alpha;

                currentPixel.getRgb(&red, &green, &blue, &alpha);

                pixelData["x"] = i;
                pixelData["y"] = j;
                pixelData["red"] = red;
                pixelData["green"] = green;
                pixelData["blue"] = blue;
                pixelData["alpha"] = alpha;

                arrayRow.append(pixelData);

            }

            frameObject["row"] = arrayRow;

            pixelArray.append(arrayRow);

        }

        frameObject["imageData"] = pixelArray;

        frameArray.append(frameObject);

    }

    spriteObject["frames"] = frameArray;

    return spriteObject;
}

bool File::saveFile(Sprite *sprite) {

    fileName = QFileDialog::getSaveFileName(this,
                           tr("Save Sprite"), "",
                           tr("sprite (*.ssp)"));

    QFile file(fileName);

    if (!file.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open save file.");
        return false;
    }

    QJsonObject spriteObject = serializeToJson(sprite);

    QJsonDocument jsonDoc(spriteObject);

    QByteArray jsonData = jsonDoc.toJson();

    file.write(jsonData);

    return true;
}

void File::deserializeFromJson(Sprite *sprite, QJsonObject spriteObject) {

    for(Frame* frame : sprite->frames) {
        delete frame;
    }

    sprite->frames.clear();

    sprite->spriteSize = spriteObject["spriteSize"].toInt();

    QJsonArray frameArray = spriteObject["frames"].toArray();

    for (const QJsonValue & frameJson : frameArray) {

        QJsonObject frameObject = frameJson.toObject();

        int frameSize = frameObject["width"].toInt();

        Frame* frame = new Frame(frameSize);

        QJsonArray pixelArray = frameObject["imageData"].toArray();

        for(const QJsonValue &rowArray : pixelArray ) {

            QJsonArray arrayRow = rowArray.toArray();

            for(const QJsonValue &pixelData : arrayRow) {

                int x = pixelData["x"].toInt();
                int y = pixelData["y"].toInt();
                int red = pixelData["red"].toInt();
                int green = pixelData["green"].toInt();
                int blue = pixelData["blue"].toInt();
                int alpha = pixelData["alpha"].toInt();

                frame->updatePixel(x, y, QColor::fromRgb(red, green, blue, alpha));

                if (alpha > 0){

                    frame->selectablePixels.append(qMakePair(x, y));
                }
            }
        }

        sprite->frames.append(frame);
    }
}

bool File::loadFile(Sprite* sprite) {

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                                  "/home",
                                                    tr("sprite (*.ssp)"));

    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open save file.");
        return false;
    }

    QByteArray saveData = file.readAll();

    QJsonDocument jsonDoc(QJsonDocument::fromJson(saveData));

    QJsonObject spriteObject = jsonDoc.object();

    deserializeFromJson(sprite, spriteObject);

    emit fileLoaded();

    return true;
}
