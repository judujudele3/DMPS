#include <iostream>

#include "../data/TextData.hpp"

// Modules de nettoyage
#include "../module/TextRemoveExtraSpacesModule.hpp"
#include "../module/TextTrimLinesModule.hpp"
#include "../module/TextLowerFirstLetterModule.hpp"

// Modules d’analyse
#include "../module/TextLineCountModule.hpp"
#include "../module/TextCharacterCountModule.hpp"
#include "../module/TextWordCountModule.hpp"
#include "../module/TextLengthModule.hpp"
#include "../module/TextTokenizeModule.hpp"
#include "../module/TextLengthDistributionModule.hpp"
#include "../module/TextTopWordsModule.hpp"
#include "../module/TextLanguageDetectionModule.hpp"
/*
int main() {

    std::string sampleText =
        "   Hello   DMPS   \n"
        " This   is   a test   \n"
        " Line three   hello world hello DMPS \n  " " Best regards,   \n";

    TextData text(sampleText);

    
    //   Modules de nettoyage
    
    TextRemoveExtraSpacesModule removeSpaces;
    TextTrimLinesModule trimLines;
    TextLowerFirstLetterModule lowerFirst;

    removeSpaces.apply(text);
    trimLines.apply(text);
    lowerFirst.apply(text);

    std::cout << "==== Cleaned Text ====\n";
    std::cout << text.getContent() << "\n\n";

    
      // Modules d’analyse
       
    TextLineCountModule lineModule;
    TextCharacterCountModule charModule;
    TextWordCountModule wordModule;
    TextLengthModule lengthModule;
    TextTokenizeModule tokenizeModule;
    TextLengthDistributionModule lengthDistModule;
    TextTopWordsModule topWordsModule(3);

    TextLanguageDetectionModule langModule;

    ModuleResult r1 = lineModule.apply(text);
    ModuleResult r2 = charModule.apply(text);
    ModuleResult r3 = wordModule.apply(text);
    ModuleResult r4 = lengthModule.apply(text);
    ModuleResult r5 = tokenizeModule.apply(text);
    ModuleResult r6 = lengthDistModule.apply(text);
    ModuleResult r7 = topWordsModule.apply(text);
    ModuleResult r8 = langModule.apply(text);

    /* =======================
       Affichage résultats
       =======================
       std::cout << langModule.getName()
          << ": " << r8.get<std::string>("Language") << std::endl;
          
    std::cout << lineModule.getName()
              << ": " << r1.get<int>("LineCount") << std::endl;

    std::cout << charModule.getName()
              << ": " << r2.get<int>("CharacterCount") << std::endl;

    std::cout << wordModule.getName()
              << ": " << r3.get<size_t>("WordCount") << std::endl;

    std::cout << lengthModule.getName()
              << ": " << r4.get<int>("ContentLength") << std::endl;

    std::cout << tokenizeModule.getName() << ": ";
    for (const auto& w : r5.get<std::vector<std::string>>("Tokens"))
        std::cout << w << " ";
    std::cout << std::endl;

    std::cout << lengthDistModule.getName() << ":\n";
    for (const auto& s : r6.get<std::vector<std::string>>("LengthDistribution"))
        std::cout << s << std::endl;

    std::cout << topWordsModule.getName() << ": ";
    for (const auto& w : r7.get<std::vector<std::string>>("TopWords"))
        std::cout << w << " ";
    std::cout << std::endl;

    return 0;
}
*/
