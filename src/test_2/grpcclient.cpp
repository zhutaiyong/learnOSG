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


osg::Geode* createTexturePanel()
{
    osg::Vec3Array *vertices = new osg::Vec3Array;
    vertices->push_back(osg::Vec3(-1.0, -1.0, 0.0));
    vertices->push_back(osg::Vec3(1.0, -1.0, 0.0));
    vertices->push_back(osg::Vec3(1.0, 1.0, 0.0));
    vertices->push_back(osg::Vec3(-1.0, 1.0, 0.0));

    osg::Vec2Array *texcoord = new osg::Vec2Array;
    texcoord->push_back(osg::Vec2(0.0, 0.0));
    texcoord->push_back(osg::Vec2(1.0, 0.0));
    texcoord->push_back(osg::Vec2(1.0, 1.0));
    texcoord->push_back(osg::Vec2(0.0, 1.0));

    osg::Geometry *geom = new osg::Geometry;
    geom->setVertexArray(vertices);
    geom->setTexCoordArray(0, texcoord);
    geom->addPrimitiveSet(new osg::DrawArrays(GL_QUADS, 0, 4));

    osg::Geode *geode = new osg::Geode;
    geode->addDrawable(geom);
    osg::StateSet *set1 = geode->getOrCreateStateSet();
    //geode->getOrCreateStateSet()->setTextureAttributeAndModes(0, texture, osg::StateAttribute::ON);
    set1->setMode(GL_LIGHTING, osg::StateAttribute::OFF);// 设置不受光照影响, 不然太暗了就看不清

    return geode;
}


osg::Texture2D *createFloatTextureRectangle(int width, int height)
{
    osg::ref_ptr<osg::Texture2D> tex2D = new osg::Texture2D;
    tex2D->setTextureSize(width, height);
    tex2D->setInternalFormat(GL_RGBA16F_ARB);
    tex2D->setSourceFormat(GL_RGBA);
    tex2D->setSourceType(GL_FLOAT);
    return tex2D.release();
}

int main(int argc, char** argv)
{

    osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer;
    osg::ref_ptr<osg::Group> grp = new osg::Group;
    std::string strFileName = "/data/test/learnOsg/Depend/Data/OpenSceneGraph-Data-3.0.0/glider.osgt";
    osg::ref_ptr<osg::Node> node = osgDB::readNodeFile(strFileName);

    //获取系统分辨率
    unsigned int screenWidth, screenHeight;
    {
        osg::GraphicsContext::WindowingSystemInterface *wsInterface = osg::GraphicsContext::getWindowingSystemInterface();
        if (!wsInterface)
        {
                    return -1;
        }
        wsInterface->getScreenResolution(osg::GraphicsContext::ScreenIdentifier(0), screenWidth, screenHeight);

    }
    int texWidth = screenWidth;
    int texHeight = screenHeight;
    // 采样贴图设置:
    osg::Texture2D *textureSample = createFloatTextureRectangle(texWidth, texHeight);

    // 采样像机1设置:
    {

        osg::Camera *cameraSample = new osg::Camera;// 各种采样
        grp->addChild(cameraSample);// 加入场景
        cameraSample->addChild(node);// 加入模型

        cameraSample->setClearColor(osg::Vec4(0.0f, 0.0f, 0.0f, 0.0f));
        cameraSample->setRenderTargetImplementation(osg::Camera::FRAME_BUFFER_OBJECT);// 这句使内容不渲染到屏幕上
        cameraSample->attach(osg::Camera::COLOR_BUFFER0, textureSample);// 关联采样贴图
        cameraSample->setViewport(0, 0, texWidth, texHeight);//

    }
    osg::Geode * panel1 = createTexturePanel();
    panel1->getOrCreateStateSet()->setTextureAttributeAndModes(0, textureSample);
#if 0
    {
                grp->addChild(panel1);
    }
#endif
#if 1
    //最终的摄像机

    {
        osg::ref_ptr<osg::Camera> finalCamera = new osg::Camera;
        finalCamera->addChild(panel1);
        grp->addChild(finalCamera);
        finalCamera->setReferenceFrame(osg::Camera::ABSOLUTE_RF);
    }

#endif
    viewer->setSceneData(grp);

    viewer->run();







    //    std::cout<<"zty"<<std::endl;
    //    osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();
    //    viewer->setThreadingModel(osgViewer::ViewerBase::SingleThreaded);
    //    osg::Group *root = new osg::Group();
    //    osg::ref_ptr <osg::Node> b25 = osgDB::readNodeFile("/data/test/workData/thirdparty/OpenSceneGraph-Data-3.0.0/cow.osgt") ;
    //    root->addChild(b25.get());
    //    viewer->setUpViewInWindow(0,0,800,600);
    //    viewer->setSceneData(root);
    //    viewer->realize();
    //    viewer->run();





    //    QApplication app(argc,argv);
    //    MainWindow mainWindow;
    //    mainWindow.showMaximized();
    //    return app.exec();



    return 0;
}
