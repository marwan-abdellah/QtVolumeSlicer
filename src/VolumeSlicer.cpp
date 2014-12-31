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

#include "VolumeSlicer.h"
#include <fstream>
#include <iostream>
#include <QDebug>

/**
 * @brief VolumeSlicer::VolumeSlicer
 * @param parent
 * @param volumePrefix
 */
VolumeSlicer::VolumeSlicer(QWindow *parent, char *volumePrefix) :
    OpenGLWindow(parent),
    volumePrefix_(volumePrefix),
    volumeScale_(1.0) { }

/**
 * @brief VolumeSlicer::~VolumeSlicer
 */
VolumeSlicer::~VolumeSlicer()
{
    delete [] rgbaVolume_;
}

/**
 * @brief VolumeSlicer::ReadHeader
 */
void VolumeSlicer::ReadHeader()
{
    // Format the file header
    char hdrFile[300];
    sprintf(hdrFile, "%s.hdr", volumePrefix_);

    // Open the file
    std::ifstream hdrStream;
    hdrStream.open(hdrFile, std::ios::in);

    if (hdrStream.fail()) {
        // qDebug << "Could not open the header file " << hdrFile << endl;
        exit(0);
    }

    // Read the volume header
    hdrStream >> volumeWidth_;
    hdrStream >> volumeHeight_;
    hdrStream >> volumeDepth_;

    // Close the stream
    hdrStream.close();
}

/**
 * @brief VolumeSlicer::ReadVolume
 */
void VolumeSlicer::ReadVolume()
{
    // Read the header file to extract the volume dimensions
    ReadHeader();

    // Form the volume file path string
    char imgFile[100];
    sprintf(imgFile, "%s.img", volumePrefix_);

    // Volume 3D size
    int volume3dSize = volumeWidth_*volumeHeight_*volumeDepth_;

    // Allocate the raw volume and RGBA volumes
    rawVolume_  = new GLubyte [volume3dSize];
    rgbaVolume_ = new GLubyte [volume3dSize * 4];

    // Read the volume file from the input stream
    std::ifstream imgStream;
    imgStream.open(imgFile, std::ios::in | std::ios::binary);
    if (imgStream.fail()) {
        qDebug() << "Could not open the volume file " << imgFile;
        exit(0);
    }
    imgStream.read((char *)rawVolume_, volume3dSize);

    // Close the stream
    imgStream.close();

    // Put a box around the volume so that we can see the outline
    // of the data.
    GLubyte *ptr = rawVolume_;
    int i, j, k;
    for (i = 0; i < volumeDepth_; i++) {
        for (j = 0; j < volumeHeight_; j++) {
            for (k = 0; k < volumeWidth_; k++) {
                if (((i < 4) && (j < 4)) ||
                        ((j < 4) && (k < 4)) ||
                        ((k < 4) && (i < 4)) ||
                        ((i < 4) && (j >  volumeHeight_-5)) ||
                        ((j < 4) && (k > volumeWidth_-5)) ||
                        ((k < 4) && (i > volumeDepth_-5)) ||
                        ((i > volumeDepth_-5) && (j >  volumeHeight_-5)) ||
                        ((j >  volumeHeight_-5) && (k > volumeWidth_-5)) ||
                        ((k > volumeWidth_-5) && (i > volumeDepth_-5)) ||
                        ((i > volumeDepth_-5) && (j < 4)) ||
                        ((j >  volumeHeight_-5) && (k < 4)) ||
                        ((k > volumeWidth_-5) && (i < 4))) {
                    *ptr = 110;
                }
                ptr++;
            }
        }
    }

    // Classification
    ptr = rawVolume_;
    GLubyte *qtr = rgbaVolume_;
    GLubyte v, val;
    for (i = 0; i < volume3dSize; i++) {
        v = *(ptr++);
        val = (v < 64) ? 0 : v - 64;
        val = val >> 1;
        *(qtr++) = val;
        *(qtr++) = ((float)val) * 0.93;
        *(qtr++) = ((float)val) * 0.78;
        *(qtr++) = val;
    }

    // Free the raw volume
    delete [] rawVolume_;
}

