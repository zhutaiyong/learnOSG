#pragma once
#include <QHBoxLayout>
#include <QMainWindow>

#include <osgGA/CameraManipulator>

class  MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    virtual ~MainWindow();

};


class ZoomManipulator : public osgGA::CameraManipulator
{
public:
    //构造函数传入节点计算包围盒
    ZoomManipulator(osg::Node *node);
    ~ZoomManipulator();

    //所有漫游器都必须实现的4个纯虚函数
    virtual void setByMatrix(const osg::Matrixd& matrix){}
    virtual void setByInverseMatrix(const osg::Matrixd& matrix){}
    virtual osg::Matrixd getMatrix() const{return osg::Matrix();}
    virtual osg::Matrixd getInverseMatrix() const;

    //获取传入节点，用于使用CameraManipulator中的computeHomePosition
    virtual const osg::Node* getNode() const { return _root; }
    virtual osg::Node* getNode() { return _root;  }

    virtual bool handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& us);

    osg::Vec3	_eye;				//视点位置
    osg::Vec3   _direction;         //视点方向
    osg::Vec3	_up;                //向上方向
    osg::Node*	_root;
};
