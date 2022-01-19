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



class FindTextureVisitor : public osg::NodeVisitor
{
public:
    FindTextureVisitor( osg::Texture* tex ) : _texture(tex)
    {
        setTraversalMode( osg::NodeVisitor::TRAVERSE_ALL_CHILDREN );
    }

    virtual void apply( osg::Node& node )
    {
        replaceTexture( node.getStateSet() );
        traverse( node );
    }

    virtual void apply( osg::Geode& geode )
    {
        replaceTexture( geode.getStateSet() );
        for ( unsigned int i=0; i<geode.getNumDrawables(); ++i )
        {
            replaceTexture( geode.getDrawable(i)->getStateSet() );
        }
        traverse( geode );
    }

    void replaceTexture( osg::StateSet* stateset )
    {
        if ( stateset )
        {
            osg::Texture* oldTexture = dynamic_cast<osg::Texture*>(
                        stateset->getTextureAttribute(0, osg::StateAttribute::TEXTURE) );
            if ( oldTexture ) stateset->setTextureAttribute( 0, _texture.get() );
        }
    }

protected:
    osg::ref_ptr<osg::Texture> _texture;
};




int main(int argc, char** argv)
{

    osg::ref_ptr<osg::Node> model = osgDB::readNodeFile( "/data/test/learnOsg/Depend/Data/OpenSceneGraph-Data-3.0.0/lz.osgt" );
    osg::ref_ptr<osg::Node> sub_model = osgDB::readNodeFile( "/data/test/learnOsg/Depend/Data/OpenSceneGraph-Data-3.0.0/glider.osgt" );

    int tex_width = 1024, tex_height = 1024;

    osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D;
    texture->setTextureSize( tex_width, tex_height );
    texture->setInternalFormat( GL_RGBA );
    texture->setFilter( osg::Texture2D::MIN_FILTER, osg::Texture2D::LINEAR );
    texture->setFilter( osg::Texture2D::MAG_FILTER, osg::Texture2D::LINEAR );

    FindTextureVisitor ftv( texture.get() );
    if ( model.valid() ) model->accept( ftv );

    osg::ref_ptr<osg::Camera> camera = new osg::Camera;
    camera->setViewport( 0, 0, tex_width, tex_height );
    camera->setClearColor( osg::Vec4(1.0f, 1.0f, 1.0f, 0.0f) );
    camera->setClearMask( GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT );

    camera->setRenderOrder( osg::Camera::PRE_RENDER );
    camera->setRenderTargetImplementation( osg::Camera::FRAME_BUFFER_OBJECT );
    camera->attach( osg::Camera::COLOR_BUFFER, texture.get() );

    camera->setReferenceFrame( osg::Camera::ABSOLUTE_RF );
    camera->addChild( sub_model.get() );

    osg::ref_ptr<osg::Group> root = new osg::Group;
    root->addChild( model.get() );
    root->addChild( camera.get() );

    osgViewer::Viewer viewer;
    viewer.setSceneData( root.get() );
    viewer.setCameraManipulator( new osgGA::TrackballManipulator);

    float delta = 0.1f, bias = 0.0f;
    osg::Vec3 eye(0.0f,-5.0f, 5.0f);
    while ( !viewer.done() )
    {
        if ( bias<-1.0f ) delta = 0.1f;
        else if ( bias>1.0f ) delta = -0.1f;
        bias += delta;
        camera->setViewMatrixAsLookAt( eye, osg::Vec3(), osg::Vec3(bias, 1.0f, 1.0f) );

        viewer.frame();
    }



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
