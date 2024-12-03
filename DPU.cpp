#include "DPU.hpp"

bool DPU::Load(const char* model_filename)
{
    auto graph = xir::Graph::deserialize(model_filename);
    auto subgraph = get_dpu_subgraph(graph.get());
    
    // Create runner
    _runner = vart::Runner::create_runner(subgraph[0], "run");

}

void::DPU::Run(const char* images_filepath)
{
    
}