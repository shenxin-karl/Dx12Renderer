#pragma once
#include <RenderGraph/RenderGraphStd.h>
#include <RenderGraph/Bindable/Bindable.hpp>
#include <Dx12lib/Texture/SamplerTexture.h>
#include <Dx12lib/Pipeline/ShaderRegister.hpp>

namespace rgph {

class SamplerTextureBindable : public Bindable {
public:
	static std::shared_ptr<SamplerTextureBindable> make(dx12lib::ShaderRegister shaderRegister, 
		std::shared_ptr<dx12lib::ITextureResource> pShaderResource, 
		size_t mipMap = 0
	);
	void bind(dx12lib::IGraphicsContext &graphicsCtx) const override;
	SamplerTextureBindable() : Bindable(BindableType::SamplerTexture) {}
private:
	size_t _mipMap;
	dx12lib::ShaderRegister _shaderRegister;
	std::shared_ptr<dx12lib::ITextureResource> _pShaderResource;
};

}
