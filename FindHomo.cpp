/******************************************************************************
 * Project      : Middle Course Test
 * File         : FindHomo.cpp
 * Authors      : Pooya Nasiri , 
 * Emails       : pooya.nasiri@studenti.unipd.it , 
 * Affiliation  : University of Padova – MSc in Computer Vision
 * Supervisor   : Prof. Stefano Ghidoni
 * Date         : 2025-April
 ******************************************************************************/

#include "headers.h"
#include "GoodMatches.h"
#include "FindHomo.h"

bool findBestHomography(const std::vector<cv::KeyPoint> &sceneKp, const cv::Mat &sceneDesc,
                        const std::vector<std::vector<cv::KeyPoint>> &modelKpList,
                        const std::vector<cv::Mat> &modelDescList,
                        const std::vector<cv::Mat> &modelImages,
                        const std::vector<cv::Mat> &modelMasks,
                        const cv::Mat &sceneImage,
                        cv::Mat &bestH, std::vector<cv::Point2f> &projectedBox)

{
    int maxInliers = 0;
    float bestScore = 0.0f;

    for (size_t i = 0; i < modelKpList.size(); ++i)
    {
        const auto &modelKp = modelKpList[i];
        const auto &modelDesc = modelDescList[i];

        std::vector<cv::DMatch> goodMatches = ratioTestMatch(modelDesc, sceneDesc);
        if (goodMatches.size() < 5)
            continue;

        std::vector<cv::Point2f> modelPts, scenePts;
        for (const auto &match : goodMatches)
        {
            modelPts.push_back(modelKp[match.queryIdx].pt);
            scenePts.push_back(sceneKp[match.trainIdx].pt);
        }

        cv::Mat inlierMask;
        cv::Mat H = cv::findHomography(modelPts, scenePts, cv::RANSAC, 3.0, inlierMask);
        if (H.empty())
            continue;

        int numInliers = cv::countNonZero(inlierMask);
        if (numInliers < 5)
            continue;
        float currentScore = float(numInliers) / goodMatches.size();
        if (numInliers > maxInliers && currentScore > bestScore)
        {
            bestScore = currentScore;
            maxInliers = numInliers;
            std::vector<cv::Point2f> inlierModelPts, inlierScenePts;
            for (int j = 0; j < inlierMask.rows; ++j)
                if (inlierMask.at<uchar>(j))
                {
                    inlierModelPts.push_back(modelPts[j]);
                    inlierScenePts.push_back(scenePts[j]);
                }
            bestH = cv::findHomography(inlierModelPts, inlierScenePts, 0);
            cv::Size modelSize = modelImages[i].size();
            cv::Rect objectROI = cv::boundingRect(modelMasks[i]);
            std::vector<cv::Point2f> corners = {
                {float(objectROI.x), float(objectROI.y)},
                {float(objectROI.x + objectROI.width), float(objectROI.y)},
                {float(objectROI.x + objectROI.width), float(objectROI.y + objectROI.height)},
                {float(objectROI.x), float(objectROI.y + objectROI.height)}};
            cv::perspectiveTransform(corners, projectedBox, bestH);

            for (auto &pt : projectedBox)
            {
                pt.x = std::clamp(pt.x, 0.f, float(sceneImage.cols - 1));
                pt.y = std::clamp(pt.y, 0.f, float(sceneImage.rows - 1));
            }
            cv::Rect bounds = cv::boundingRect(projectedBox);
            if (bounds.area() > 0.9 * sceneImage.cols * sceneImage.rows || bounds.area() < 0.05 * sceneImage.cols * sceneImage.rows)
            {
                bestH.release();
                continue;
            }

            // std::cout << "Projecting box for model " << i << ": ";
            // for (const auto &pt : projectedBox)
            //     std::cout << "(" << pt.x << ", " << pt.y << ") ";
            // std::cout << std::endl;

            // cv::Mat matchImg;
            // cv::drawMatches(modelImages[i], modelKp, sceneImage, sceneKp, goodMatches, matchImg,
            //                 cv::Scalar::all(-1), cv::Scalar::all(-1),
            //                 std::vector<char>(), cv::DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
            // cv::imshow("img", matchImg);
            // cv::waitKey(0);
        }
    }

    return !bestH.empty();
}