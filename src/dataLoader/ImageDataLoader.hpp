#ifndef IMAGEDATALOADER_HPP
#define IMAGEDATALOADER_HPP

#include "IDataLoader.hpp"
#include "../data/ImageData.hpp"
#include <memory>
#include <string>

class ImageDataLoader : public IDataLoader {
public:
    bool supports(const std::string& extension) const override;
    std::shared_ptr<IData> load(const std::string& path) override;
    std::string getExtension(const std::string& path) const override;
};

#endif // IMAGEDATALOADER_HPP

