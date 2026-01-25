#include "TextTopWordsModule.hpp"
#include <sstream>
#include <unordered_map>
#include <algorithm>

ModuleResult TextTopWordsModule::apply(IData& data) {
    ModuleResult res;

    auto* textData = dynamic_cast<TextData*>(&data);
    if (!textData) {
        res.addError(ErrorType::Error, "INVALID_TYPE", "TextData expected");
        return res;
    }

    std::unordered_map<std::string,int> freqMap;
    std::istringstream iss(textData->getContent());
    std::string word;
    while (iss >> word) {
        freqMap[word]++;
    }

    // Mettre les paires (mot, fréquence) dans un vecteur
    std::vector<std::pair<std::string,int>> freqVec(freqMap.begin(), freqMap.end());

    // Trier par fréquence décroissante
    std::sort(freqVec.begin(), freqVec.end(), [](auto &a, auto &b){
        return a.second > b.second;
    });

    // Extraire top K
    std::vector<std::string> topWords;
    for (int i=0; i<std::min(m_topK, (int)freqVec.size()); ++i) {
        topWords.push_back(freqVec[i].first + " (" + std::to_string(freqVec[i].second) + ")");
    }

    res.set<std::vector<std::string>>("TopWords", topWords);
    res.message = "Top words computed successfully";
    return res;
}
