#include "DPU.hpp"
#include "Object.h"

#define NMS_THRESH 0.45f
#define BBOX_CONF_THRESH 0.3f

class YOLOv8: public DPU
{
public:
    /// @brief Constructor
    YOLOv8();

    /// @brief Destructor
    ~YOLOv8();

    /// @brief Detect objects
    /// @param image 
    /// @return returns detected objects
    std::vector<Object> Detect(cv::Mat image);

    std::vector<std::vector<Object>> Detect(std::vector<string> image_files);

protected:
    struct GridAndStride
    {
        int grid0;
        int grid1;
        int stride;

        GridAndStride(int g0, int g1, int s) :grid0(g0), grid1(g1), stride(s) {}
    };

    void postProcess(const int image_w, const int image_h, float scaleX, float scaleY);
    void generate_grids_and_stride();
    void generate_yolox_proposals(float prob_threshold);
    void nms_sorted_bboxes(const std::vector<Object>& faceobjects, std::vector<int>& picked, float nms_threshold);

private:
    float _nms_threshold = NMS_THRESH;
    float _bbox_confidential_threshold = BBOX_CONF_THRESH;
    uint _numClasses = 80;

    std::vector<Object> _proposals;
    std::vector<Object> _objects;

    std::vector<GridAndStride> _grid_strides;
};

