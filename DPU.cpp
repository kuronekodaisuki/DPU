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
    std::vector<string> image_filenames;
    struct stat s;
    lstat(images_filepath, &s);

    if (S_IS_DIR(s.st_mode))
    {
        // Enumerate image file names
        DIR* dir = opendir(images_filepath);
        if (dir != nullptr)
        {
            for (struct dirent* entry = readdir(dir); entry != nullptr; entry = readdir(dir))
            {
                if ( entry->d_type == DT_REG || entry->d_type == DT_UNKNOWN)
                {
                    string name = entry->d_name;
                    string ext = name.substr(name.find_last(".") + 1);
                    switch (ext)
                    {
                        case "JPEG":
                        case "JPG":
                        case "jpeg":
                        case "jpg":
                        case "PNG":
                        case "png":
                        images.push_back(name);
                    }
                }
            }
            closedir(dir);
        }
    }
}