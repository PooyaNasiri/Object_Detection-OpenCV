/******************************************************************************
 * Project      : Middle Course Test
 * File         : Features.cpp
 * Authors      : Pooya Nasiri , 
 * Emails       : pooya.nasiri@studenti.unipd.it , 
 * Affiliation  : University of Padova – MSc in Computer Vision
 * Supervisor   : Prof. Stefano Ghidoni
 * Date         : 2025-April
 ******************************************************************************/

#include "headers.h"
#include "Features.h"

namespace fs = std::filesystem;

void loadModelFeatures(const std::string &modelDir,
                       std::vector<std::vector<cv::KeyPoint>> &keypointsVec,
                       std::vector<cv::Mat> &descriptorVec,
                       std::vector<cv::Mat> &imageVec,
                       std::vector<cv::Mat> &maskVec)
{
    cv::Ptr<cv::SIFT> sift = cv::SIFT::create();

    for (const auto &entry : fs::directory_iterator(modelDir))
    {
        std::string filename = entry.path().filename().string();
        if (filename.find("_color.png") == std::string::npos)
            continue;

        std::string colorPath = entry.path().string();
        std::string maskPath = colorPath;
        size_t pos = maskPath.find("_color.png");
        if (pos != std::string::npos)
            maskPath.replace(pos, std::string("_color.png").length(), "_mask.png");

        cv::Mat color = cv::imread(colorPath, cv::IMREAD_COLOR);
        cv::Mat mask = cv::imread(maskPath, cv::IMREAD_GRAYSCALE);

        if (color.empty() || mask.empty())
        {
            std::cerr << "Error loading: " << colorPath << " or " << maskPath << std::endl;
            continue;
        }

        maskVec.push_back(mask);

        cv::Mat gray;
        cv::cvtColor(color, gray, cv::COLOR_BGR2GRAY);

        std::vector<cv::KeyPoint> keypoints;
        cv::Mat descriptors;
        sift->detectAndCompute(gray, mask, keypoints, descriptors);

        keypointsVec.push_back(keypoints);
        descriptorVec.push_back(descriptors);
        imageVec.push_back(color);

        std::cout << "Model view: " << filename << " -> " << keypoints.size() << " keypoints." << std::endl;
    }
}

void loadSceneFeatures(const std::string &testDir,
                       std::vector<std::string> &sceneNames,
                       std::vector<std::vector<cv::KeyPoint>> &keypointsVec,
                       std::vector<cv::Mat> &descriptorVec)
{
    cv::Ptr<cv::SIFT> sift = cv::SIFT::create();

    for (const auto &entry : fs::directory_iterator(testDir))
    {
        std::string path = entry.path().string();
        if (entry.path().extension() != ".jpg")
            continue;

        cv::Mat color = cv::imread(path, cv::IMREAD_COLOR);
        if (color.empty())
        {
            std::cerr << "Could not load: " << path << std::endl;
            continue;
        }

        cv::Mat gray;
        cv::cvtColor(color, gray, cv::COLOR_BGR2GRAY);

        std::vector<cv::KeyPoint> keypoints;
        cv::Mat descriptors;
        sift->detectAndCompute(gray, cv::noArray(), keypoints, descriptors);

        keypointsVec.push_back(keypoints);
        descriptorVec.push_back(descriptors);
        sceneNames.push_back(entry.path().filename().string());

        std::cout << "Scene: " << entry.path().filename() << " -> " << keypoints.size() << " keypoints." << std::endl;
    }
}
