/******************************************************************************
 * Project      : Middle Course Test
 * File         : Main.cpp
 * Authors      : Pooya Nasiri ,
 * Emails       : pooya.nasiri@studenti.unipd.it ,
 * Affiliation  : University of Padova – MSc in Computer Vision
 * Supervisor   : Prof. Stefano Ghidoni
 * Date         : 2025-April
 ******************************************************************************/

#include "headers.h"
#include "Features.h"
#include "FindHomo.h"
#include "Evaluator.h"

namespace fs = std::filesystem;

int main(int argc, char **argv)
{
    std::vector<std::vector<cv::KeyPoint>> modelKp;
    std::vector<std::vector<cv::KeyPoint>> sceneKp;
    std::vector<std::string> sceneNames;
    std::vector<cv::Mat> modelDesc;
    std::vector<cv::Mat> sceneDesc;
    std::vector<cv::Mat> modelImages;
    std::vector<cv::Mat> modelMasks;

    if (argc < 2)
    {
        std::cout << "Usage: " << argv[0] << " <object_folder_path>\nExample:Main.exe 004_sugar_box";
        return 1;
    }
    std::string path = argv[1];
    loadModelFeatures(path + "/models/", modelKp, modelDesc, modelImages, modelMasks);
    loadSceneFeatures(path + "/test_images/", sceneNames, sceneKp, sceneDesc);

    std::string outputDir = path + "/results/";
    fs::create_directory(outputDir);

    for (size_t i = 0; i < sceneDesc.size(); ++i)
    {
        cv::Mat bestH;
        std::vector<cv::Point2f> bbox;
        std::string scenePath = path + "/test_images/" + sceneNames[i];
        cv::Mat scene = cv::imread(scenePath);
        bool found = findBestHomography(sceneKp[i], sceneDesc[i],
                                        modelKp, modelDesc,
                                        modelImages, modelMasks,
                                        scene, bestH, bbox);

        if (found)
        {
            std::vector<std::vector<cv::Point>> pts = {std::vector<cv::Point>(bbox.begin(), bbox.end())};
            cv::polylines(scene, pts, true, cv::Scalar(0, 255, 0), 2);
            float xmin = bbox[0].x, ymin = bbox[0].y;
            float xmax = bbox[0].x, ymax = bbox[0].y;
            for (const auto &pt : bbox)
            {
                xmin = std::min(xmin, pt.x);
                xmax = std::max(xmax, pt.x);
                ymin = std::min(ymin, pt.y);
                ymax = std::max(ymax, pt.y);
            }

            std::string id = std::to_string(i);
            std::ofstream outFile(outputDir + sceneNames[i] + ".txt");
            outFile << path << " " << int(xmin) << " " << int(ymin)
                    << " " << int(xmax) << " " << int(ymax) << std::endl;
            outFile.close();
            cv::imwrite(outputDir + sceneNames[i], scene);
            std::cout << "Saved: " << sceneNames[i] << " with bounding box" << std::endl;
        }
    }
    evaluate(path);
    return 0;
}