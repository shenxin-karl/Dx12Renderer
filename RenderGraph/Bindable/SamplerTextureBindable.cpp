#include "SamplerTextureBindable.h"

namespace rgph {

std::shared_ptr<SamplerTextureBindable> SamplerTextureBindable::make(dx12lib::ShaderRegister shaderRegister,
	std::shared_ptr<dx12lib::IShaderResource> pShaderResource, 
	size_t mipMap,
	const std::source_location &sourceLocation)
{
	assert(shaderRegister.slot && !shaderRegister.slot.isSampler());
	assert(pShaderResource != nullptr);

	auto pBindable = std::make_shared<SamplerTextureBindable>(sourceLocation);
	pBindable->_mipMap = mipMap;
	pBindable->_shaderRegister = shaderRegister;
	pBindable->_pShaderResource = pShaderResource;
	return pBindable;
}

void SamplerTextureBindable::bind(dx12lib::IGraphicsContext &graphicsCtx) const {
	const auto &srv = _pShaderResource->getSRV(_mipMap);
	graphicsCtx.setShaderResourceView(_shaderRegister, srv);
}

}
