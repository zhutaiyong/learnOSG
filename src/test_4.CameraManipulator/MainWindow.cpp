#include "MainWindow.h"
#include <osgViewer/Viewer>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
}

MainWindow::~MainWindow()
{

}


ZoomManipulator::ZoomManipulator(osg::Node *node)
{
    _root = node;
    computeHomePosition();

    _eye = _homeEye;
    _direction = _homeCenter - _homeEye;
    _up = _homeUp;
}

ZoomManipulator::~ZoomManipulator()
{

}

osg::Matrixd ZoomManipulator::getInverseMatrix() const
{
    osg::Matrix mat;
    mat.makeLookAt(_eye, _eye + _direction, _up);

    return mat;
}


bool ZoomManipulator::handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& us)
{
    switch(ea.getEventType())
    {

    case (osgGA::GUIEventAdapter::SCROLL):
        {
            osgViewer::Viewer *viewer = dynamic_cast<osgViewer::Viewer*>(&us);
            osg::Camera *camera = viewer->getCamera();
            osg::Matrix MVPW = camera->getViewMatrix() * camera->getProjectionMatrix() * camera->getViewport()->computeWindowMatrix();

            osg::Matrix inverseMVPW = osg::Matrix::inverse(MVPW);
            osg::Vec3 mouseWorld = osg::Vec3(ea.getX(), ea.getY(), 0) * inverseMVPW;

            osg::Vec3 direction = mouseWorld - _eye;
            direction.normalize();

            if (ea.getScrollingMotion() == osgGA::GUIEventAdapter::SCROLL_UP)
            {
                _eye += direction * 2.0;
            }
            else if (ea.getScrollingMotion() == osgGA::GUIEventAdapter::SCROLL_DOWN)
            {
                _eye -= direction * 2.0;
            }
        }

    default:
        return false;
    }
}

