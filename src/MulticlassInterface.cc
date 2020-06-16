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
        // Printing for debug, clean later
        std::cout << input_features.size() << "/" <<model->getNumberOfFeatures() << " features included"<<std::endl; 
        for (int i = 0; i<input_features.size(); i++){
            std::cout << model->getFeatureName(i) << " " << input_features[i] << std::endl; 
        }

        model->input.clear();
        for (size_t i = 0; i < input_features.size(); i++)
          {
            std::string featureName = model->getFeatureName(i);
            if (input_features[i] != -999.) // Checking no -999s
              {
                model->input.setValue(featureName, input_features[i]);
              }
            else // Change -999 to the feature's default value
              {
                auto feature = model->getFeatureSpecs()[i];
                model->input.setValue(featureName, feature.getDefaultValue());
              }
          }

        model->run(ev_number);
        std::map <std::string, float> model_outputs;
        for (const auto& it : model->output)
          {
            model_outputs[it.first] = it.second;    
          }
        return model_outputs;
    }

  private:
    hmc::Model* model;
    int multiclass_year; 
    const std::string &multiclass_version; 
    const std::string &multiclass_tag; 
};