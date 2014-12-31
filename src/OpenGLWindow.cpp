/*******************************************************************************
 *
 * Copyrights Marwan Abdellah 2014 <marwan.m.abdellah@ieee.org>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 ******************************************************************************/

#include "OpenGLWindow.h"

/**
 * @brief OpenGLWindow::OpenGLWindow
 * Constructor
 * @param parent
 */
OpenGLWindow::OpenGLWindow(QWindow *parent) :
    QWindow(parent),
    updatePending_(false),
    animating_(false),
    context_(NULL),
    fullScreen_(false)
{
    // Specify whether the window is meant for raster rendering with
    // QBackingStore, or OpenGL rendering with QOpenGLContext.
    setSurfaceType(QWindow::OpenGLSurface);

    // Set the window size
    resize(800, 600);

    // Initialize the rotation
    xRotation_ = 0.f;
    yRotation_ = 0.f;
    zRotation_ = 0.f;

    // Initialize the translation
    xTranslation_ = 0.f;
    yTranslation_ = 0.f;
    zTranslation_ = -10.0f;
}

/**
 * @brief OpenGLWindow::~OpenGLWindow
 * Destructor
 */
OpenGLWindow::~OpenGLWindow()
{
    // Perform any clean-up operations here.
}

/**
 * @brief OpenGLWindow::Render
 */
void OpenGLWindow::Render()
{
    // Implement the rendering code here.
}

/**
 * @brief OpenGLWindow::Initialize
 */
void OpenGLWindow::Initialize()
{
    // Perform any initialization code here.
}

/**
 * @brief OpenGLWindow::ResizeGLWindow
 * Resize the OpenGL window.
 * @param w
 * @param h
 */
void OpenGLWindow::ResizeGLWindow(int windowWidth, int windowHeight)
{
    // Fix if _windowHeight_ is zero
    if (windowHeight == 0) {
        windowHeight = 1;
    }

    // Adjust the viewing port
    glViewport(0, 0, windowWidth, windowHeight);

    // Adjust the MVP matrix
    projectionMatrix.setToIdentity();
    projectionMatrix.perspective(45, (float)windowWidth/float(windowHeight), 1, 1000);
    modelViewMatrix.setToIdentity();
}

/**
 * @brief OpenGLWindow::ToogleAnimation
 * Set the animation state.
 * @param animating
 */
void OpenGLWindow::ToogleAnimation(bool animating)
{
    animating_ = animating;

    if(animating)
        RenderLater();

}

/**
 * @brief OpenGLWindow::RenderLater
 * Update some operation and then render the screen.
 */
void OpenGLWindow::RenderLater()
{
    if (!updatePending_) {
        // There is an update pending and then render the window.
        updatePending_ = true;

        QCoreApplication::postEvent(this, new QEvent(QEvent::UpdateRequest));
    }
}

/**
 * @brief OpenGLWindow::RenderNow
 */
void OpenGLWindow::RenderNow()
{
    // If the window is not show, then return.
    if (!isExposed())
        return;

    bool needsInitialize = false;

    // If no opengl context is available, create an OpenGL context.
    if (!context_) {
        // Create the context.
        context_ = new QOpenGLContext(this);

        // Set the window format.
        context_->setFormat(requestedFormat());

        // Create it.
        context_->create();

        // Make sure it needs initialization.
        needsInitialize = true;
    }

    // Make this context the current context.
    context_->makeCurrent(this);

    // If it is a new context, then initialize the context, else render directly
    if (needsInitialize) {

        // Initialize OpenGL windowing system
        initializeOpenGLFunctions();

        // Custom initialization
        Initialize();

        // Adjust the aspect ratio of the window
        const qreal retinaScale = devicePixelRatio();

        // Resize the window
        ResizeGLWindow(width() * retinaScale, height() * retinaScale);
    }

    // Render the frame
    Render();

    // Push pixels and swap buffers
    context_->swapBuffers(this);

    // If the window is animating, stop rendering and render later once the
    // window is stopped.
    if (animating_)
        RenderLater();
}

/**
 * @brief OpenGLWindow::event
 * Respond to the events on the OpenGL window.
 * @param event
 * @return
 */
bool OpenGLWindow::event(QEvent *event)
{
    switch (event->type()) {
    case QEvent::UpdateRequest : {
        updatePending_ = false;
        RenderNow();
        return true;
    }   break;
    default:
        return QWindow::event(event);
        break;
    }
}

/**
 * @brief OpenGLWindow::exposeEvent
 * @param event
 */
void OpenGLWindow::exposeEvent(QExposeEvent *event)
{
    // If the window is exposed, then render it.
    if (isExposed()) {
        RenderNow();
    }

    QWindow::exposeEvent(event);
}

/**
 * @brief OpenGLWindow::resizeEvent
 * @param event
 */
void OpenGLWindow::resizeEvent(QResizeEvent *event)
{
    // Get the new window width and height
    const int windowNewWidth = event->size().width();
    const int windowNewHeight = event->size().height();

    // Aspect ratio
    const qreal retinaScale = devicePixelRatio();

    // Resize OpenGL window
    ResizeGLWindow(windowNewWidth*retinaScale, windowNewHeight*retinaScale);

    // Update frame
    RenderNow();

    // Update window
    QWindow::resizeEvent(event);
}

/**
 * @brief OpenGLWindow::keyPressEvent
 * @param event
 */
void OpenGLWindow::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_F1: {

        // Toggle full screen
        fullScreen_ = !fullScreen_;

        if(fullScreen_) {
            // Show full screen
            showFullScreen();
        } else {
            // Normal size
            showNormal();
        }

    } break;

    case Qt::Key_Escape: {
        qApp->exit();

    } break;
    }

    QWindow::keyPressEvent(event);
}


static void qNormalizeAngle(int &angle)
{
    while (angle < 0)
        angle += 360 * 8;
    while (angle > 360 * 8)
        angle -= 360 * 8;
}

void OpenGLWindow::SetZTranslation(int distance)
{
    if (distance != zTranslation_) {
        zTranslation_ = distance;
        emit zTranslationChanged(distance);
        Render();
    }
}

void OpenGLWindow::SetXRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != xRotation_) {
        xRotation_ = angle;
        emit xRotationChanged(angle);
        Render();
    }
}

void OpenGLWindow::SetYRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != yRotation_) {
        yRotation_ = angle;
        emit yRotationChanged(angle);
        Render();
    }
}

void OpenGLWindow::SetZRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != zRotation_) {
        zRotation_ = angle;
        emit zRotationChanged(angle);
        Render();
    }
}

void OpenGLWindow::mousePressEvent(QMouseEvent *event)
 {
     lastPosition_ = event->pos();
 }

void OpenGLWindow::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - lastPosition_.x();
    int dy = event->y() - lastPosition_.y();

    if (event->buttons() & Qt::LeftButton) {
        SetXRotation(xRotation_ + 0.5 * dy);
        SetYRotation(yRotation_ + 0.5 * dx);
    } else if (event->buttons() & Qt::RightButton) {
        SetXRotation(xRotation_ + 0.5 * dy);
        SetZRotation(zRotation_ + 0.5 * dx);
    } else if (event->buttons() & Qt::MiddleButton) {
        SetZTranslation(zTranslation_ + 0.1 * dy);
    }

    lastPosition_ = event->pos();
}
