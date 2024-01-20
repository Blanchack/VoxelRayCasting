#pragma once

#include "Shader.h"

#include <vector>
#include <string>

class Model {
public:
	virtual ~Model() = default;

	virtual void serializedDataToShader(Shader shader) final;

	virtual std::vector<Voxel> getSerializedData()

private:

};
