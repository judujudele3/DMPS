#ifndef IMAGEROTATEMODULE_HPP
#define IMAGEROTATEMODULE_HPP

#include "IModule.hpp"
#include "../data/ImageData.hpp"

enum class RotateDegree { R90, R180, R270 };

class ImageRotateModule : public IModule {
public:
    ImageRotateModule(RotateDegree deg) : degree(deg) {}
    std::string getName() const override;
    ModuleResult apply(IData& data) override;
    static std::vector<ModuleParameter> getParameterDescriptors();


private:
    RotateDegree degree;
};

#endif
