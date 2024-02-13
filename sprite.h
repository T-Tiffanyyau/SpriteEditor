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
 * @date 2023-11-14
 */
#ifndef SPRITE_H
#define SPRITE_H

#include <QWidget>
#include <QTimer>
#include <QJsonObject>
#include <QJsonArray>
#include "frame.h"


class Sprite : public QWidget
{
    Q_OBJECT

public:

    /**
     * @brief Sprite - the constructor initializes the variables and
     * connects the timer.
     * @param parent
     */
    explicit Sprite(QWidget *parent = nullptr);

    /**
     * Destructor deletes all of the sprites members.
     */
    ~Sprite();

    /**
     * @brief Holds the frames of this sprite.
     */
    QList<Frame*> frames;

    /**
     * @brief A timer for sending frames to the preview.
     */
    QTimer *timer;

    /**
     * @brief Holds the current index in the frame list we're at when sending frames
     * to preview.
     */
    int framesIndex;

    /**
     * @brief Holds the size of this sprite (length and width)
     */
    int spriteSize;

    /**
     * @brief mousePressEvent will trigger when the mouse is clicked on the sprite. It emits signals to pass the updated information of this frame to other parts of the application
     * and update the sprite preview.
     */
    void mousePressEvent(QMouseEvent *);

    /**
     * @brief adjustFrameCount checks if the number of frames in the sprite to match the specified frameCount.
     * It adds new frames or deletes excess frames to ensure the total number of frames equals frameCount.
     * It emits a signal to update the user interface with these changes.
     * @param frameCount
     */
    void adjustFrameCount(int frameCount);

signals:

    /**
     * @brief passChildSignal - sends a frame to the sprite canvas to be
     * displayed.
     * @param frame - the frame being sent.
     */
    void passFrameSignal(Frame* frame);

    /**
     * @brief sendFramesToPreview - sends a single frame to the sprite
     * preview class.
     * @param frame - the frame being sent.
     */
    void sendFramesToPreview(Frame* frame);

    /**
     * @brief frameCopied - this signal sends the newly copied frame to
     * the sprite editor.
     * @param newFrame - the new frame that old frame was copied to.
     */
    void frameCopied(Frame* newFrame);

    /**
     * @brief saveSprite - this signal sends the sprite to the file class
     * and lets the file class know to save this sprite to file.
     */
    void saveSprite(Sprite*);

    /**
     * @brief openSprite - this signal lets the file class know that it should
     * open a file and change the internal's of the sprite that is sent with
     * the signal.
     */
    void openSprite(Sprite*);

    /**
     * @brief sendSpriteToView - this signal send the newly loaded sprite
     * to the SpriteEditor so the editor can update its view.
     */
    void sendSpriteToView(Sprite*);

    /**
     * @brief sendAllFramesToPreview - this signal sends all of the frames
     * to the preview so it can redraw the newly opened frames.
     */
    void sendAllFramesToPreview(QList<Frame*>);

    /**
     * @brief setCanvasSize - this signal lets the sprite canvas know that
     * it needs to set its pixels size to size int sent with the signal.
     */
    void setCanvasSize(int);

public slots:

    /**
     * @brief sendFrames cycles through the sprite's frames, emitting a signal to display each frame in sequence for to preview the animation on top right.
     */
    void sendFrames();

    /**
     * @brief setPreviewSpeed adjusts the preview speed by setting the interval of a timer based on the provided speed value.
     */
    void setPreviewSpeed(int);

    /**
     * @brief setSpriteSize set the size of the frames when the program starts.
     */
    void setSpriteSize(int);

    /**
     * @brief saveSpriteToFile emits saveSprite signal when the save button is clicked.
     */
    void saveSpriteToFile();

    /**
     * @brief openSpriteFromFile emits openSprite signal when the open button is clicked.
     */
    void openSpriteFromFile();

    /**
     * @brief createNewFile indicates we are getting a new frame.
     */
    void createNewFile();

    /**
     * @brief updateSprite indicates to update sprite.
     */
    void updateSprite();

};

#endif // SPRITE_H
