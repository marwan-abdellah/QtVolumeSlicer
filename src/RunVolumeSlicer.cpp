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

#include <QApplication>
#include <QSurfaceFormat>
#include <QMessageBox>
#include <iostream>
#include "VolumeSlicer.h"

int main(int argc, char *argv[])
{

    // Make sure that a compatible volume
    if (argc < 2) {
        QApplication application(argc, argv);

        QMessageBox errorMessage;
        errorMessage.setText("No compatible volume was provided.");
        errorMessage.setInformativeText("VolumeSlicer <VOLUME_PREFIX>");
        errorMessage.setStandardButtons(QMessageBox::Close);
        return errorMessage.exec();
    }

    // If the volume was provided, then create a _VolumeSlicer_ application
    QGuiApplication uiApplication(argc, argv);

    VolumeSlicer* slicer = new VolumeSlicer(0, argv[1]);
    QSurfaceFormat format;
    format.setSamples(16);
    slicer->setFormat(format);
    slicer->show();
    slicer->ToogleAnimation(true);

    return uiApplication.exec();
}
