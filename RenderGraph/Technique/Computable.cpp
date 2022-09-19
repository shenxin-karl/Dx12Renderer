#include "Computable.h"
#include "Dx12lib/Pipeline/PipelineStateObject.h"
#include "RenderGraph/Bindable/Bindable.hpp"

namespace rgph {

Computable::Computable(std::weak_ptr<dx12lib::ComputePSO> pComputePSO)
: _pComputePSO(pComputePSO)
{
	auto pSharedComputePSO = _pComputePSO.lock();
	assert(pSharedComputePSO != nullptr);
	_name = pSharedComputePSO->getName();
}

void Computable::addBindable(std::shared_ptr<Bindable> pBindable) {
	_bindables.push_back(std::move(pBindable));
}

void Computable::setDispatchArgs(const DispatchArgs &dispatchArgs) {
	_dispatchArgs = dispatchArgs;
}

auto Computable::getDispatchArgs() const -> const DispatchArgs & {
	return _dispatchArgs;
}

auto Computable::getName() const -> const std::string & {
	return _name;
}

void Computable::compute(dx12lib::IDirectContext &directCtx) {
	auto pSharedComputePSO = _pComputePSO.lock();
	assert(pSharedComputePSO != nullptr);
	for (auto &pBindable : _bindables)
		pBindable->bind(directCtx);

	directCtx.setComputePSO(pSharedComputePSO);
	assert(_dispatchArgs.groupCountX != -1);
	directCtx.dispatch(_dispatchArgs.groupCountX, _dispatchArgs.groupCountY, _dispatchArgs.groupCountZ);
}

}
