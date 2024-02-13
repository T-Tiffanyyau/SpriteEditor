/**
 * @file frame.h
 * @author teamname: The QT's
 *
 * @brief
 * CS 3505
 * Assignment Name: A8: Sprite Editor Implementation
 *
 * The frame class is responsible for displaying and interacting with sprite frames, handling rendering, mouse events,
 * and pixel updates. It provides functionalities like bucket filling and updating individual pixels, essential
 * for editing and visualizing sprite animations or images.
 *
 * @date 2023-11-14
 */
#ifndef FRAME_H
#define FRAME_H

#include <QWidget>
#include <QImage>
#include <QJsonObject>

class Frame : public QWidget
{

    Q_OBJECT

public:

    Frame(int spriteSize, QWidget *parent = nullptr);

    /**
     * @brief This function creates an empty frame on the ui.
     */
    void paintEvent(QPaintEvent *);

    /**
     * @brief focusInEvent draws a frame around the selected frame, indicating it is being selected in ui.
     */
    void focusInEvent(QFocusEvent *);

    /**
     * @brief focusOutEvent remove the frame around the original selected frame as it is no longer selected.
     */
    void focusOutEvent(QFocusEvent *);

    /**
     * @brief bucketFill fills the pixels inside a boundary to a new color selected by user.
     * @param pixelX- the x coord of the pixel that was clicked
     * @param pixelY- the y coord of the pixel that was clicked
     * @param newColor- the color to bucket fill with
     * @return a list of the modified pixels
     */
    QList<QPair<int, int>> bucketFill(int pixelX, int pixelY, QColor newColor);

    /**
     * @brief A helper method for bucketFill to ensure that color operations are applied only to the intended pixels within the frame.
     * @param pixelX- the x coord of the pixel
     * @param pixelY- the y coord of the pixel
     * @param OldColor- the original color of the pixel
     * @param newColor- the desired new color
     * @return whether or not this pixel needs to be recolored
     */
    bool isValidPixel(int pixelX, int pixelY, QColor OldColor, QColor newColor);

    /**
     * @brief mousePressEvent is reponsible drawing the Qimage within the frame
     */
    void mousePressEvent(QMouseEvent *event);

    QList<QPair<int, int>> selectablePixels;
    QImage image;
    int width;
    int height;

signals:
    /**
     * @brief Called when a frame is clicked.
     */
    void clicked(Frame* frame);

    /**
     * @brief Called when focus between frames, at which point we
     * must clear any selected pixels
     */
    void focusLost();

public slots:

    /**
     * @brief Updates the given pixel to the given color
     * @param pixelX- the x coord of the pixel
     * @param pixelY- the y coord of the pixel
     * @param color- the color
     */
    void updatePixel(int pixelX, int pixelY, QColor color);

};

#endif // FRAME_H
