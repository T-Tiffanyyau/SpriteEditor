/**
 * @file spritepreview.h
 * @author teamname: The QT's
 *
 * @brief
 * CS 3505
 * Assignment Name: A8: Sprite Editor Implementation
 *
 * The SpritePreview class is responsible for displaying the preview of the frame animation on the top right corner
 * of the UI.
 *
 * This File was reviewed by Matthew Lam
 *
 * @date 2023-11-14
 */

#ifndef SPRITEPREVIEW_H
#define SPRITEPREVIEW_H

#include <QWidget>
#include "frame.h"

class SpritePreview : public QWidget {
    Q_OBJECT

public:
    explicit SpritePreview(QWidget *parent = nullptr);

    /**
     * @brief paintEvent draws the pixels of the selected frame on the UI.
     */
    void paintEvent(QPaintEvent *);

    /**
     * @brief Helper function for copy frame in SpriteEditor. Add the copied frame to the preview.
     * @param frame The frame to add.
     */
    void addFrame(Frame* frame);

private:
    QList<Frame*> frames;
    Frame *currFrame;

public slots:
    /**
     * @brief updatePreview updates the frame that is currently being previewed.
     * @param frame The frame to update.
     */
    void updatePreview(Frame* frame);

    /**
     * @brief loadNewFrames updates the preview with the frames.
     * @param newFrames The new frames to load.
     */
    void loadNewFrames(QList<Frame*> newFrames);
};

#endif // SPRITEPREVIEW_H
