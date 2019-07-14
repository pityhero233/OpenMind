#include "mainwindow.h"
#include <QApplication>
int main(int argc, char *argv[])
{
    cout << "OpenMind 2019\nVersion 0.1 Alpha\nA product of Dream Blue Team at THE Hack 2019.\nAll Rights Reserved (c) 2019-2022\n";
    cout << "Creating shared memory...\n";

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    Qt3DExtras::Qt3DWindow view;
    w.scene = new core::QEntity;
    w.initVec();//initalize palettes
    w.cam = view.camera();
    w.renderGround(w.scene);
    // camera
    w.cam->lens()->setPerspectiveProjection(45,16/9,0.1,1000);
    w.cam->setPosition(QVector3D(60,60,60));
    w.cam->setViewCenter(QVector3D(0,0,0));

     //manipulator
    Qt3DExtras::QOrbitCameraController* manipulator = new Qt3DExtras::QOrbitCameraController(w.scene);
    manipulator->setLinearSpeed(100);
    manipulator->setLookSpeed(200);
    manipulator->setCamera(w.cam);

    view.setRootEntity(w.scene);
    view.show();
    return a.exec();
}
