# 🔍 Classical Object Detection — Computer Vision Project (CV25)

**Author:** Pooya Nasiri  
**University of Padova — Master's in Computer Engineering**  
**Course:** Computer Vision  
**Instructor:** Prof. Stefano Ghidoni  
**Date:** April, 2025

---

## 📘 Project Overview

This project implements a classical **object detection system** capable of identifying known objects in real-world RGB scenes, using **feature-based matching** — without any deep learning.

The detector is designed for 3 specific objects:

- 🧃 Mustard Bottle
- 📦 Sugar Box
- 🔧 Power Drill

It locates each object in an image by comparing features from synthetic model views and highlights them with a **bounding box**.

---

## ⚙️ Methodology

The system is implemented entirely in **C++** using **OpenCV 3**. It follows these main steps:

1. **Keypoint Extraction**
   - SIFT features are extracted from the model views (masked by segmentation).
   - Scene images are processed without a mask.

2. **Feature Matching**
   - Lowe's ratio test
   - Optional symmetric matching (cross-check)

3. **Geometric Verification**
   - RANSAC-based homography estimation
   - Inlier-based confidence scoring
   - Bounding box projection from object mask bounding region

4. **Bounding Box Generation**
   - Homography is used to project the mask-bounding rectangle to the scene.
   - Output is both saved as image + text.

---

## 📂 Dataset Structure

Each object has its own folder with the following contents:

```
<OBJECT_ID>_<OBJECT_NAME>/
├── models/           ← 60 synthetic views (_color.png + _mask.png)
├── test_images/      ← RGB scene images
├── labels/           ← Ground truth bounding boxes
```

Example folder: `004_sugar_box/`

---

## 🧠 Features

- ✅ Feature masking using binary segmentation masks
- ✅ Adaptive bounding box using actual object shape
- ✅ Homography refinement using inliers only
- ✅ Optional symmetric matching
- ✅ Filter on match density, geometry, and bounding box shape
- ✅ Modular C++ design (separated logic: Features, Matching, Main)

---

## 🧪 Evaluation

To assess detection quality, the following metrics are supported:

- **Mean IoU (Intersection over Union)** across all test images
- **Detection Accuracy**, based on IoU > 0.5 with ground truth

---

## 🧑‍💻 Compilation

This project requires:

- GCC or Clang with C++17 support
- OpenCV 3.x

Use the provided `Makefile`:

```bash
make
```

Clean build:
```bash
make clean
```

---

## 🚀 Running the Detector

```bash
./Main <object_folder_path>
```

Example:

```bash
./Main 004_sugar_box/
```

The results are saved in `results/`:
- `.jpg`: images with bounding boxes
- `.txt`: bounding box coordinates in format:
  ```
  <object_id>_<object_name> <xmin> <ymin> <xmax> <ymax>
  ```

---

## License & Contact
- See the repository for license details.
- Author: [Pooya Nasiri](https://github.com/PooyaNasiri) | [Portfolio](https://pooyanasiri.github.io)
  
---

## 🗃️ Output Sample

```
results/
├── 004_color_0001.jpg      ← output image with box
├── 004_color_0001.txt      ← bounding box coordinates
```

---
