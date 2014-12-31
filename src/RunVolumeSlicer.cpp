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
