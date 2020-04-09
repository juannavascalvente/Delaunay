//
// Created by delaunay on 29/3/20.
//

#ifndef DELAUNAY_COMMANDPARAMFACTORY_H
#define DELAUNAY_COMMANDPARAMFACTORY_H


class CommandParamFactory
{
    GeneratorCmdParamIn *createGeneratorParamIn(size_t szNumPoints) { new GeneratorCmdParamIn(szNumPoints); };
    GeneratorCmdParamOut *createGeneratorParamOut(Dcel &dcel) { new GeneratorCmdParamOut(dcel); };
};


#endif //DELAUNAY_COMMANDPARAMFACTORY_H
