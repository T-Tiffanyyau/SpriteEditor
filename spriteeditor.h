/**
 * @file spriteeditor.h
 * @author teamname: The QT's
 *
 * @brief
 * CS 3505
 * Assignment Name: A8: Sprite Editor Implementation
 *
 * The spriteeditor class is reponsible for functions on the ui application like addFrame, copyFrame, saveFiles etc.
 *
 * @date 2023-11-14
 */
#ifndef SPRITEEDITOR_H
#define SPRITEEDITOR_H

#include <QMainWindow>
#include "QtWidgets/qboxlayout.h"
#include "sprite.h"
#include "file.h"

QT_BEGIN_NAMESPACE
namespace Ui { class SpriteEditor; }
QT_END_NAMESPACE

class SpriteEditor : public QMainWindow
{
    Q_OBJECT

public:
    SpriteEditor(File& file, QWidget *parent = nullptr);
    ~SpriteEditor();
    QHBoxLayout* layout;

signals:
    void changeColor(QColor);
    void sendSpriteSize(int);
    void connectFrameSignals(Frame* frame);

private:
    Ui::SpriteEditor *ui;
    int spriteSize;
    Sprite* mySprite;

    void setSpriteSize();
    void createFileActions(Sprite* sprite);
    void createFileMenu();

    QMenu *fileMenu;

    QAction *newAction;
    QAction *openAction;
    QAction *saveAction;
    QAction *exitAct;
    void addFrame();
    Frame* selectedFrame;

private slots:
    /**
     * @brief chooseColor Shows the color picker and sets the current color on the canvas
     * Source: https://www.youtube.com/watch?v=fl3q_5bxhlk
     */
    void chooseColor();

    /**
     * @brief Copies the focused frame when the copy frame button is clicked.
     */
    void copyFrame();

    /**
     * @brief Deletes the focused frame when the delete frame button is clicked.
     */
    void deleteFrame();

    /**
     * @brief Adds a loaded sprite's frames to the layout.
     */
    void addFramesToLayout(Sprite *sprite);

    /**
     * @brief Called to display instructions on copy-pasting when the copy pixels
     * button is clicked.
     */
    void showCpInstructions();

    /**
     * @brief Called to hide the instructions on copy-pasting when the copy-paste
     * has completed.
     */
    void hideCpInstructions();

    /**
     * @brief Called to display a message to select pixels if the user hits the copy
     * pixels button and there are no selected pixels.
     */
    void tellUserToSelectPixels();

    /**
     * @brief Called to hide the message to select pixels.
     */
    void hideExplanation();

public slots:

    /**
     * @brief Takes care of copying the given frame.
     */
    void onFrameCopied(Frame* newFrame);

    /**
     * @brief Sets the selected frame to frame.
     */
    void frameSelected(Frame* frame);

};
#endif // SPRITEEDITOR_H
