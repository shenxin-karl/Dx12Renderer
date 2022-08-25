#pragma once
#include <RenderGraph/Bindable/Bindable.hpp>
#include <string>
#include "Dx12lib/Context/CommonContext.h"

namespace rgph {

class Pass;
class GraphicsPSOBindable : public Bindable {
public:
	static std::shared_ptr<GraphicsPSOBindable> make(std::shared_ptr<dx12lib::GraphicsPSO> pso, DECLARE_SOURCE_LOCATION_ARG);
	void bind(dx12lib::IGraphicsContext &graphicsCtx) const override;
	const std::string &getPSOName() const;
	GraphicsPSOBindable(const std::source_location &sr) : Bindable(BindableType::PipelineStateObject, sr) {}
private:
	std::shared_ptr<dx12lib::GraphicsPSO> _pso;
};

}
