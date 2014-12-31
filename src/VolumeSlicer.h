#ifndef TEXTUREMAPPINGWINDOW_H
#define TEXTUREMAPPINGWINDOW_H

#include "OpenGLWindow.h"
#include <QOpenGLShaderProgram>

class VolumeSlicer : public OpenGLWindow
{
    Q_OBJECT

public:

    explicit VolumeSlicer(QWindow *parent = 0, char* volumePrefix = "");
    ~VolumeSlicer();

    void ResizeGLWindow(int windowWidth, int windowHeight);

protected:
    /**
     * @brief Initialize
     */
    void Initialize();

    /**
     * @brief Render
     */
    void Render();

    /**
     * @brief keyPressEvent
     * @param event
     */
    void keyPressEvent(QKeyEvent *event);

private:

    /**
     * @brief ReadHeader
     */
    void ReadHeader();

    /**
     * @brief ReadVolume
     */
    void ReadVolume();

    /**
     * @brief InitializeVolume
     */
    void InitializeVolume();

    /**
     * @brief LoadVolumeTextures
     */
    void LoadVolumeTextures();

    /**
     * @brief SetDisplayList
     */
    void SetDisplayList();

    /**
     * @brief RenderFrame
     */
    void RenderFrame();

private:

    /** \brief Volume prefix */
    char* volumePrefix_;

    /** \brief volume width */
    int volumeWidth_;

    /** \brief Volume height */
    int volumeHeight_;

    /** \brief Volume depth */
    int volumeDepth_;

    /** \brief Volume scale */
    float volumeScale_;

    /** \brief Volume raw image */
    GLubyte* rawVolume_;

    /** \brief Volume image with RGBA components */
    GLubyte* rgbaVolume_;

    /** \brief Volume texture ID */
    GLuint volumeTextureId_;

    /** \brief Sampling step */
    float samplingStep_;

    /** \brief Display list */
    GLuint displayList_;
};

#endif // TEXTUREMAPPINGWINDOW_H