/**
 * @brief VolumeSlicer::SetDisplayList
 */
void VolumeSlicer::SetDisplayList()
{
    // Size of the slice
    float zSlice;

    // Diagonal size of the slice
    const float diagonalSizeSquared = volumeWidth_*volumeWidth_ +
            volumeHeight_*volumeHeight_ + volumeDepth_*volumeDepth_;
    // Number of slices
    const int halfSlicesMinus1  = 1.3 * sqrt(diagonalSizeSquared) / 4.0;
    const int numSlices         = 2 * halfSlicesMinus1 + 1;

    // Number of vertecies
    const int numVertecies      = numSlices * 4;

    // Slices
    GLfloat *vPoints            = new GLfloat [3 * numVertecies];
    GLfloat *ptr                = vPoints;
    const float sliceArm        = sqrt(3.0) / numSlices;
    const float sliceDistance   = 1;

    for (int i = -halfSlicesMinus1; i <= halfSlicesMinus1; i++) {

        zSlice     = i * sliceArm;

        *(ptr++) = -sliceDistance;
        *(ptr++) = -sliceDistance;
        *(ptr++) =  zSlice;

        *(ptr++) =  sliceDistance;
        *(ptr++) = -sliceDistance;
        *(ptr++) =  zSlice;

        *(ptr++) =  sliceDistance;
        *(ptr++) =  sliceDistance;
        *(ptr++) =  zSlice;

        *(ptr++) = -sliceDistance;
        *(ptr++) =  sliceDistance;
        *(ptr++) =  zSlice;
    }

    // Vertex array
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vPoints);

    // Generate the display list
    displayList_ = glGenLists(1);
    glNewList(displayList_, GL_COMPILE);
    glDrawArrays(GL_QUADS, 0, numVertecies);
    glEndList();

    delete [] vPoints;
}

/**
 * @brief VolumeSlicer::Initialize
 */
void VolumeSlicer::Initialize()
{
    // Read the input volume
    ReadVolume();

    // Upload the volume texture to the GPU
    LoadVolumeTextures();

    // Compile the display list
    SetDisplayList();
}

/**
 * @brief VolumeSlicer::RenderFrame
 */
