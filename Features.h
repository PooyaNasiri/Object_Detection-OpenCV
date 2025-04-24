#include "headers.h"

void loadModelFeatures(const std::string &modelDir,
                       std::vector<std::vector<cv::KeyPoint>> &keypointsVec,
                       std::vector<cv::Mat> &descriptorVec,
                       std::vector<cv::Mat> &imageVec,
                       std::vector<cv::Mat> &maskVec);

void loadSceneFeatures(const std::string &testDir,
                       std::vector<std::string> &sceneNames,
                       std::vector<std::vector<cv::KeyPoint>> &keypointsVec,
                       std::vector<cv::Mat> &descriptorVec);