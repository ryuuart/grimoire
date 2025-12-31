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

#pragma region Evals

double Parameters::evalRotation(const TD::OP_Inputs *inputs) {
    return inputs->getParDouble(RotationName);
}

const TD::OP_CHOPInput *Parameters::evalScale(const TD::OP_Inputs *inputs) {
    return inputs->getParCHOP(ScaleName);
}

int Parameters::evalCount(const TD::OP_Inputs *inputs) {
    return inputs->getParInt(CountName);
}

#pragma endregion

#pragma region Setup

void Parameters::setup(TD::OP_ParameterManager *manager) {
    {
        TD::OP_NumericParameter p;
        p.name = RotationName;
        p.label = RotationLabel;
        p.page = "Transforms";
        p.defaultValues[0] = 0.0;
        p.minSliders[0] = 0.0;
        p.maxSliders[0] = 1.0;
        p.minValues[0] = 0.0;
        p.maxValues[0] = 1.0;
        p.clampMins[0] = false;
        p.clampMaxes[0] = false;
        TD::OP_ParAppendResult res = manager->appendFloat(p);

        assert(res == TD::OP_ParAppendResult::Success);
    }

    {
        TD::OP_StringParameter p;
        p.name = ScaleName;
        p.label = ScaleLabel;
        p.page = "Transforms";
        p.defaultValue = "";
        TD::OP_ParAppendResult res = manager->appendCHOP(p);

        assert(res == TD::OP_ParAppendResult::Success);
    }

    {
        TD::OP_NumericParameter p;
        p.name = CountName;
        p.label = CountLabel;
        p.page = "Transforms";
        p.defaultValues[0] = 0;
        p.minSliders[0] = 0.0;
        p.maxSliders[0] = 10.0;
        p.minValues[0] = 0.0;
        p.maxValues[0] = 1.0;
        p.clampMins[0] = false;
        p.clampMaxes[0] = false;
        TD::OP_ParAppendResult res = manager->appendInt(p);

        assert(res == TD::OP_ParAppendResult::Success);
    }
}

#pragma endregion
