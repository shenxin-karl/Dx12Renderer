#include "FullScreenRenderable.h"
#include "Dx12lib/Pipeline/PipelineStateObject.h"

namespace rgph {

FullScreenRenderable::FullScreenRenderable(std::weak_ptr<dx12lib::GraphicsPSO> pGraphicsPSO) {
	setGraphicsPSO(std::move(pGraphicsPSO));
}

auto FullScreenRenderable::getName() const -> const std::string & {
	return _name;
}

void FullScreenRenderable::setGraphicsPSO(std::weak_ptr<dx12lib::GraphicsPSO> pGraphicsPSO) {
	_pGraphicsPSO = std::move(pGraphicsPSO);
	_name = _pGraphicsPSO.lock()->getName();
}

auto FullScreenRenderable::getGraphicsPSO() const -> std::shared_ptr<dx12lib::GraphicsPSO> {
	return _pGraphicsPSO.lock();
}

void FullScreenRenderable::setVertexBuffer(std::shared_ptr<dx12lib::VertexBuffer> pVertexBuffer) {
	_pVertexBuffer = std::move(pVertexBuffer);
}

auto FullScreenRenderable::getVertexBuffer() const -> std::shared_ptr<dx12lib::VertexBuffer> {
	return _pVertexBuffer;
}

void FullScreenRenderable::setVertexInputSlot(size_t slot) {
	_vertexInputSlot = slot;
}

auto FullScreenRenderable::getVertexInputSlot() const -> size_t {
	return _vertexInputSlot;
}

void FullScreenRenderable::execute(dx12lib::IGraphicsContext &graphicsCtx) {
	assert(_pVertexBuffer != nullptr);

}

}
