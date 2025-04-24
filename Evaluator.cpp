/******************************************************************************
 * Project      : Middle Course Test
 * File         : Evaluate.cpp
 * Authors      : Pooya Nasiri ,
 * Emails       : pooya.nasiri@studenti.unipd.it ,
 * Affiliation  : University of Padova – MSc in Computer Vision
 * Supervisor   : Prof. Stefano Ghidoni
 * Date         : 2025-April
 ******************************************************************************/

#include "headers.h"
#include "Evaluator.h"

namespace fs = std::filesystem;

float computeIoU(const Box &box1, const Box &box2)
{
    int xA = std::max(box1.xmin, box2.xmin);
    int yA = std::max(box1.ymin, box2.ymin);
    int xB = std::min(box1.xmax, box2.xmax);
    int yB = std::min(box1.ymax, box2.ymax);

    int interArea = std::max(0, xB - xA) * std::max(0, yB - yA);
    int box1Area = (box1.xmax - box1.xmin) * (box1.ymax - box1.ymin);
    int box2Area = (box2.xmax - box2.xmin) * (box2.ymax - box2.ymin);

    return float(interArea) / float(box1Area + box2Area - interArea);
}

bool readBoxFromTxt(const std::string &file, Box &box)
{
    std::ifstream in(file);
    std::string line;
    if (!std::getline(in, line))
        return false;

    std::istringstream iss(line);
    std::string id;
    if (!(iss >> id >> box.xmin >> box.ymin >> box.xmax >> box.ymax))
        return false;

    return true;
}

int evaluate(const std::string path)
{
    std::string predDir = path + "/results/";
    std::string gtDir = path + "/labels/";
    std::cout << "\n\nRunning Evaluation on: " << path << std::endl;

    int TP = 0;
    int total = 0;
    int detected = 0;
    float iouSum = 0.0f;

    for (const auto &entry : fs::directory_iterator(gtDir))
    {
        std::string gtPath = entry.path().string();
        std::string filename = entry.path().filename().string();
        std::string predPath = predDir + filename;
        size_t pos = predPath.find("-box.txt");
        if (pos != std::string::npos)
            predPath.replace(pos, std::string("-box.txt").length(), "-color.jpg.txt");
        Box gtBox, predBox;
        if (!readBoxFromTxt(gtPath, gtBox))
            continue;

        total++;

        if (!fs::exists(predPath) || !readBoxFromTxt(predPath, predBox))
        {
            std::cout << filename << " -> NOT DETECTED" << std::endl;
            continue;
        }

        detected++;
        float iou = computeIoU(gtBox, predBox);
        std::cout << filename << " -> IoU: " << iou << std::endl;

        iouSum += iou;
        if (iou > 0.5f)
            TP++;
    }

    float mIoU = (total > 0) ? iouSum / total : 0;
    float accuracy = (total > 0) ? detected * 100.0 / total : 0;
    std::cout << "\n--- Evaluation Summary ---" << std::endl;
    std::cout << "Total ground truth: " << total << std::endl;
    std::cout << "Detection Accuracy: " << accuracy << "%" << std::endl;
    std::cout << "True Positives (IoU > 0.5): " << TP << "/" << total << std::endl;
    std::cout << "mIoU: " << mIoU << std::endl;

    return 0;
}
