/******************************************************************************
 * Project      : Middle Course Test
 * File         : GoodMatches.cpp
 * Authors      : Pooya Nasiri , 
 * Emails       : pooya.nasiri@studenti.unipd.it , 
 * Affiliation  : University of Padova – MSc in Computer Vision
 * Supervisor   : Prof. Stefano Ghidoni
 * Date         : 2025-April
 ******************************************************************************/

#include "headers.h"
#include "GoodMatches.h"

std::vector<cv::DMatch> ratioTestMatch(const cv::Mat &desc1, const cv::Mat &desc2)
{
    cv::BFMatcher matcher(cv::NORM_L2);
    std::vector<std::vector<cv::DMatch>> matchesAB, matchesBA;

    matcher.knnMatch(desc1, desc2, matchesAB, 2);
    matcher.knnMatch(desc2, desc1, matchesBA, 2);

    std::vector<cv::DMatch> goodMatches;
    for (size_t i = 0; i < matchesAB.size(); ++i)
    {
        if (matchesAB[i].size() < 2)
            continue;
        const cv::DMatch &mAB = matchesAB[i][0];
        if (mAB.distance >= 0.75 * matchesAB[i][1].distance)
            continue;

        for (const auto &mBA : matchesBA[mAB.trainIdx])
            if (mBA.trainIdx == mAB.queryIdx)
            {
                goodMatches.push_back(mAB);
                break;
            }
    }

    return goodMatches;
}