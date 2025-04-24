#include "headers.h"

bool findBestHomography(const std::vector<cv::KeyPoint> &sceneKp, const cv::Mat &sceneDesc,
                        const std::vector<std::vector<cv::KeyPoint>> &modelKpList,
                        const std::vector<cv::Mat> &modelDescList,
                        const std::vector<cv::Mat> &modelImages,
                        const std::vector<cv::Mat> &modelMasks,
                        const cv::Mat &sceneImage,
                        cv::Mat &bestH, std::vector<cv::Point2f> &projectedBox);