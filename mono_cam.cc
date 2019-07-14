#include <iostream>
#include <algorithm>
#include <fstream>
#include <chrono>
#include <stdio.h>
#include <opencv2/opencv.hpp>

#include <System.h>
#include <time.h>
#include<boost/interprocess/shared_memory_object.hpp>
#include<boost/interprocess/mapped_region.hpp>
using namespace std;
using namespace boost::interprocess;

int main(int argc, char **argv)
{
    cout << "creating shared memory obj..";
    shared_memory_object share1(open_or_create,"shared_p1",read_write);
    shared_memory_object share2(open_or_create,"shared_p2",read_write);
    shared_memory_object share3(open_or_create,"shared_p3",read_write);
    share1.truncate(1024);mapped_region reg1(share1,read_write);
    share2.truncate(1024);mapped_region reg2(share2,read_write);
    share3.truncate(1024);mapped_region reg3(share3,read_write);
    double* p1 = static_cast<double*>(reg1.get_address());
    double* p2 = static_cast<double*>(reg2.get_address());
    double* p3 = static_cast<double*>(reg3.get_address());
    *p1 = 3.333333333333333333333;*p2 = 4.444444444444444444444;*p3 = 5.5555555555555555;
    cv::VideoCapture cap(0);

    // Create SLAM system. It initializes all system threads and gets ready to process frames.
    ORB_SLAM2::System SLAM("ORBvoc.txt","cam.yaml",ORB_SLAM2::System::MONOCULAR,true);

    cout << endl << "-------" << endl;
    cout << "Start processing sequence ..." << endl;

    double start_time = time(NULL);
    // Main loop
    cv::Mat im;
    while (true)
    {
        cap.read(im);
        if(im.empty())
        {
            cerr << "Empty image" << endl;
            return 1;
        }

        double tframe = time(NULL) - start_time;


        // Pass the image to the SLAM system
        auto pos = SLAM.TrackMonocular(im,tframe);
	//if (!pos.empty()) cout<<pos.at<double>(4,1)<<" "<<pos.at<double>(4,2)<<" "<<pos.at<double>(4,3)<<endl;
   	if (!pos.empty()) printf("%.10lf %.10lf %.10lf\n",pos.at<double>(4,1),pos.at<double>(4,2),pos.at<double>(4,3));
	if (!pos.empty()){
		//report to shared memory
    		*p1 = pos.at<double>(4,1);*p2=pos.at<double>(4,2);*p3=pos.at<double>(4,3);
		printf("pose updated.");
	}
    }
    // Stop all threads
    SLAM.Shutdown();

    // Save camera trajectory
    SLAM.SaveKeyFrameTrajectoryTUM("KeyFrameTrajectory.txt");

    return 0;
}
