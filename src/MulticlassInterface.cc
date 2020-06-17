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

#include <boost/format.hpp>
#include <boost/algorithm/string/replace.hpp>

// Multiclass DNN
#include "MulticlassInference/MulticlassInference/interface/hmc.h"

using namespace hmc;

// MulticlassInterface class
class MulticlassInterface {

  public:
    MulticlassInterface (int year, const std::vector <std::pair <std::string, std::string>> models_def)
    : multiclass_year(year)
    {
        for (auto &model : models_def) {
            models.push_back(loadModel(multiclass_year, model.first, model.second));
        }
    }
    
    std::map<std::string, float> run(std::map <std::string, float> input_features, long long ev_number)
    {
        // Printing for debug, clean later
        // std::cout << input_features.size() << "/" <<model->getNumberOfFeatures() << " features included"<<std::endl; 
        // for (size_t i = 0; i<input_features.size(); i++){
        //     std::cout << model->getFeatureName(i) << " " << input_features[i] << std::endl; 
        // }
        std::map <std::string, float> model_outputs;
        for (auto &model : models) 
        {
            model->input.clear();
            for (size_t i = 0; i < model->getFeatureSpecs().size(); i++)
              {
                std::string feature_name = model->getFeatureName(i);
                if (input_features[feature_name] != -999.) // Checking no -999s
                  {
                    model->input.setValue(feature_name, input_features[feature_name]);
                  }
                else // Change -999 to the feature's default value
                  {
                    auto feature = model->getFeatureSpecs()[i];
                    model->input.setValue(feature_name, feature.getDefaultValue());
                  }
              }

            model->run(ev_number);
            for (const auto& it : model->output)
              {
                model_outputs[boost::str(boost::format("%s__%s__%s") % model->getVersion() % model->getTag() % it.first)] = it.second;    
              }
        }
        return model_outputs;
    }

  private:
    std::vector<hmc::Model*> models;
    int multiclass_year; 
};
