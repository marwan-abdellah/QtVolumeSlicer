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