void VolumeSlicer::RenderFrame()
{
    glEnable(GL_TEXTURE_3D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glBindTexture(GL_TEXTURE_3D, volumeTextureId_);

    glClear(GL_COLOR_BUFFER_BIT);

    // Clip planes
    static GLdouble eqx0[4] = { 1.0, 0.0, 0.0, 0.0};
    static GLdouble eqx1[4] = {-1.0, 0.0, 0.0, 1.0};
    static GLdouble eqy0[4] = {0.0,  1.0, 0.0, 0.0};
    static GLdouble eqy1[4] = {0.0, -1.0, 0.0, 1.0};
    static GLdouble eqz0[4] = {0.0, 0.0,  1.0, 0.0};
    static GLdouble eqz1[4] = {0.0, 0.0, -1.0, 1.0};

    // Define equations for automatic texture coordinate generation
    static GLfloat x[] = {1.0, 0.0, 0.0, 0.0};
    static GLfloat y[] = {0.0, 1.0, 0.0, 0.0};
    static GLfloat z[] = {0.0, 0.0, 1.0, 0.0};

    glPushMatrix ();
    glScalef(volumeScale_, volumeScale_, volumeScale_);

    glPushMatrix ();

    // Transform the viewing direction
    glRotatef(-zRotation_, 0.0, 0.0, 1.0);
    glRotatef(-yRotation_, 0.0, 1.0, 0.0);
    glRotatef(-xRotation_, 1.0, 0.0, 0.0);
    glTranslatef(-0.5, -0.5, -0.5);

    // Take a copy of the model view matrix now shove it in to the GPU
    // buffer for later use in automatic texture coord generation.
    glTexGenfv(GL_S, GL_EYE_PLANE, x);
    glTexGenfv(GL_T, GL_EYE_PLANE, y);
    glTexGenfv(GL_R, GL_EYE_PLANE, z);

    // Take a copy of the model view matrix and shove it in to the GPU
    // buffer for later use in clipping planes.
    glClipPlane(GL_CLIP_PLANE0, eqx0);
    glClipPlane(GL_CLIP_PLANE1, eqx1);
    glClipPlane(GL_CLIP_PLANE2, eqy0);
    glClipPlane(GL_CLIP_PLANE3, eqy1);
    glClipPlane(GL_CLIP_PLANE4, eqz0);
    glClipPlane(GL_CLIP_PLANE5, eqz1);

    glPopMatrix ();

    // Enable clip planes
    glEnable(GL_CLIP_PLANE0);
    glEnable(GL_CLIP_PLANE1);
    glEnable(GL_CLIP_PLANE2);
    glEnable(GL_CLIP_PLANE3);
    glEnable(GL_CLIP_PLANE4);
    glEnable(GL_CLIP_PLANE5);

    // Render enclosing rectangles
    glCallList(displayList_);

    glPopMatrix ();

    glDisable(GL_TEXTURE_3D);
}

/**
 * @brief VolumeSlicer::Render
 */
void VolumeSlicer::Render()
{
    RenderFrame();
}

/**
 * @brief VolumeSlicer::ResizeGLWindow
 * @param windowWidth
 * @param windowHeight
 */
void VolumeSlicer::ResizeGLWindow(int windowWidth, int windowHeight)
{
    // Fix if _windowHeight_ is zero
    if (windowHeight == 0) {
        windowHeight = 1;
    }

    // Adjust the viewing port
    glViewport(0, 0, (GLsizei) windowWidth, (GLsizei) windowHeight);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Orthographic projection
    GLfloat windowSize = 1.0;
    GLfloat aspect = (GLfloat) windowHeight/(GLfloat) windowWidth;
    glOrtho(-windowSize, windowSize,
            -windowSize * aspect, windowSize * aspect,
            -windowSize, windowSize);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    /*
    // Adjust the MVP matrix
    projectionMatrix.setToIdentity();
    projectionMatrix.perspective(45, (float) windowWidth / (float)windowHeight,
                                 1, 1000);
    modelViewMatrix.setToIdentity();
    */
}

/**
 * @brief VolumeSlicer::LoadVolumeTextures
 */
void VolumeSlicer::LoadVolumeTextures()
{
    // Clear buffers
    glClearColor (0.0, 0.0, 0.0, 0.0);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // Generate the volume texture on the GPU
    glGenTextures(1, &volumeTextureId_);
    glBindTexture(GL_TEXTURE_3D, volumeTextureId_);

    // Set the texture parameters
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // For automatic texture coordinate generation
    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
    glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);

    // Upload the texture to the GPU
    glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA,
                 volumeWidth_, volumeHeight_, volumeDepth_,
                 0, GL_RGBA, GL_UNSIGNED_BYTE, rgbaVolume_);

    // Enable automatic texture generation
    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);
    glEnable(GL_TEXTURE_GEN_R);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
}

/**
 * @brief OpenGLWindow::keyPressEvent
 * @param event
 */
void VolumeSlicer::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_F1: {

        // Toggle full screen
        fullScreen_ = !fullScreen_;
        if(fullScreen_) showFullScreen();
        else showNormal();

    } break;

    case Qt::Key_A:
        xRotation_ += 1;
        break;
    case Qt::Key_Z:
        xRotation_ -= 1;
        break;
    case Qt::Key_S:
        yRotation_ += 1;
        break;
    case Qt::Key_X:
        yRotation_ -= 1;
        break;
    case Qt::Key_D:
        zRotation_ += 1;
        break;
    case Qt::Key_C:
        zRotation_ -= 1;
        break;
    case Qt::Key_F:
        volumeScale_ *= 1.1;
        break;
    case Qt::Key_V:
        volumeScale_ /= 1.1;
        break;

    case Qt::Key_Escape:
        qApp->exit();
        break;
    }

    QWindow::keyPressEvent(event);
}
