/*
 * Copyright 2021 Xilinx Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
/*
 * Filename: yolov8.hpp
 *
 * Description:
 * This network is used to detecting objects from an image, it will return
 * its coordinate, label and confidence.
 *
 * Please refer to document "xilinx_XILINX_AI_SDK_user_guide.pdf" for more
 * details of these APIs.
 */
#pragma once
#include <vitis/ai/proto/dpu_model_param.pb.h>

#include <vitis/ai/library/tensor.hpp>

namespace vitis {
namespace ai {

/**
 *@struct YOLOv8Result
 *@brief Struct of the result returned by the YOLOv8 neural network.
 */
struct YOLOv8Result 
{
  /**
   *@struct BoundingBox
   *@brief Struct of detection result with an object.
   */
  struct BoundingBox {
    /// Classification.
    int label;
    /// Confidence. The value ranges from 0 to 1.
    float score;
    /// (x0,y0,x1,y1). x0, x1 Range from 0 to the input image columns.
    /// y0,y1. Range from 0 to the input image rows.
    std::vector<float> box;
  };
  /// All objects, The vector of BoundingBox.
  std::vector<BoundingBox> bboxes;
};

/**
 *@brief Post-process of the YOLOv8 neural network in batch mode.
 *@param input_tensors A vector of all input-tensors in the network.
 * Usage: input_tensors[input_tensor_index].
 *@param output_tensors A vector of all output-tensors in the network.
 *Usage: output_tensors[output_index].
 *@param config The dpu model configuration information.
 *@param scale The vector of scale of origin image.
 *@param left The vector of left padding position of network input size. Please
 see code for details.
 *@param top The vector of top padding position of network input size. Please
 see code for details.

 *@return The vector of struct of YOLOv8Result.
 */
std::vector<YOLOv8Result> yolov8_post_process(
    const std::vector<vitis::ai::library::InputTensor>& input_tensors,
    const std::vector<vitis::ai::library::OutputTensor>& output_tensors,
    const vitis::ai::proto::DpuModelParam& config,
    const std::vector<float>& scale, const std::vector<int>& left,
    const std::vector<int>& top);

/**
 * @cond NOCOMMENTS
 */
void yolov8_middle_process(
    const vitis::ai::library::InputTensor& input_tensor,
    const vitis::ai::library::OutputTensor& output_tensor);

/**
 * @endcond
 */
}  // namespace ai
}  // namespace vitis

