#include "../../MulticlassInference/MulticlassInference/interface/hmc.h"

// -------------------------------------------------------------------------------------------------------------- //
//                                                                                                                //
//   class MulticlassInterface                                                                                    //
//                                                                                                                //
//   Class to compute DNN output from the Multiclass approach                                                     //
//                                                                                                                //
//   Author: Jaime Leon Holgado (CIEMAT)                                                                          //
//   Date  : June 2020                                                                                            //
//                                                                                                                //
// -------------------------------------------------------------------------------------------------------------- //

// Standard libraries
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <map>

// Multiclass DNN
#include "MulticlassInference/MulticlassInference/interface/hmc.h"

using namespace hmc;

// MulticlassInterface class
class MulticlassInterface {

  public:
    MulticlassInterface (int year, const std::string &version, const std::string &tag)
    : multiclass_year(year), multiclass_version(version), multiclass_tag(tag)
    {
        model = loadModel(multiclass_year, multiclass_version, multiclass_tag);
    }
    
    std::map<std::string, float> run(std::vector <float> input_features, long long ev_number)
    {
        // hmc::Model* model = loadModel(multiclass_year, multiclass_version, multiclass_tag);
        std::cout << input_features.size() << "/" <<model->getNumberOfFeatures() << " features included"<<std::endl; 

        model->input.clear();
        for (size_t i = 0; i < input_features.size(); i++)
          {
            std::string featureName = model->getFeatureName(i);
            if (input_features[i] != -999.)
              {
                model->input.setValue(featureName, input_features[i]);
              }
            else
              {
                auto feature = model->getFeatureSpecs()[i];
                model->input.setValue(featureName, feature.getDefaultValue());
              }
          }
        // if (multiclass_year == 2018 && multiclass_version.compare("V0") == 0 && multiclass_tag.compare("kl1_c2v1_c31"))
        //    fill_v0_kl1_c2v1_c31(input_features, model);
        model->run(ev_number);
        std::map <std::string, float> model_outputs;
        for (const auto& it : model->output)
          {
            model_outputs[it.first] = it.second;    
          }
        return model_outputs;
    }
    /*
    void fill_v0_kl1_c2v1_c31(std::vector <float> input_features, hmc::Model* model)
    {
        if ((int)model->getNumberOfFeatures() != (int)input_features.size())
        {
            std::cout << "Error" << std::endl; 
            return;
        }
        for (size_t i = 0; i < input_features.size(); i++)
          {
            if (input_features[i] != -999)
              {
                std::string featureName = model->getFeatureName(i);
                model->input.setValue(featureName, input_features[i]);
              }
          }
    }
    */
  private:
    hmc::Model* model;
    int multiclass_year; 
    const std::string &multiclass_version; 
    const std::string &multiclass_tag; 
};


/*
//#include "MulticlassInterface.h"
// Constructor
MulticlassInterface::MulticlassInterface (int year, std::string version, std::string tag)
: multiclass_year(year), multiclass_version(version), multiclass_tag(tag)
{
    
}


// Destructor
MulticlassInterface::~MulticlassInterface() {}


// Run
std::map<std::string, float> MulticlassInterface::run(std::vector <float> input_features, int ev_number)
{
    hmc::Model* model = hmc::loadModel(multiclass_year, multiclass_version, multiclass_tag);
    model->input.clear();
    if (multiclass_year == 2018 && multiclass_version.compare("V0") == 0 && multiclass_tag.compare("kl1_c2v1_c31"))
        fill_v0_kl1_c2v1_c31(input_features, model);
    model->run(ev_number);
    std::map <std::string, float> model_outputs;
    for (const auto& it : model_outputs)
      {
        model_outputs[it.first] = it.second;    
      }
    return model_outputs;
}


void MulticlassInterface::fill_v0_kl1_c2v1_c31(std::vector <float> input_features, hmc::Model* model)
{
    for (size_t i = 0; i < input_features.size(); i++)
      {
        if (input_features[i] != -999)
          {
            std::string featureName = model->getFeatureName(i);
            model->input.setValue(featureName, input_features[i]);
          }
      }
}
*/
