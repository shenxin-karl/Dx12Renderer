#include "GraphicsPSOBindable.h"
#include "Dx12lib/Pipeline/PipelineStateObject.h"

namespace rgph {

std::shared_ptr<GraphicsPSOBindable> GraphicsPSOBindable::make(std::shared_ptr<dx12lib::GraphicsPSO> pso,
	const std::source_location &sourceLocation)
{
	auto pBindable = std::make_shared<GraphicsPSOBindable>(sourceLocation);
	pBindable->_pso = pso;
	return pBindable;
}

void GraphicsPSOBindable::bind(dx12lib::IGraphicsContext &graphicsCtx) const {
	graphicsCtx.setGraphicsPSO(_pso);
}

const std::string & GraphicsPSOBindable::getPSOName() const {
	return _pso->getName();
}

}
