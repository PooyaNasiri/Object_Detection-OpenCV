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

struct Box
{
    int xmin, ymin, xmax, ymax;
};

float computeIoU(const Box &box1, const Box &box2);

bool readBoxFromTxt(const std::string &file, Box &box);

int evaluate(const std::string path);