#pragma once
#include <string>
#include <RenderGraph/RenderGraphStd.h>

namespace rgph {

class Material : public NonCopyable {
public:
	Material(std::string name);
	const std::string &getMaterialName() const;
	size_t getNumTechnique() const;
	std::shared_ptr<Technique> getTechnique(size_t idx) const;
	~Material() override;
protected:
	std::string _materialName;
	std::vector<std::shared_ptr<Technique>> _techniques;
};

}
