/*
 *
 * Copyright 2015 gRPC authors.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include <iostream>
#include <memory>
#include <string>
#include <QApplication>

#include "MainWindow.h"

#include <osgViewer/CompositeViewer>
#include <osgViewer/Viewer>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osg/DrawPixels>
#include <osg/Geode>
#include <osg/MatrixTransform>
#include <osgViewer/ViewerEventHandlers>
#include <osgGA/StateSetManipulator>
#include <osg/GraphicsContext>
#include <osgGA/TrackballManipulator>
#include <osgUtil/Optimizer>
#include <osg/Texture2D>
#include <osg/TexGen>
#include <osg/TexEnv>
#include <osg/BlendFunc>
#include <osg/BlendColor>
#include <osg/Billboard>
#include <osg/PositionAttitudeTransform>
#include <osg/ComputeBoundsVisitor>
#include <osg/ShapeDrawable>
#include <osg/LineWidth>
#include <osg/ConvexPlanarPolygon>
#include <osg/ClipNode>
#include <osg/Scissor>
#include <osgUtil/SmoothingVisitor>

//std::string strFileName = "/data/test/learnOsg/Depend/Data/OpenSceneGraph-Data-3.0.0/glider.osgt";
//osg::ref_ptr<osg::Node> node = osgDB::readNodeFile(strFileName);



// 下面提供一个简单的漫游器示例，这个漫游器实现了当用户使用鼠标滚轮滚动时，视点沿着鼠标位置进行zoom in和zoom out


int main(int argc, char** argv)
{


    std::cout<<"zty"<<std::endl;
    osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();
    //viewer->setThreadingModel(osgViewer::ViewerBase::SingleThreaded);
    osg::Group *root = new osg::Group();

    osg::ref_ptr <osg::Node> b25 = osgDB::readNodeFile("/data/test/workData/thirdparty/OpenSceneGraph-Data-3.0.0/cow.osgt") ;
    root->addChild(b25.get());


    ZoomManipulator* pp = new ZoomManipulator(root);
    viewer->setCameraManipulator(pp);

    viewer->setUpViewInWindow(0,0,800,600);
    viewer->setSceneData(root);
    viewer->realize();
    viewer->run();


    //    QApplication app(argc,argv);
    //    MainWindow mainWindow;
    //    mainWindow.showMaximized();
    //    return app.exec();



    return 0;
}
