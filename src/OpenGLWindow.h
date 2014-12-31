#ifndef OPENGLWINDOW_H
#define OPENGLWINDOW_H

#include <QWindow>
#include <QOpenGLFunctions>
#include <QCoreApplication>
#include <QEvent>
#include <QResizeEvent>
#include <QKeyEvent>
#include <QMatrix4x4>
#include <QtGui>

class OpenGLWindow : public QWindow, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    /**
     * @brief OpenGLWindow
     * @param parent
     */
    explicit OpenGLWindow(QWindow *parent = 0);

    /**
     * @brief ~OpenGLWindow
     */
    ~OpenGLWindow();

    /**
     * @brief ToogleAnimation
     * @param animating
     */
    void ToogleAnimation(bool animating);

public slots:
    /**
     * @brief RenderLater
     */
    void RenderLater();

    /**
     * @brief RenderNow
     */
    void RenderNow();

    /**
     * @brief SetXRotation
     * @param angle
     */
    virtual void SetXRotation(int angle);

    /**
     * @brief SetYRotation
     * @param angle
     */
    virtual void SetYRotation(int angle);

    /**
     * @brief SetZRotation
     * @param angle
     */
    virtual void SetZRotation(int angle);

    /**
     * @brief SetZTranslation
     * @param distance
     */
    virtual void SetZTranslation(int distance);


signals:
    /**
      * @brief xRotationChanged
      * @param angle
      */
    void xRotationChanged(int angle);

    /**
      * @brief yRotationChanged
      * @param angle
      */
    void yRotationChanged(int angle);

    /**
      * @brief zRotationChanged
      * @param angle
      */
    void zRotationChanged(int angle);

    /**
     * @brief zTranslationChanged
     * @param angle
     */
    void zTranslationChanged(int distance);

protected:
    /**
     * @brief event
     * @param event
     * @return
     */
    bool event(QEvent *event);

    /**
     * @brief exposeEvent
     * @param event
     */
    void exposeEvent(QExposeEvent *event);

    /**
     * @brief resizeEvent
     * @param event
     */
    void resizeEvent(QResizeEvent * event);

    /**
     * @brief keyPressEvent
     * @param event
     */
    void keyPressEvent(QKeyEvent * event);

    /**
     * @brief Render
     */
    virtual void Render();

    /**
     * @brief Initialize
     */
    virtual void Initialize();

    /**
     * @brief ResizeGLWindow
     * @param windowWidth
     * @param windowHeight
     */
    virtual void ResizeGLWindow(int windowWidth, int windowHeight);

    /**
     * @brief mousePressEvent
     * @param event
     */
    void mousePressEvent(QMouseEvent *event);

    /**
     * @brief mouseMoveEvent
     * @param event
     */
    void mouseMoveEvent(QMouseEvent *event);

    /** \brief OpenGL projection matrix */
    QMatrix4x4 projectionMatrix;

    /** \brief OpenGL model-view matrix */
    QMatrix4x4 modelViewMatrix;

    /** \brief Full-screen mode */
    bool fullScreen_;

    /** \brief X-axis rotation */
    GLfloat xRotation_;

    /** \brief Y-axis rotation */
    GLfloat yRotation_;

    /** \brief Z-axis rotation */
    GLfloat zRotation_;

    /** \brief X-axis Translation */
    GLfloat xTranslation_;

    /** \brief Y-axis Translation */
    GLfloat yTranslation_;

    /** \brief Z-axis Translation */
    GLfloat zTranslation_;

    /** \brief Last position the mouse was clicked */
    QPoint lastPosition_;

private:
    /** \brief Are there any updates pending! */
    bool updatePending_;

    /** \brief Is the window animating */
    bool animating_;

    /** \brief OpenGL context */
    QOpenGLContext *context_;
};

#endif // OPENGLWINDOW_H
