#pragma once
#include <Dx12lib/dx12libStd.h>


namespace rgph {

struct DispatchArgs {
	size_t groupCountX = -1;	
	size_t groupCountY = 1;
	size_t groupCountZ = 1;
};

class Bindable;
class Computable {
public:
	Computable(std::weak_ptr<dx12lib::ComputePSO> pComputePSO);
	void addBindable(std::shared_ptr<Bindable> pBindable);
	void setDispatchArgs(const DispatchArgs &dispatchArgs);
	auto getDispatchArgs() const -> const DispatchArgs &;
	auto getName() const -> const std::string &;
	virtual void compute(dx12lib::IDirectContext &directCtx);
private:
	std::string _name;
	DispatchArgs _dispatchArgs;
	std::weak_ptr<dx12lib::ComputePSO> _pComputePSO;
	std::vector<std::shared_ptr<Bindable>> _bindables;
};

}
