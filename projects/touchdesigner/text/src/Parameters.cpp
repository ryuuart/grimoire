/* Shared Use License: This file is owned by Derivative Inc. (Derivative)
 * and can only be used, and/or modified for use, in conjunction with
 * Derivative's TouchDesigner software, and only if you are a licensee who has
 * accepted Derivative's TouchDesigner license or assignment agreement
 * (which also govern the use of this file). You may share or redistribute
 * a modified version of this file provided the following conditions are met:
 *
 * 1. The shared file or redistribution must retain the information set out
 * above and this list of conditions.
 * 2. Derivative's name (Derivative Inc.) or its trademarks may not be used
 * to endorse or promote products derived from this file without specific
 * prior written permission from Derivative.
 */

// Parameters.cpp generated using the cppParsTemplateGen Palette Component.
// https://derivative.ca/UserGuide/Palette:cppParsTemplateGen

#include "Parameters.h"
#include "CPlusPlus_Common.h"
#include <array>
#include <string>

#pragma region Evals

std::string Parameters::evalContent(const TD::OP_Inputs *inputs) {
    return inputs->getParString(ContentName);
}

std::string Parameters::evalTypefamily(const TD::OP_Inputs *inputs) {
    return inputs->getParString(TypefamilyName);
}

TD::Color Parameters::evalForegroundcolor(const TD::OP_Inputs *inputs) {
    std::array<double, 4> vals;
    inputs->getParDouble4(ForegroundcolorName, vals[0], vals[1], vals[2],
                          vals[3]);
    return TD::Color((float)vals[0], (float)vals[1], (float)vals[2],
                     (float)vals[3]);
}

int Parameters::evalFontsize(const TD::OP_Inputs *inputs) {
    return inputs->getParInt(FontsizeName);
}

#pragma endregion

#pragma region Setup

void Parameters::setup(TD::OP_ParameterManager *manager) {
    {
        TD::OP_StringParameter p;
        p.name = ContentName;
        p.label = ContentLabel;
        p.page = "Typography";
        p.defaultValue = "";
        TD::OP_ParAppendResult res = manager->appendString(p);

        assert(res == TD::OP_ParAppendResult::Success);
    }

    {
        TD::OP_StringParameter p;
        p.name = TypefamilyName;
        p.label = TypefamilyLabel;
        p.page = "Typography";
        p.defaultValue = "Helvetica";
        TD::OP_ParAppendResult res = manager->appendString(p);

        assert(res == TD::OP_ParAppendResult::Success);
    }

    {
        TD::OP_NumericParameter p;
        p.name = ForegroundcolorName;
        p.label = ForegroundcolorLabel;
        p.page = "Typography";

        const int ArraySize = 4;

        const std::array<double, ArraySize> DefaultValues = {0.0, 0.0, 0.0,
                                                             1.0};
        const std::array<double, ArraySize> MinSliders = {0.0, 0.0, 0.0, 0.0};
        const std::array<double, ArraySize> MaxSliders = {1.0, 1.0, 1.0, 1.0};
        const std::array<double, ArraySize> MinValues = {0.0, 0.0, 0.0, 0.0};
        const std::array<double, ArraySize> MaxValues = {1.0, 1.0, 1.0, 1.0};
        const std::array<bool, ArraySize> ClampMins = {false, false, false,
                                                       false};
        const std::array<bool, ArraySize> ClampMaxes = {false, false, false,
                                                        false};
        for (int i = 0; i < DefaultValues.size(); ++i) {
            p.defaultValues[i] = DefaultValues[i];
            p.minSliders[i] = MinSliders[i];
            p.maxSliders[i] = MaxSliders[i];
            p.minValues[i] = MinValues[i];
            p.maxValues[i] = MaxValues[i];
            p.clampMins[i] = ClampMins[i];
            p.clampMaxes[i] = ClampMaxes[i];
        }
        TD::OP_ParAppendResult res = manager->appendRGBA(p);

        assert(res == TD::OP_ParAppendResult::Success);
    }

    {
        TD::OP_NumericParameter p;
        p.name = FontsizeName;
        p.label = FontsizeLabel;
        p.page = "Typography";
        p.defaultValues[0] = 12;
        p.minSliders[0] = 0.0;
        p.maxSliders[0] = 128.0;
        p.minValues[0] = 0.0;
        p.maxValues[0] = 1.0;
        p.clampMins[0] = false;
        p.clampMaxes[0] = false;
        TD::OP_ParAppendResult res = manager->appendInt(p);

        assert(res == TD::OP_ParAppendResult::Success);
    }
}

#pragma endregion
