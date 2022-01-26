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

#include <osgAnimation/Interpolator>
#include <osgAnimation/EaseMotion>
#include <osg/Billboard>
#include <osg/PositionAttitudeTransform>
#include <osg/ComputeBoundsVisitor>
#include <osg/ShapeDrawable>
#include <osg/LineWidth>
#include <osg/ConvexPlanarPolygon>
#include <osg/ClipNode>
#include <osg/Scissor>
#include <osgUtil/SmoothingVisitor>


class FadeCallback:public osg::NodeCallback
{
public:
    FadeCallback()
    {
        _motion=new osgAnimation::InOutCubicMotion;
    }
    virtual void operator()(osg::Node* node,osg::NodeVisitor* nv)
    {
        osg::Geode* geode = dynamic_cast<osg::Geode*>(node);
        if (geode)
        {
            osg::ShapeDrawable* shape = dynamic_cast<osg::ShapeDrawable*>(geode->getDrawable(0));
            if (shape)
            {
                _motion->update(0.005);
                float alpha = _motion->getValue();
                if (alpha>1.0)
                {
                    alpha=2.0-alpha;
                }
                shape->setColor(osg::Vec4(1.0,1.0,1.0,alpha));
            }
        }
        traverse(node,nv);
    }
protected:
    osg::ref_ptr<osgAnimation::InOutCubicMotion> _motion;
};
int main()
{
    osg::ref_ptr<osg::ShapeDrawable> shape = new osg::ShapeDrawable(new osg::Box(osg::Vec3(0.0,0.0,0.0),1.0));

    shape->setColor(osg::Vec4(1.0,1.0,1.0,1.0));
    shape->setSupportsDisplayList(false);

    osg::ref_ptr<osg::Geode> geode=new osg::Geode;
    geode->addDrawable(shape.get());
    geode->setUpdateCallback(new FadeCallback);
    geode->getOrCreateStateSet()->setAttributeAndModes(new osg::BlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA));
    osgViewer::Viewer viewer;
    viewer.setSceneData(geode.get());

    return viewer.run();
}
