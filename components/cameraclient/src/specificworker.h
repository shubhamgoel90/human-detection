/*
 *    Copyright (C)2019 by YOUR NAME HERE
 *
 *    This file is part of RoboComp
 *
 *    RoboComp is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    RoboComp is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with RoboComp.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
       \brief
       @author authorname
*/



#ifndef SPECIFICWORKER_H
#define SPECIFICWORKER_H

#include <genericworker.h>
#include <innermodel/innermodel.h>
#include <ipcamreader.h>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGridLayout>
#include <QGraphicsEllipseItem>
//#define URL "http://192.168.0.100:88/cgi-bin/CGIStream.cgi?cmd=GetMJStream&usr=guest&pwd=smpt00"
#define URL "http://10.253.247.24:88/cgi-bin/CGIStream.cgi?cmd=GetMJStream&usr=guest&pwd=smpt00"
#define IWIDTH 640//1280
#define IHEIGHT 480//960

using SKELETON_CONNECTIONS = std::vector<std::tuple<std::string, std::string>>;
using HUMAN_JOINT_HEIGHTS = std::map<std::string, double>;


class SpecificWorker : public GenericWorker
{
Q_OBJECT
public:
	SpecificWorker(TuplePrx tprx);
	~SpecificWorker();
	bool setParams(RoboCompCommonBehavior::ParameterList params);

	void CameraSimple_getImage(RoboCompCameraSimple::TImage &im);
	void checkPersonImage(cv::Mat frame, std::string camera);
	void mouseClick(int  event, int  x, int  y);
	void createRemap(int width, int height, float K1, float K2, float K3);
	void readFrameLive(int camera, cv::Mat &frame);
	void readFrameVideo(int camera, cv::Mat &frame);
	void initVideoLive();
	void initVideoReader();
	
public slots:
	void compute();
	void initialize(int period);

private:
	cv::Mat map1, map2, map1b, map2b, mtx, dist, newcameramtx;
	RoboCompCommonBehavior::ParameterList config_params;
	const float LEFT = -2000, BOTTOM = -2000, WIDTH = 4000, HEIGHT = 4000;
	QGraphicsScene scene;
	QGraphicsView view;
	QGraphicsEllipseItem* personPose;
	bool newPoint =false;

	RoboCompPeopleServer::KeyPoint keypoint;
	shared_ptr<InnerModel> innermodel;
	SKELETON_CONNECTIONS skeleton;
	HUMAN_JOINT_HEIGHTS joint_heights;
	float scale;
	IPCamReader cam;
	int frame_counter;
	cv::VideoCapture camcv1;
	cv::VideoCapture camcv2;
	cv::VideoWriter videoWriter1;
	cv::VideoWriter videoWriter2;
	cv::Mat fgMaskMOG2; //fg mask fg mask generated by MOG2 method
	cv::Ptr<cv::BackgroundSubtractorMOG2> pMOG2; //MOG2 Background subtractor
	cv::Mat kernel, erode, dilate;
	void drawBody(cv::Mat frame, const RoboCompPeopleServer::People &people, std::string camera);
	QVec getFloorCoordinates(const RoboCompPeopleServer::Person &person, const std::string &camera);
	std::tuple<bool, QVec> inverseRay(const RoboCompPeopleServer::Person &p, const std::string &joint, const std::string &camera);
	QMat K, Ki;
};

//OpenCV Mouse callback
static void mouse_callback(int event, int x, int y, int, void* userdata)
{
	if  ( event == cv::EVENT_LBUTTONDOWN )
	{
		// Check for null pointer in userdata and handle the error
		SpecificWorker* worker = reinterpret_cast<SpecificWorker*>(userdata);
		worker->mouseClick(event, x, y);
	}
}


#endif
