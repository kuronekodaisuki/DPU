#include "YOLOv8.hpp"

YOLOv8::YOLOv8(): DPU()
{

}

YOLOv8::~YOLOv8()
{

}

std::vector<std::vector<Object>> YOLOv8::Detect(std::vector<string> image_files)
{
    std::vector<std::vector<Object>> objects;

    return objects;
}

std::vector<Object> YOLOv8::Detect(cv::Mat image)
{
    std::vector<Object> objects;

    postProcess(_width, _height, _width / (float)image.cols, _height / (float)image.rows);

    return objects;
}

void YOLOv8::postProcess(const int image_w, const int image_h, float scaleX, float scaleY)
{

}
    
void YOLOv8::generate_grids_and_stride()
{
    std::vector<int> strides = { 8, 16, 32 };

    for (auto stride : strides)
    {
        int num_grid_y = _height / stride;
        int num_grid_x = _width / stride;
        for (int g1 = 0; g1 < num_grid_y; g1++)
        {
            for (int g0 = 0; g0 < num_grid_x; g0++)
            {
                _grid_strides.push_back(GridAndStride(g0, g1, stride));
            }
        }
    }
    //_numClasses = _outputShape[2] - 5;
}

void YOLOv8::generate_yolox_proposals(float prob_threshold)
{
    const size_t num_anchors = _grid_strides.size();

    for (size_t anchor_idx = 0; anchor_idx < num_anchors; anchor_idx++)
    {
        const int grid0 = _grid_strides[anchor_idx].grid0;
        const int grid1 = _grid_strides[anchor_idx].grid1;
        const int stride = _grid_strides[anchor_idx].stride;

        const size_t offset = anchor_idx * (_numClasses + 5);

        // yolox/models/yolo_head.py decode logic
        float x_center = (_results[offset + 0] + grid0) * stride;
        float y_center = (_results[offset + 1] + grid1) * stride;
        float w = exp(_results[offset + 2]) * stride;
        float h = exp(_results[offset + 3]) * stride;
        float box_objectness = _results[offset + 4];

        float x0 = x_center - w / 2;
        float y0 = y_center - h / 2;

        cv::Rect_<float> bound(x0, y0, w, h);

        for (size_t class_idx = 0; class_idx < _numClasses; class_idx++)
        {
            float box_cls_score = _results[offset + 5 + class_idx];
            float box_prob = box_objectness * box_cls_score;
            if (prob_threshold < box_prob)
            {
                Object obj;
                obj.rect = bound;
                obj.label = (int)class_idx;
                obj.prob = box_prob;

                _proposals.push_back(obj);
            }

        } // class loop

    } // point anchor loop
}

void YOLOv8::nms_sorted_bboxes(const std::vector<Object>& objects, std::vector<int>& picked, float nms_threshold)
{
    picked.clear();

    const size_t n = objects.size();

    std::vector<float> areas(n);
    for (size_t i = 0; i < n; i++)
    {
        areas[i] = objects[i].rect.area();
    }

    for (size_t i = 0; i < n; i++)
    {
        const Object& a = objects[i];

        int keep = 1;
        for (int j = 0; j < (int)picked.size(); j++)
        {
            const Object& b = objects[picked[j]];

            // intersection over union
            float inter_area = (a.rect & b.rect).area();
            float union_area = areas[i] + areas[picked[j]] - inter_area;
            // float IoU = inter_area / union_area
            if (nms_threshold < inter_area / union_area)
                keep = 0;
        }

        if (keep)
            picked.push_back((int)i);
    }
}
