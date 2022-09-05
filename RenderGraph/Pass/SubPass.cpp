#include "SubPass.h"
#include "Dx12lib/Pipeline/PipelineStateObject.h"


namespace rgph {

SubPass::SubPass(std::weak_ptr<dx12lib::GraphicsPSO> pGraphicsPso)
: _pGraphicsPso(std::move(pGraphicsPso))
{
	auto pSharePso = _pGraphicsPso.lock();
	assert(pSharePso != nullptr);
	_subPassName = pSharePso->getName();
}

const std::string &SubPass::getSubPassName() const {
	return _subPassName;
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

void SubPass::bind(dx12lib::IGraphicsContext &graphicsCtx) const {
	graphicsCtx.setGraphicsPSO(_pGraphicsPso.lock());
}

void SubPass::accept(const Job &job) {
	_jobs.push_back(job);
}

void SubPass::execute(dx12lib::IGraphicsContext &graphicsCtx) const {
	execute(graphicsCtx, _jobs);
}

void SubPass::reset() {
	_jobs.clear();
}

SubPass::~SubPass() {
}

size_t SubPass::getJobCount() const {
	return _jobs.size();
}

const std::vector<Job> &SubPass::getJobs() const {
	return _jobs;
}

void SubPass::setVertexDataInputSlots(const VertexInputSlots &inputSlot) {
	_vertexDataSlots = inputSlot;
}

void SubPass::setTransformCBufferShaderRegister(const dx12lib::ShaderRegister &transShaderRegister) {
	_transformCBufferShaderRegister = transShaderRegister;
}

void SubPass::setPassCBufferShaderRegister(const dx12lib::ShaderRegister &passShaderRegister) {
	_passCBufferShaderRegister = passShaderRegister;
}

const VertexInputSlots &SubPass::getVertexDataInputSlots() const {
	return _vertexDataSlots;
}

const dx12lib::ShaderRegister &SubPass::getTransformCBufferShaderRegister() const {
	return _transformCBufferShaderRegister;
}

const dx12lib::ShaderRegister &SubPass::getPassCBufferShaderRegister() const {
	return _passCBufferShaderRegister;
}

const std::vector<std::shared_ptr<Bindable>> &SubPass::getBindables() const {
	return _bindables;
}

void SubPass::execute(dx12lib::IGraphicsContext &graphicsCtx, const std::vector<Job> &jobs) const {
	if (jobs.empty())
		return;

	for (auto &pBindable : _bindables)
		pBindable->bind(graphicsCtx);

	for (auto &job : jobs)
		job.execute(graphicsCtx, _vertexDataSlots, _transformCBufferShaderRegister);
}

bool SubPass::valid() const {
	return _pGraphicsPso.use_count() > 0;
}

}
