#include "SubPass.h"

#include "RenderGraph/Bindable/GraphicsPSOBindable.h"

namespace rgph {

SubPass::SubPass(std::shared_ptr<GraphicsPSOBindable> pGraphicsBindable)
: _pGraphicsPsoBindable(std::move(pGraphicsBindable))
{
	assert(pGraphicsBindable != nullptr);
}

const std::string &SubPass::getSubPassName() const {
	return _pGraphicsPsoBindable->getPSOName();
}

void SubPass::addBindable(std::shared_ptr<Bindable> pBindable) {
	_bindables.push_back(std::move(pBindable));
}

std::shared_ptr<Bindable> SubPass::getBindableByType(BindableType bindableType) const {
	for (auto &pBindable : _bindables) {
		if (pBindable->getBindableType() == bindableType)
			return pBindable;
	}
	return nullptr;
}

void SubPass::accept(const Job &job) {
	_jobs.push_back(job);
}

void SubPass::execute(dx12lib::IGraphicsContext &graphicsCtx) const {
	if (_jobs.empty())
		return;

	_pGraphicsPsoBindable->bind(graphicsCtx);
	for (auto &pBindable : _bindables)
		pBindable->bind(graphicsCtx);

	for (auto &job : _jobs)
		job.execute(graphicsCtx, _vertexDataSlots, _transformCBufferShaderRegister);
}

void SubPass::reset() {
	_jobs.clear();
}

SubPass::~SubPass() {
}

size_t SubPass::getJobCount() const {
	return _jobs.size();
}

void SubPass::setVertexDataInputSlots(const VertexInputSlot &inputSlot) {
	_vertexDataSlots = inputSlot;
}

void SubPass::setTransformCBufferShaderRegister(const dx12lib::ShaderRegister &transShaderRegister) {
	_transformCBufferShaderRegister = transShaderRegister;
}

void SubPass::setPassCBufferShaderRegister(const dx12lib::ShaderRegister &passShaderRegister) {
	_passCBufferShaderRegister = passShaderRegister;
}

const VertexInputSlot &SubPass::getVertexDataInputSlots() const {
	return _vertexDataSlots;
}

const dx12lib::ShaderRegister &SubPass::getTransformCBufferShaderRegister() const {
	return _transformCBufferShaderRegister;
}

const dx12lib::ShaderRegister &SubPass::getPassCBufferShaderRegister() const {
	return _passCBufferShaderRegister;
}

}
