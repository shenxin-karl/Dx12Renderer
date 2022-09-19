#include "Texture.h"
#include "Dx12lib/Device/Device.h"
#include "Dx12lib/Resource/ResourceStateTracker.h"

namespace dx12lib {

Texture::Texture(std::weak_ptr<Device> pDevice, const D3D12_RESOURCE_DESC &desc,
	const D3D12_CLEAR_VALUE *pClearValue)
{
	setDevice(pDevice);

	if (pClearValue != nullptr)
		_clearValue = *pClearValue;
	else
		std::memset(&_clearValue, 0, sizeof(_clearValue));

	_resourceDesc = desc;
	assert(getDimension() != TextureDimension::UNKNOWN);

#if defined(DEBUG) || defined(_DEBUG)
	if (desc.Flags & D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET || desc.Flags & D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL)
		assert(pClearValue != nullptr);
#endif

	auto pSharedDevice = pDevice.lock();
	auto pd3dDevice = pSharedDevice->getD3DDevice();
	ThrowIfFailed(pd3dDevice->CreateCommittedResource(
		RVPtr(CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT)),
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_COMMON,
		pClearValue,
		IID_PPV_ARGS(&_pResource)
	));

	pSharedDevice->getGlobalResourceState()->addGlobalResourceState(_pResource.Get(), D3D12_RESOURCE_STATE_COMMON);
	initFeatureSupport(pd3dDevice, desc.Format);
}

Texture::Texture(std::weak_ptr<Device> pDevice, WRL::ComPtr<ID3D12Resource> pResource,
	const D3D12_CLEAR_VALUE *pClearValue) {
}

auto Texture::getDimension() const -> TextureDimension {
	switch (_resourceDesc.Dimension) {
	case D3D12_RESOURCE_DIMENSION_TEXTURE1D: 
		return TextureDimension::TEXTURE_1D;
	case D3D12_RESOURCE_DIMENSION_TEXTURE2D: 
		return TextureDimension::TEXTURE_2D;
	case D3D12_RESOURCE_DIMENSION_TEXTURE3D: 
		return TextureDimension::TEXTURE_3D;
	case D3D12_RESOURCE_DIMENSION_UNKNOWN: 
	case D3D12_RESOURCE_DIMENSION_BUFFER: 
	default: 
		return TextureDimension::UNKNOWN;
	}
}

auto Texture::getDesc() const -> const D3D12_RESOURCE_DESC & {
	return _resourceDesc;
}

auto Texture::getArraySize() const -> size_t {
	return _resourceDesc.DepthOrArraySize;
}

auto Texture::getSRV(size_t mipSlice) const -> const ShaderResourceView * {

}

auto Texture::getUAV(size_t mipSlice) const -> const UnorderedAccessView * {

}

auto Texture::getRTV(size_t mipSlice) const -> const RenderTargetView * {
}

auto Texture::getDSV(size_t mipSlice) const -> const DepthStencilView * {
}

auto Texture::getPlaneSRV(size_t planeSlice, size_t mipSlice) const -> const ShaderResourceView * {
}

auto Texture::getPlaneUAV(size_t planeSlice, size_t mipSlice) const -> const UnorderedAccessView * {
}

auto Texture::getPlaneRTV(size_t planeSlice, size_t mipSlice) const -> const RenderTargetView * {
}

auto Texture::getPlaneDSV(size_t planeSlice, size_t mipSlice) const -> const DepthStencilView * {
}

auto Texture::getCubeSRV(size_t mipSlice) const -> const ShaderResourceView * {
}

auto Texture::getCubeUAV(size_t mipSlice) const -> const UnorderedAccessView * {
}

auto Texture::getCubeRTV(size_t mipSlice) const -> const RenderTargetView * {
}

auto Texture::getCubeDSV(size_t mipSlice) const -> const DepthStencilView * {
}

Texture::~Texture() {
}

D3D12_RESOURCE_DESC Texture::make2D(size_t width, size_t height, D3D12_RESOURCE_FLAGS flags, size_t numMipMap) {
}

D3D12_RESOURCE_DESC Texture::make2DArray(size_t width, size_t height, size_t arraySize, 
	D3D12_RESOURCE_FLAGS flags, size_t numMipMap)
{
}

D3D12_RESOURCE_DESC Texture::makeCube(size_t width, size_t height, D3D12_RESOURCE_DESC flags, size_t numMipMap) {
}

bool Texture::checkSRVSupport() const {
	return checkFormatSupport(D3D12_FORMAT_SUPPORT1_SHADER_SAMPLE);
}        

bool Texture::checkUAVSupport() const {
	return checkFormatSupport(D3D12_FORMAT_SUPPORT1_RENDER_TARGET);
}

bool Texture::checkRTVSupport() const {
	return checkFormatSupport(D3D12_FORMAT_SUPPORT1_TYPED_UNORDERED_ACCESS_VIEW) &&
		   checkFormatSupport(D3D12_FORMAT_SUPPORT2_UAV_TYPED_LOAD)				 &&
		   checkFormatSupport(D3D12_FORMAT_SUPPORT2_UAV_TYPED_STORE)				 ;
}

bool Texture::checkDSVSupport() const {
	return checkFormatSupport(D3D12_FORMAT_SUPPORT1_DEPTH_STENCIL);
}

void Texture::initFeatureSupport(ID3D12Device *pDevice, DXGI_FORMAT format) {
	_formatSupport.Format = format;
	ThrowIfFailed(pDevice->CheckFeatureSupport(
		D3D12_FEATURE_FORMAT_SUPPORT,
		&_formatSupport,
		sizeof(_formatSupport)
	));
}

bool Texture::checkFormatSupport(D3D12_FORMAT_SUPPORT1 formatSupport1) const {
	return _formatSupport.Support1 & formatSupport1;
}

bool Texture::checkFormatSupport(D3D12_FORMAT_SUPPORT2 formatSupport2) const {
	return _formatSupport.Support2 & formatSupport2;
}

}
