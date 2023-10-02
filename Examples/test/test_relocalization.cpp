/**
 * This file is part of ORB-SLAM2.
 *
 * Copyright (C) 2014-2016 Raúl Mur-Artal <raulmur at unizar dot es> (University of Zaragoza)
 * For more information see <https://github.com/raulmur/ORB_SLAM2>
 *
 * ORB-SLAM2 is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ORB-SLAM2 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ORB-SLAM2. If not, see <http://www.gnu.org/licenses/>.
 */

#include <iostream>
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <chrono>
#include <unistd.h>
#include <opencv2/core/core.hpp>

#include <System.h>

#include "InitGlog.h"

using namespace std;

// 基本操作类似 stereo_euroc.cc

void LoadImages(const string &strPathToSequence, vector<string> &vstrImageLeft,
                vector<string> &vstrImageRight, vector<double> &vTimestamps);

int main(int argc, char **argv)
{
    if (argc != 5)
    {
        cerr << endl
             << "Usage: ./stereo_kitti path_to_vocabulary path_to_settings path_to_sequence path_log" << endl;
        return 1;
    }
    const string programName = string(argv[0]);
    const string logPath = string(argv[4]);
    InitGlog outlog(programName, logPath);

    // Retrieve paths to images
    vector<string> vstrImageLeft;
    vector<string> vstrImageRight;
    vector<double> vTimestamps;
    LoadImages(string(argv[3]), vstrImageLeft, vstrImageRight, vTimestamps);

    const int nImages = vstrImageLeft.size();

    // Create SLAM system. It initializes all system threads and gets ready to process frames.
    ORB_SLAM2::System SLAM(argv[1], argv[2], ORB_SLAM2::System::STEREO, true);
    SLAM.LoadMap("./output/map.bin");
    cout << "pause Program! show offline Map！" << endl;

    // Vector for tracking time statistics
    vector<float> vTimesTrack;
    vTimesTrack.resize(nImages);

    LOG(INFO) << "Start processing sequence ...";
    LOG(INFO) << "Images in the sequence: " << nImages;

    /*************************************************************
     * 输入图像帧号，完成relocalization
     *************************************************************/
    cv::Mat imLeft, imRight;
    while (true)
    {
        int frameNumber = 0;
        cerr << "plase input relocalization frame：" << endl;
        cin >> frameNumber;
        cerr << "frameNumber=\t" << frameNumber << endl;
        if (-1 == frameNumber)
        {
            LOG(INFO) << "end relocalization!";
            break;
        }
        imLeft = cv::imread(vstrImageLeft[frameNumber], cv::IMREAD_UNCHANGED);
        imRight = cv::imread(vstrImageRight[frameNumber], cv::IMREAD_UNCHANGED);
        double tframe = vTimestamps[frameNumber];
        if (imLeft.empty() && imRight.empty())
        {
            LOG(ERROR) << frameNumber << " image is empty!";
        }
#ifdef COMPILEDWITHC11
        std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();
#else
        std::chrono::monotonic_clock::time_point t1 = std::chrono::monotonic_clock::now();
#endif
        
        //do work... relocalization
        SLAM.Relocalization(imLeft,tframe);

#ifdef COMPILEDWITHC11
        std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();
#else
        std::chrono::monotonic_clock::time_point t2 = std::chrono::monotonic_clock::now();
#endif

        double ttrack = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1).count();

        usleep(0.5 * 1e6);
    }

    SLAM.Shutdown();
    outlog.ShutDownLog();
    return 0;
}

// 类似 mono_kitti.cc， 不过是生成了双目的图像路径
void LoadImages(const string &strPathToSequence, vector<string> &vstrImageLeft,
                vector<string> &vstrImageRight, vector<double> &vTimestamps)
{
    ifstream fTimes;
    string strPathTimeFile = strPathToSequence + "/times.txt";
    fTimes.open(strPathTimeFile.c_str());
    while (!fTimes.eof())
    {
        string s;
        getline(fTimes, s);
        if (!s.empty())
        {
            stringstream ss;
            ss << s;
            double t;
            ss >> t;
            vTimestamps.push_back(t);
        }
    }

    string strPrefixLeft = strPathToSequence + "/image_0/";
    string strPrefixRight = strPathToSequence + "/image_1/";

    const int nTimes = vTimestamps.size();
    vstrImageLeft.resize(nTimes);
    vstrImageRight.resize(nTimes);

    for (int i = 0; i < nTimes; i++)
    {
        stringstream ss;
        ss << setfill('0') << setw(6) << i;
        vstrImageLeft[i] = strPrefixLeft + ss.str() + ".png";
        vstrImageRight[i] = strPrefixRight + ss.str() + ".png";
    }
}
