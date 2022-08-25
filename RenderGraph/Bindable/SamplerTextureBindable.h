#pragma once
#include <RenderGraph/RenderGraphStd.h>
#include <RenderGraph/Bindable/Bindable.hpp>
#include <Dx12lib/Texture/SamplerTexture.h>
#include <Dx12lib/Pipeline/ShaderRegister.hpp>

namespace rgph {

class SamplerTextureBindable : public Bindable {
public:
	static std::shared_ptr<SamplerTextureBindable> make(dx12lib::ShaderRegister shaderRegister, 
		std::shared_ptr<dx12lib::IShaderResource> pShaderResource, 
		size_t mipMap = 0,
		DECLARE_SOURCE_LOCATION_ARG
	);
	void bind(dx12lib::IGraphicsContext &graphicsCtx) const override;
	SamplerTextureBindable(const std::source_location &sr) : Bindable(BindableType::SamplerTexture, sr) {}
private:
	size_t _mipMap;
	dx12lib::ShaderRegister _shaderRegister;
	std::shared_ptr<dx12lib::IShaderResource> _pShaderResource;
};

}
