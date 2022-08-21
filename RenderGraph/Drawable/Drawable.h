#pragma once
#include <memory>
#include <RenderGraph/RenderGraphStd.h>
#include <Dx12lib/dx12libStd.h>
#include <Dx12lib/Context/ContextStd.h>
#include <RenderGraph/Technique/TechniqueType.hpp>

namespace rg {

struct DrawArgs {
	size_t vertexCount = 0;
	size_t baseVertexLocation = 0;
	size_t indexCount = 0;
	size_t startIndexLocation = 0;
	size_t instanceCount = 1;
	size_t startInstanceLocation = 0;
};

class Drawable : public IDrawable {
public:
	Drawable() = default;
	~Drawable() override = default;
	void bind(dx12lib::IGraphicsContext &graphicsCtx) const override;
	void submit(const TechniqueFlag &techniqueFlag) const override;
	bool addTechnique(std::shared_ptr<Technique> pTechnique) override;
	bool hasTechnique(const TechniqueType &techniqueType) const override;
	bool removeTechnique(const TechniqueType &techniqueType) override;
	void setTechniqueActive(const TechniqueType &techniqueType, bool bActivate) override;
	void setDrawArgs(const DrawArgs &drawArgs);
	void setTopology(D3D_PRIMITIVE_TOPOLOGY topology);
	void setIndexBuffer(std::shared_ptr<dx12lib::IndexBuffer> pIndexBuffer);
	void setVertexBuffer(std::shared_ptr<dx12lib::VertexBuffer> pVertexBuffer, size_t slot);
	const DrawArgs &getDrawArgs() const;
	D3D_PRIMITIVE_TOPOLOGY getTopology() const;
	std::shared_ptr<dx12lib::IndexBuffer> getIndexBuffer() const;
	std::shared_ptr<dx12lib::VertexBuffer> getVertexBuffer(size_t slot) const;
	void genDrawArgs();
	void clearTechnique();
private:
	DrawArgs _drawArgs;
	TechniqueFlag _techniqueFlag = TechniqueType::None;
	D3D_PRIMITIVE_TOPOLOGY _topology = D3D_PRIMITIVE_TOPOLOGY_UNDEFINED;
	std::shared_ptr<dx12lib::IndexBuffer> _pIndexBuffer;
	std::shared_ptr<dx12lib::VertexBuffer> _pVertexBuffers[dx12lib::kVertexBufferSlotCount];
	std::vector<std::shared_ptr<Technique>> _techniques;
};

}
