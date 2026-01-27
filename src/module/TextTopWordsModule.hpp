#ifndef TEXTTOPWORDSMODULE_HPP
#define TEXTTOPWORDSMODULE_HPP

#include "IModule.hpp"
#include "../data/TextData.hpp"
#include <vector>
#include <string>

class TextTopWordsModule : public IModule {
public:
    TextTopWordsModule(int topK = 5) : m_topK(topK) {}
    std::string getName() const override ;
    ModuleResult apply(IData& data) override;
    static std::vector<ModuleParameter> getParameterDescriptors();

private:
    int m_topK;
};

#endif
