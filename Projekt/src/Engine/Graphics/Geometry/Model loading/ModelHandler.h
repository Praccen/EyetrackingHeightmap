#pragma once

#include <map>

#include "Model.h"

class ModelHandler {
private:
	std::map<std::string, Model*> m_models;

public:
	ModelHandler();
	~ModelHandler();

	Model* getModel(std::string path);
};