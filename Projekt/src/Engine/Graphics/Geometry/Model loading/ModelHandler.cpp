#include "ModelHandler.h"

ModelHandler::ModelHandler() {

}

ModelHandler::~ModelHandler() {
	std::map<std::string, Model*>::iterator it;
	for (it = m_models.begin(); it != m_models.end(); it++)
	{
		delete it->second;
	}
}


 Model* ModelHandler::getModel(std::string path) {
	//Find model mapped to the key path
	auto search = m_models.find(path);
	if (search != m_models.end()) {
		//Return correct model
		return search->second;
	}
	else {
		//Load the model
		Model* tempModel;
		tempModel = new Model("res/Models/" + path);

		//Put it together with its path in m_models
		m_models.emplace(path, tempModel);
		//Return the newly loaded model
		return tempModel;
	}
}