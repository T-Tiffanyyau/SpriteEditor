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
 */
#ifndef SPRITECANVAS_H
#define SPRITECANVAS_H

#include <QWidget>
#include "frame.h"

class SpriteCanvas : public QWidget
{
    Q_OBJECT

public:

    explicit SpriteCanvas(QWidget *parent = nullptr);

    /**
     * @brief paintEvent handles drawing operations such as displaying the sprite's image, indicating selected pixels.
     * Override the default paintEvent with drawing of the shape.
     */
    void paintEvent(QPaintEvent *);

    /**
     * @brief mouseMoveEvent is reponsible for mouse movement events such as drawing, erasing, or selecting pixels on the sprite frame.
     */
    void mouseMoveEvent(QMouseEvent *);

    /**
     * @brief mousePressEvent is reponsible for actions when the mouse is pressed, such as drawing, erasing, or selecting pixels on the sprite frame
     * when the mouse is pressed.
     */
    void mousePressEvent(QMouseEvent *);

    /**
     * @brief wheelEvent is reponsible for mouse wheel movements, allowing the user to zoom in or out on the sprite frame being edited
     */
    void wheelEvent(QWheelEvent *);

private:

    /**
     * @brief Keeps track of which frame has focus.
     */
    Frame* currFrame;

    /**
     * @brief The rectangle defining the portion of a frame's image to draw (the
     * whole image in this case)
     */
    QRect source;

    /**
     * @brief Keeps track of the current color to draw with.
     */
    QColor color;

    /**
     * @brief Holds the size (length and width) of the sprite we're drawing.
     */
    int spriteSize;

    /**
     * @brief Holds the size (length and width) of the editor screen.
     */
    int editScreenSize;

    /**
     * @brief A list of the x, y coordinates of the pixels that have been selected
     * by the user.
     */
    QList<QPair<int, int>> selectedPixels;

    /**
     * @brief Holds the x, y coordinates of the pixel chosen to anchor a copy-paste around.
     * (the translation for the point at which they wish to paste will be defined from this point)
     */
    QPair<int, int> pastingAnchorPoint;

    /**
     * @brief Holds whether or not the next click should be interpreted as an anchor selection
     * for copy-pasting. (This will be true immediately after clicking the copy button to copy
     * selected pixels)
     */
    bool clickIsForAnchorSelection;

    /**
     * @brief Holds whether or not the next click should be interpreted as the point at which
     * the user wants to paste a copied group of pixels. (This will be true immediately after
     * an anchor selection has been made)
     */
    bool clickIsForPasting;

    /**
     * @brief Holds whether or not the select feature is currently enabled.
     */
    bool groupSelect;

    /**
     * @brief Holds whether or not the bucket fill feature is currently enabled.
     */
    bool bucketFillOn;

    /**
     * @brief Holds whether or not the eraser feature is currently enabled.
     */
    bool eraserOn;

    /**
     * @brief Holds whether or not the pen tool feature is currently enabled.
     */
    bool penToolOn;


public slots:

    /**
     * @brief setCurrentFrame is used to set the current frame that is being displayed and edited on the canvas.
     * @param frame- the frame to set currframe to
     */
    void setCurrentFrame(Frame* frame);

    /**
     * @brief Called when a new color has been picked by the user, this method
     * updates the color to draw with.
     * @param color - The color that was selected.
     */
    void changeColor(QColor color);

    /**
     * @brief Called when a sprite size has been chosen by the user, this method
     * correspondingly sets the spriteSize and source variables.
     * @param size - The chosen size.
     */
    void setSpriteSize(int size);

    /**
     * @brief Called when the selection feature has been enabled or disabled, this
     * method correspondingly updates the bool holding whether or not selection is enabled.
     * @param checked - Whether or not selection is enabled.
     */
    void updateGroupSelectState(bool checked);

    /**
     * @brief Called when the bucket fill feature has been enabled or disabled, this
     * method correspondingly updates the bool holding whether or not bucket fill is enabled.
     * @param checked - Whether or not bucket fill is enabled.
     */
    void updateBucketFillState(bool);

    /**
     * @brief Called when the eraser feature has been enabled or disabled, this
     * method correspondingly updates the bool holding whether or not erasing is enabled.
     * @param checked - Whether or not erasing is enabled.
     */
    void updateEraserState(bool);

    /**
     * @brief Called when the pen tool feature has been enabled or disabled, this
     * method correspondingly updates the bool holding whether or not the pen tool is enabled.
     * @param checked - Whether or not the pen tool is enabled.
     */
    void updatePenToolState(bool);

    /**
     * @brief Called when the button is pressed to copy a group of selected pixels, this method
     * sets clickIsForAnchorSelection to true, so the next click will be interpreted as the
     * anchor point.
     */
    void updateCopyPasteState();

    /**
     * @brief Called when focus switches between frames, this method deselects all pixels.
     */
    void clearSelectedPixels();

signals:

    /**
     * @brief Emitted when a group of pixels has been pasted successfully.
     */
    void pastingDone();

    /**
     * @brief Emitted when a user hits the copy button but there are no selected pixels,
     * tells the view to display an informative message.
     */
    void noSelectedPixelsToCopy();

    /**
     * @brief Emitted when a user hits the copy button there are selected pixels, thus it was
     * a valid copy request. Tells the view to display an informative message on anchor selection
     * and pasting.
     */
    void validCopy();

};

#endif // SPRITECANVAS_H
