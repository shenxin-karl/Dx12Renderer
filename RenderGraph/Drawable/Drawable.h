#pragma once
#include <memory>
#include <RenderGraph/RenderGraphStd.h>
#include <Dx12lib/dx12libStd.h>
#include <Dx12lib/Context/ContextStd.h>
#include <RenderGraph/Technique/TechniqueType.hpp>

namespace rgph {

class Drawable : public IDrawable {
public:
	Drawable() = default;
	~Drawable() override = default;
	void submit(const TechniqueFlag &techniqueFlag) const override;
	bool addTechnique(std::shared_ptr<Technique> pTechnique) override;
	bool hasTechnique(const TechniqueType &techniqueType) const override;
	bool removeTechnique(const TechniqueType &techniqueType) override;
	void setTechniqueActive(const TechniqueType &techniqueType, bool bActivate) override;
	void clearTechnique();
	std::shared_ptr<Geometry> getGeometry() const;
	std::shared_ptr<TransformCBuffer> getTransformCBuffer() const;
	void setGeometry(std::shared_ptr<Geometry> pGeometry);
	void setGeometry(std::shared_ptr<TransformCBuffer> pTransformCBuf);
private:
	TechniqueFlag _techniqueFlag = TechniqueType::None;
	std::shared_ptr<Geometry> _pGeometry;
	std::shared_ptr<TransformCBuffer> _pTransformCBuf;
	std::vector<std::shared_ptr<Technique>> _techniques;
};

}
