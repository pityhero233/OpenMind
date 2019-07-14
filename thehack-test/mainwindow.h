#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include<boost/interprocess/shared_memory_object.hpp>
#include<boost/interprocess/mapped_region.hpp>
#include <QMainWindow>
#include<Qt3DCore>
#include<Qt3DExtras>
#include<Qt3DInput>
#include<Qt3DRender>
#include<iostream>
#include<vector>
#include<QTimer>

#define POINTS_SAMPLE_RATE 200 //5fps
typedef QVector3D ivec;
using namespace std;
using namespace Qt;
namespace core = Qt3DCore;
namespace ext = Qt3DExtras;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public slots:
    void onPointGettyTimeoff(){
        refreshLocationAndRender();
    };

public:
    explicit MainWindow(QWidget *parent = nullptr);
    core::QEntity* scene;
    ~MainWindow();
    Qt3DRender::QCamera* cam;
    void render(core::QEntity* root){
        Qt3DCore::QEntity* sphereA = new Qt3DCore::QEntity(root);
        Qt3DCore::QEntity* sphereB = new Qt3DCore::QEntity(root);
        Qt3DCore::QEntity* planee = new Qt3DCore::QEntity(root);

        Qt3DExtras::QSphereMesh* meshA = new Qt3DExtras::QSphereMesh;//shape 1
        meshA->setRadius(5);
        meshA->setRings(100);
        meshA->setSlices(20);
        Qt3DExtras::QSphereMesh* meshB = new Qt3DExtras::QSphereMesh;//shape 2
        meshA->setRadius(2);
        meshA->setRings(100);
        meshA->setSlices(20);

        Qt3DExtras::QPlaneMesh* plane = new Qt3DExtras::QPlaneMesh;
        plane->setWidth(500);
        plane->setHeight(500);
        Qt3DExtras::QPhongMaterial* matA = new Qt3DExtras::QPhongMaterial;//material 1
        matA->setAmbient(Qt::red);
        matA->setSpecular(Qt::white);
        matA->setShininess(10);

        Qt3DExtras::QPhongMaterial* matB = new Qt3DExtras::QPhongMaterial;//material 2
        matB->setAmbient(Qt::green);
        matB->setSpecular(Qt::white);
        matB->setShininess(10);

        Qt3DExtras::QPhongMaterial* matPlane = new Qt3DExtras::QPhongMaterial;//material 2
        matPlane->setAmbient(Qt::gray);
        matPlane->setSpecular(Qt::gray);
        matPlane->setShininess(0);

        Qt3DCore::QTransform* transform2 =  new Qt3DCore::QTransform;//location 2
        transform2->setTranslation(QVector3D(0,10,10));
        Qt3DCore::QTransform* transform1 =  new Qt3DCore::QTransform;//location 1
        transform1->setTranslation(QVector3D(0,10,0));
        Qt3DCore::QTransform* transform3 =  new Qt3DCore::QTransform;//location 3
        transform3->setTranslation(QVector3D(0,-10,0));
        sphereA->addComponent(matA);
        sphereA->addComponent(transform1);
        sphereA->addComponent(meshA);
        sphereB->addComponent(matB);
        sphereB->addComponent(transform2);
        sphereB->addComponent(meshB);
        planee->addComponent(plane);
        planee->addComponent(matPlane);
        planee->addComponent(transform3);
    }
    void renderGround(core::QEntity* root){
        Qt3DCore::QEntity* planee = new Qt3DCore::QEntity(root);
        Qt3DExtras::QPlaneMesh* plane = new Qt3DExtras::QPlaneMesh;
        plane->setWidth(500);
        plane->setHeight(500);
        Qt3DExtras::QPhongMaterial* matPlane = new Qt3DExtras::QPhongMaterial;//material 2
        matPlane->setAmbient(Qt::gray);
        matPlane->setSpecular(Qt::gray);
        matPlane->setShininess(0);
        Qt3DCore::QTransform* transform3 =  new Qt3DCore::QTransform;//location 3
        transform3->setTranslation(QVector3D(0,-10,0));
        planee->addComponent(plane);
        planee->addComponent(matPlane);
        planee->addComponent(transform3);
    }
    void renderAxis(core::QEntity* root){
        core::QEntity* axisX = new core::QEntity(root);
        core::QEntity* axisY = new core::QEntity(root);
        core::QEntity* axisZ = new core::QEntity(root);
        ext::QCylinderMesh* axisXMesh = new ext::QCylinderMesh;
        axisXMesh->setLength(10);axisXMesh->setRadius(0.5);
        axisXMesh->setSlices(20);
        ext::QCylinderMesh* axisYMesh = new ext::QCylinderMesh;
        axisYMesh->setLength(10);axisYMesh->setRadius(0.5);
        axisYMesh->setSlices(20);
        ext::QCylinderMesh* axisZMesh = new ext::QCylinderMesh;
        axisZMesh->setLength(10);axisZMesh->setRadius(0.5);
        axisZMesh->setSlices(20);
        ext::QPhongMaterial* greenAxisMaterial = new ext::QPhongMaterial;
        greenAxisMaterial->setAmbient(Qt::green);
        greenAxisMaterial->setSpecular(Qt::green);
        greenAxisMaterial->setShininess(5);
        ext::QPhongMaterial* redAxisMaterial = new ext::QPhongMaterial;
        redAxisMaterial->setAmbient(Qt::red);
        redAxisMaterial->setSpecular(Qt::red);
        redAxisMaterial->setShininess(5);
        ext::QPhongMaterial* yellowAxisMaterial = new ext::QPhongMaterial;
        yellowAxisMaterial->setAmbient(Qt::yellow);
        yellowAxisMaterial->setSpecular(Qt::yellow);
        yellowAxisMaterial->setShininess(5);
        core::QTransform* transX = new core::QTransform;
        transX->setTranslation(ivec(10,0,0));
        transX->setRotation(QQuaternion::fromAxisAndAngle(ivec(10,0,0),10));
        axisX->addComponent(axisXMesh);
        axisX->addComponent(greenAxisMaterial);
    }
    void refreshLocationAndRender(){//visit 3 shared memories and seek data
        float* tot1 = static_cast<float*>(p1reg->get_address());
        if (fabs(*tot1-x)>3){
//          //printf("newp ready to  draw.\n");
            x = *tot1;
            float* tot2 = static_cast<float*>(p2reg->get_address());
            y = *tot2;
            float* tot3 = static_cast<float*>(p3reg->get_address());
            z = *tot3;
            //printf("%f %f %f\n",x,y,z);
            //update points:
//            handVec.push_back(ivec(x,y,z));
//            refreshCylinder(scene,lastPoint,ivec(x,y,z),1);
            drawLine(lastPoint,ivec(x,y,z),Qt::yellow,scene);
            refreshBall(scene,ivec(x,y,z));
            lastPoint = ivec(x,y,z);
            cam->setViewCenter(lastPoint);
        }

    }
    inline float mid(float a,float b){
        return (a+b)/2.0;
    }
    void refreshCylinder(core::QEntity* root,ivec beg,ivec end,float r=0.5){//draw a cylinder that uses beg and end as circle center and has a radius of r.
        core::QEntity* brush = new core::QEntity(root);

        ext::QCylinderMesh* mesh = new ext::QCylinderMesh;
        auto length = sqrt((end[0]-beg[0])*(end[0]-beg[0])+(end[1]-beg[1])*(end[1]-beg[1])+(end[2]-beg[2])*(end[2]-beg[2]));
        mesh->setLength(length);mesh->setRadius(r);

        core::QTransform* trans = new core::QTransform;
        trans->setTranslation(ivec(mid(beg[0],end[0]),mid(beg[0],end[0]),mid(beg[0],end[0])));
        //calculate rotations
        ivec delta = end-beg;
        const float deg2rad = 180 / 3.14159265;
//        trans->setRotationX(cos(delta[0]/length*deg2rad));
//        trans->setRotationY(cos(delta[1]/length*deg2rad));
//        trans->setRotationZ(cos(delta[2]/length*deg2rad));
        trans->setRotation(QQuaternion::fromDirection(delta,ivec(0,0,1)));
        brush->addComponent(mesh);
        brush->addComponent(trans);
        brush->addComponent(matVec[totColor]);
    }
    void refreshBall(core::QEntity* root,ivec p,float r=1.0){
        core::QEntity* ball = new core::QEntity(root);
        ext::QSphereMesh* mesh = new ext::QSphereMesh;
        mesh->setRadius(r);
        core::QTransform* trans = new core::QTransform;
        trans->setTranslation(p);
        ball->addComponent(mesh);
        ball->addComponent(trans);
        ball->addComponent(matVec[totColor]);
    }
    void initVec(){
        //init scene
        //init shared memory
        lastPoint = ivec(0,0,0);
        shared_p1 = new boost::interprocess::shared_memory_object(boost::interprocess::open_or_create,"shared_p1",boost::interprocess::read_write);
        shared_p2 = new boost::interprocess::shared_memory_object(boost::interprocess::open_or_create,"shared_p2",boost::interprocess::read_write);
        shared_p3 = new boost::interprocess::shared_memory_object(boost::interprocess::open_or_create,"shared_p3",boost::interprocess::read_write);
        shared_p1->truncate(1024);p1reg = new boost::interprocess::mapped_region(*shared_p1,boost::interprocess::read_only);
        shared_p2->truncate(1024);p2reg = new boost::interprocess::mapped_region(*shared_p2,boost::interprocess::read_only);
        shared_p3->truncate(1024);p3reg = new boost::interprocess::mapped_region(*shared_p3,boost::interprocess::read_only);
        cout<<"The LOC:" << x <<" "<<y <<" "<<z<<"\n";
        //init timer
        //purple red green black gray
        ext::QPhongMaterial* purpleMat = new ext::QPhongMaterial;
        purpleMat->setAmbient(QColor("purple"));
        purpleMat->setSpecular(QColor("purple"));
        purpleMat->setShininess(0.5);
        this->matVec.push_back(purpleMat);
        ext::QPhongMaterial* redMat = new ext::QPhongMaterial;
        redMat->setAmbient(Qt::red);
        redMat->setSpecular(Qt::red);
        redMat->setShininess(0.5);
        this->matVec.push_back(redMat);
        ext::QPhongMaterial* greenMat = new ext::QPhongMaterial;
        greenMat->setAmbient(Qt::green);
        greenMat->setSpecular(Qt::green);
        greenMat->setShininess(0.5);
        this->matVec.push_back(greenMat);
        ext::QPhongMaterial* blackMat = new ext::QPhongMaterial;
        blackMat->setAmbient(Qt::black);
        blackMat->setSpecular(Qt::black);
        blackMat->setShininess(0.5);
        this->matVec.push_back(blackMat);
        ext::QPhongMaterial* grayMat = new ext::QPhongMaterial;
        grayMat->setAmbient(Qt::gray);
        grayMat->setSpecular(Qt::gray);
        grayMat->setShininess(0.5);
        this->matVec.push_back(grayMat);
        pointGetter = new QTimer;
        pointGetter->stop();
        pointGetter->setInterval(POINTS_SAMPLE_RATE);
         connect(pointGetter,SIGNAL(timeout()),this,SLOT(onPointGettyTimeoff()));
        pointGetter->start();
    }
private:
    QTimer* pointGetter;
    Ui::MainWindow *ui;
    boost::interprocess::shared_memory_object* shared_p1;
    boost::interprocess::shared_memory_object* shared_p2;
    boost::interprocess::shared_memory_object* shared_p3;
    boost::interprocess::mapped_region* p1reg;
    boost::interprocess::mapped_region* p2reg;
    boost::interprocess::mapped_region* p3reg;
    float x;
    float y;
    float z;
    vector<ext::QPhongMaterial*> matVec;
    vector<QVector3D> handVec;
    ivec lastPoint;
    int totColor=1;

    //func

    void drawLine(const QVector3D& start, const QVector3D& end, const QColor& color, Qt3DCore::QEntity *_rootEntity)
    {
        auto *geometry = new Qt3DRender::QGeometry(_rootEntity);

        // position vertices (start and end)
        QByteArray bufferBytes;
        bufferBytes.resize(3 * 2 * sizeof(float)); // start.x, start.y, start.end + end.x, end.y, end.z
        float *positions = reinterpret_cast<float*>(bufferBytes.data());
        *positions++ = start.x();
        *positions++ = start.y();
        *positions++ = start.z();
        *positions++ = end.x();
        *positions++ = end.y();
        *positions++ = end.z();
        auto *buf = new Qt3DRender::QBuffer(Qt3DRender::QBuffer::VertexBuffer, geometry);
        buf->setData(bufferBytes);


        auto *positionAttribute = new Qt3DRender::QAttribute(geometry);
        positionAttribute->setName(Qt3DRender::QAttribute::defaultPositionAttributeName());
        positionAttribute->setVertexBaseType(Qt3DRender::QAttribute::Float);
        positionAttribute->setVertexSize(3);
        positionAttribute->setAttributeType(Qt3DRender::QAttribute::VertexAttribute);
        positionAttribute->setBuffer(buf);
        positionAttribute->setByteStride(3 * sizeof(float));
        positionAttribute->setCount(2);
        geometry->addAttribute(positionAttribute); // We add the vertices in the geometry

        // connectivity between vertices
        QByteArray indexBytes;
        indexBytes.resize(2 * sizeof(unsigned int)); // start to end
        unsigned int *indices = reinterpret_cast<unsigned int*>(indexBytes.data());
        *indices++ = 0;
        *indices++ = 1;

        //auto *indexBuffer = new Qt3DRender::QBuffer(geometry);
        auto *indexBuffer = new Qt3DRender::QBuffer(Qt3DRender::QBuffer::IndexBuffer, geometry);
        indexBuffer->setData(indexBytes);

        auto *indexAttribute = new Qt3DRender::QAttribute(geometry);
        indexAttribute->setVertexBaseType(Qt3DRender::QAttribute::UnsignedInt);
        indexAttribute->setAttributeType(Qt3DRender::QAttribute::IndexAttribute);
        indexAttribute->setBuffer(indexBuffer);
        indexAttribute->setCount(2);
        geometry->addAttribute(indexAttribute); // We add the indices linking the points in the geometry

        // mesh
        auto *line = new Qt3DRender::QGeometryRenderer(_rootEntity);
        line->setGeometry(geometry);
        line->setPrimitiveType(Qt3DRender::QGeometryRenderer::Lines);
        auto *material = new Qt3DExtras::QPhongMaterial(_rootEntity);
        material->setAmbient(color);

        // entity
        auto *lineEntity = new Qt3DCore::QEntity(_rootEntity);
        lineEntity->addComponent(line);
        lineEntity->addComponent(material);
    }
};

#endif // MAINWINDOW_H
